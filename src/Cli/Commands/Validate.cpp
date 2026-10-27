#include "Cli/Commands/Validate.hpp"
#include "Utils/ExpanderFiles.hpp"
#include "Parquet/ValidateParquet.hpp"
#include "Manifest/Json/ValidateParquetManifest.hpp"
#include "Parquet/InvalidInfo.hpp"
#include <spdlog/spdlog.h>

inline void accumulate(InvalidInfo& totalInfo, const InvalidInfo& currentInfo){
    totalInfo.totalRows += currentInfo.totalRows;
    totalInfo.validRows += currentInfo.validRows;
    totalInfo.invalidRows += currentInfo.invalidRows;
    totalInfo.nullValues += currentInfo.nullValues;
    totalInfo.tripDistanceInvalids += currentInfo.tripDistanceInvalids;
    totalInfo.tripDurationInvalids += currentInfo.tripDurationInvalids;
    totalInfo.fareAmountInvalids += currentInfo.fareAmountInvalids;
    totalInfo.totalAmountInvalids += currentInfo.totalAmountInvalids;
    totalInfo.locationIdInvalids += currentInfo.locationIdInvalids;
    totalInfo.passengerCountInvalids += currentInfo.passengerCountInvalids;
    totalInfo.paymentTypeInvalids += currentInfo.paymentTypeInvalids;
}

void ValidateCommand::execute(){
    auto files = expandFiles(paths);

    InvalidInfo totalInfo;

    for(const auto& file : files){
        auto result = validateParquetData(file);
        if(!result.ok()){
            spdlog::error("Error of the status: {}", result.status().ToString());
            return;
        }

        accumulate(totalInfo, *result);
    }

    generateValidationManifest(totalInfo, output);
}

void ValidateCommand::registerCommand(CLI::App& app){
    auto* cmd = app.add_subcommand("validate", "Verify the integrity of the data");
    cmd->add_option("--path", paths, "Path to inputh the parquet files")
        ->required()
        ->check(CLI::ExistingPath);

    cmd->add_option("--output", output, "Where the stats will be saved");

    cmd->callback([this]{
        execute();
    });
}