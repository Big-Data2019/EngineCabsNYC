#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>
#include <CLI/CLI.hpp>
#include <string>
#include <vector>
#include <filesystem>
#include <re2/re2.h>
#include <memory>
#include <cstdlib>
#include <arrow/api.h>
#include <arrow/io/file.h>
#include <parquet/arrow/reader.h>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <fstream>
#include <chrono>
#include <ctime>
#include <utility>
#include <locale>

class CurlGlobal{
public:
    CurlGlobal(long flags = CURL_GLOBAL_ALL){
        if(curl_global_init(flags) != 0){
            throw std::runtime_error("Failed to global initialize libcurl handler");
        }
    }
    ~CurlGlobal(){
        curl_global_cleanup();
    }

    CurlGlobal(const CurlGlobal&) = delete;
    CurlGlobal& operator = (const CurlGlobal&) = delete;
};

class CurlEasy{
private:
    CURL* curl;

public:
    CurlEasy(){
        curl = curl_easy_init();
        if(!curl){
            throw std::runtime_error("Failed to easy initialize libcurl handler");
        }
    }
    ~CurlEasy(){
        if(curl){
            curl_easy_cleanup(curl);
        }
    }
    CURL* get() const{
        return curl;
    }

    CurlEasy(const CurlEasy&) = delete;
    CurlEasy& operator = (const CurlEasy&) = delete;
};

class InspectComand{
private:    
    CLI::App* cliChild;
    std::string file;
    void execute(){
        auto status = inspectSchema(file);

        if(!status.ok()){
            spdlog::error("{}", status.ToString());
        }
    }

public:
    explicit InspectComand(CLI::App* cliParent){
        cliChild = cliParent->add_subcommand("inspect", "Shortcut to inspect a file structure");
        cliChild->add_option("path", file, "Path to input Parquet file for inspect")
            ->check(CLI::ExistingFile);
        
        cliChild->callback([this]{
            this->execute();
        });
    }
};

struct configYaml{
    std::string datasetName;
    std::vector<int> years;
    std::vector<int> months;
    std::string outputDir;

    // Constructor to get the data from the YAML and put it into variables
    configYaml(const YAML::Node& configFile){
        datasetName = configFile["dataset"].as<std::string>();
        years = configFile["years"].as<std::vector<int>>();
        months = configFile["months"].as<std::vector<int>>();
        outputDir = configFile["output_dir"].as<std::string>();
    }
};

static std::string toLowercase(std::string&& text){
    std::transform(text.begin(), text.end(), text.begin(),
        [](unsigned char c){
            return std::tolower(c);
    });

    return std::move(text);
}

static std::string getTypeDataset(const std::string datasetName){
    // Function lambda to ignore uppercase and lowercase letters
    static const re2::RE2 patron = [](){
        RE2::Options options;
        options.set_case_sensitive(false);
        return re2::RE2("(fhvhv|fhv|yellow|green)", options);
    }();

    // Search for one of the previously stated words
    if(std::string carType; RE2::PartialMatch(datasetName, patron, &carType)){
        return toLowercase(std::move(carType));
    } else{
        return "";
    }
}

struct configGlobal{
    static constexpr std::string_view baseUrl = "https://d37ci6vzurychx.cloudfront.net/trip-data";
    const std::string carType;
    const std::string outputDir;

    configGlobal(const configYaml& dataYAML) :
        carType(getTypeDataset(dataYAML.datasetName)),
        outputDir(dataYAML.outputDir)
    {}

    std::string getDownloadUrl(int year, int month) const{
        return fmt::format("{}/{}_tripdata_{}-{:02}.parquet", baseUrl, carType, year, month);
    }

    std::string getOutputPath(int year, int month) const{
        return fmt::format("{}/dataset_{}-{:02}.parquet", outputDir, year, month);
    }
};

static bool ensureOutputDir(const std::string outputDir){
    std::filesystem::path currentWorkingDir = std::filesystem::current_path();
    std::filesystem::path routeToDownload = currentWorkingDir / outputDir;
    std::error_code errorCode;

    std::filesystem::create_directories(routeToDownload, errorCode);
    if(std::filesystem::is_directory(routeToDownload) && !errorCode){
        return true;
    } else{
        spdlog::error("Folder failed to load because: {}", errorCode.message());
        return false;
    }
}

// Function to receive the network data and insert in the outputFile
static size_t writeDataCallback(void* ptr, size_t size, size_t numberMember, void* stream){
    size_t totalBytes = size * numberMember;
    std::ofstream* outputFile = static_cast<std::ofstream*>(stream);

    // Insert the binary data from the .parquet file into the outputFile
    outputFile->write(static_cast<const char*>(ptr), totalBytes);

    /*
    Returns totalBytes because if the amount is 0 it means a failure, 
    either due to lack of space or a download error
    */
    return totalBytes;
}

template <typename T>
[[nodiscard("Error: You need to put the function in an if statement")]]
static bool validateOrLog(const T& object, std::string_view message){
    if(!object){
        spdlog::error(message);
        return false;
    }

    return true;
}

template <typename T>
[[nodiscard("Error: You need to put the function in an if statement")]]
static bool isOpen(const T& object, std::string_view message){
    if(!object.is_open()){
        spdlog::error(message);
        return false;
    }

    return true;
}

static bool setDownload(const std::string& url, const std::string& output){
    CurlEasy curlEasyInit;
    CURL* curl = curlEasyInit.get();

    // Open the outputFile in binary form 
    std::ofstream outputFile(output, std::ios::binary);
    if(!isOpen(outputFile, "Failed to create or open local outputfile")){
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeDataCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outputFile);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

    // Starts the file transfer
    CURLcode resCode = curl_easy_perform(curl);
    outputFile.close();

    if(resCode != CURLE_OK){
        // If the download failed halfway through, delete the incomplete file to prevent corruption
        if(std::filesystem::exists(output)){
            std::filesystem::remove(output);
        }

        spdlog::error("Libcurl transfer failed: {}", curl_easy_strerror(resCode));
        return false;
    }

    return true;
}

template <typename InternalFunction>
[[nodiscard("ERROR: You need to put some action in the function")]]
//Function that creates two nested for loops with an internal action
static void iteration(const configYaml& dataYaml, InternalFunction action){
    for(auto& year : dataYaml.years){
        for(auto& month : dataYaml.months){
            action(year, month);
        }
    }
}

static bool existsFile(const std::string& file){
    if(std::filesystem::exists(file)){
        if(std::filesystem::file_size(file) > 0){
            spdlog::info("File already exists locally, skipping download");
            return true;
        } else{
            spdlog::warn("File {} is corrupted, re-downloading", file);
            std::filesystem::remove(file);
            return false;
        }
    } else{
        return false;
    }
}

static double calculateMegabytes(const std::string& outputPath){
    std::uintmax_t bytes = std::filesystem::file_size(outputPath);
    double megabytes = bytes / (1024.0 * 1024.0);

    return megabytes;
}

static bool readyToProcess(const std::string& outputPath, const std::string& url, bool exists){
    if(!exists){
        if(!setDownload(url, outputPath)){
            spdlog::error("The download failed for the period");
            return false;
        }
    }

    double megabytes = calculateMegabytes(outputPath);
    spdlog::info("File size: {:.2f} Mb\n", megabytes);
    return true;
}

static bool verifyDownload(const configYaml& dataYAML, const configGlobal& configGl){
    bool ready = true;

    iteration(dataYAML, [&](int year, int month){
        std::string outputPath = configGl.getOutputPath(year, month);
        std::string url = configGl.getDownloadUrl(year, month);
        bool exists = existsFile(configGl.getOutputPath(year, month));

        spdlog::info("link: {}", configGl.getDownloadUrl(year, month));
        ready = (!readyToProcess(outputPath, url, exists)) ? false : true;
    });

    return (!ready) ? false : true;
}

template <typename Func, typename... Args>
// allows an indefinite number of arguments 
auto timeLapse(Func&& function, Args&&... args){
    auto start = std::chrono::high_resolution_clock::now();
    /*
    Call the function, Forward the arguments exactly as they arrived
    And it stores the result of the function in the variable "result"
    */
    auto result = std::forward<Func>(function)(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();

    long long seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    return std::make_pair(seconds, result);
}

static nlohmann::ordered_json buildManifestJson(const configYaml& dataYAML, const configGlobal& configGl){
    nlohmann::ordered_json dataJson;
    auto [time, download] = timeLapse(verifyDownload, dataYAML, configGl);
    dataJson["download_time"] = std::to_string(time) + "s";
    dataJson["dataset"] = dataYAML.datasetName;
    dataJson["files"] = nlohmann::ordered_json::array();

    iteration(dataYAML, [&](int year, int month){
        dataJson["files"].push_back({
            {"year", year},
            {"month", month},
            {"path", configGl.getOutputPath(year, month)},
            {"source", "nyc_tlc"},
            {"status", download ? "Downloaded" : "Failed"}
        });
    });

    return dataJson;
}

static std::string generateTimestamp(const std::string& name){
    auto now = std::chrono::system_clock::now();
    time_t timeActual = std::chrono::system_clock::to_time_t(now);
    // Divide it into a format of year, month, day, etc
    tm* timePart = localtime(&timeActual);

    std::stringstream ss;
    ss<<std::put_time(timePart, "%Y%m%d%H%M%S");

    std::string timestamp = ss.str() + name;
    return timestamp;
}

static bool generateJsonFile(const configYaml& dataYAML, const configGlobal& configGl){
    try{
        const std::string manifest = "-manifest.json";
        std::string fileName = generateTimestamp(manifest);

        std::ofstream file(fileName);
        if(!isOpen(file, "Failed to open or create local output file")) return false; 

        auto jsonData = buildManifestJson(dataYAML, configGl);
        file<<std::setw(4)<<jsonData;
        return file.good();
    } catch(const nlohmann::json::exception& e){
        spdlog::error("Json error: {}", e.what());
        return false;
    }
}

static arrow::Status inspectSchema(const std::string& file){  
    arrow::MemoryPool* pool = arrow::default_memory_pool();
    std::shared_ptr<arrow::io::RandomAccessFile> input; 
    ARROW_ASSIGN_OR_RAISE(input, arrow::io::ReadableFile::Open(file));

    // Create a readerFile
    std::unique_ptr<parquet::arrow::FileReader> reader;
    ARROW_ASSIGN_OR_RAISE(reader, parquet::arrow::OpenFile(input, arrow::default_memory_pool()));

    // Get the schema
    std::shared_ptr<arrow::Schema> schema;
    ARROW_RETURN_NOT_OK(reader->GetSchema(&schema));

    // Read the metadata
    auto metadata = reader->parquet_reader()->metadata();

    spdlog::info("File: {}", file);
    spdlog::info("Num Columns: {}", metadata->num_columns());
    spdlog::info("Num Rows: {:L}", metadata->num_rows());
    spdlog::info("Num Row Groups: {}", metadata->num_row_groups());
    spdlog::info("Schema:\n{}", schema->ToString());

    return arrow::Status::OK();
}

int main(int argc, char **argv){
    std::locale::global(std::locale("en_US.UTF-8"));

    const std::string configPath = "dataset_config.yaml";
    if(!std::filesystem::exists(configPath)){
        spdlog::error("The configuration file: {} was not found", configPath);
        return 1;
    }

    CurlGlobal curlGlobalInit;

    CLI::App app{"EngineCabsNYC: Analysis engine for intelligent decision-making based on hard data"};

    try{
        YAML::Node configFile = YAML::LoadFile(configPath);
        configYaml dataYAML(configFile);
        configGlobal configGl(dataYAML);

        auto* downloadParquet = app.add_subcommand("download", "Download .parquet files");

        InspectComand cmdInspect(&app);

        CLI11_PARSE(app, argc, argv);

        if(*downloadParquet){
            spdlog::info("Type of dataset: {}\n", configGl.carType);

            if(!validateOrLog(ensureOutputDir(dataYAML.outputDir), "The output file could not be secured")){
                return 1;
            }

             spdlog::info("Manifest: {}", generateJsonFile(dataYAML, configGl) 
                ? "created" : "could not be created");
            
            return 0;
        }

        if(argc == 1){
            spdlog::info(app.help());
            return 0;
        }

    } catch(const YAML::Exception& e){
        spdlog::error("Error YAML: {}", e.what());
        return 1;
    }catch(const std::filesystem::filesystem_error& e){
        spdlog::error("Error system file: {}", e.what());
        return 1;
    } catch(const CLI::ParseError& e){
        return app.exit(e);
    }

    return 0;
}