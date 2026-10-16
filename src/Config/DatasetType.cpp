#include "Config/DatasetType.hpp"
#include <re2/re2.h>
#include <algorithm>
#include <cctype>

std::string getTypeDataset(const std::string& datasetName){
    std::string datasetType;

    // Lambda function to set the searcher with the type of datasets
    static const re2::RE2 pattern = [](){
        RE2::Options options;
        options.set_case_sensitive(false); // To ignore uppercase and lowercase letters
        return re2::RE2("(fhvhv|fhv|yellow|green)", options); // Dataset types
    }();

    // If no word matches any dataset, it returns an empty string
    if(!re2::RE2::PartialMatch(datasetName, pattern, &datasetType)){
        return {};
    }

    // Lambda function to transform the datasetType in lower case
    std::transform(datasetType.begin(), datasetType.end(), datasetType.begin(),
        [](unsigned char c){
            return static_cast<char>(std::tolower(c));
        }
    );

    return datasetType;
}