#include "Manifest/JsonBuilder.hpp"

void JsonBuilder::manyNewData(
    const std::string& data, const std::vector<nlohmann::ordered_json>& valueList){
    if(!jsonOrder.contains(data)){
        jsonOrder[data] = nlohmann::ordered_json::array();
    }

    for(const auto& element : valueList){
        jsonOrder[data].push_back(element);
    }
}