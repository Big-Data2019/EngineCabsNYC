#pragma once
#include <nlohmann/json.hpp>
#include <vector>
#include <string>

class JsonBuilder{
private:
    nlohmann::ordered_json jsonOrder;

public:
    template <typename T>
    void newData(const std::string& data, const T& value){
        jsonOrder[data] = value;
    }

    void manyNewData(
        const std::string& data, const std::vector<nlohmann::ordered_json>& valueList
    );

    nlohmann::ordered_json build() const{
        return jsonOrder;
    }
};