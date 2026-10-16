#include "Config/DatasetType.hpp"
#include <string_view>
#include <algorithm>
#include <array>
#include <cctype>

std::string getTypeDataset(const std::string& datasetName){
    std::string lowerCaseName = datasetName;

    // Lambda function to transform the datasetName in lower case
    std::transform(lowerCaseName.begin(), lowerCaseName.end(), lowerCaseName.begin(),
        [](unsigned char c){
            return static_cast<char>(std::tolower(c));
        }
    );

    static constexpr std::array<std::string_view, 4> types{"fhvhv", "fhv", "yellow", "green"};
    std::size_t bestPosition = std::string::npos;
    std::string_view matched;

    for(const auto type : types){
        const auto position = lowerCaseName.find(type);
        if(position == std::string::npos) continue;

        // If one type of dataset takes precedence over another
        if(position < bestPosition || (position == bestPosition && type.size() > matched.size())){
            bestPosition = position;
            matched = type;
        }
    }

    // If no word matches any dataset, it returns an empty string
    return std::string{matched};
}