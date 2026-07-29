#include "Config/DatasetType.hpp"
#include <algorithm>
#include <cctype>
#include <re2/re2.h>

namespace{
std::string toLowercase(std::string&& text){
    std::transform(text.begin(), text.end(), text.begin(),
        [](unsigned char(c)){
            return std::tolower(c);
    });

    return std::move(text);
}
}

std::string getTypeDataset(const std::string& datasetName){
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