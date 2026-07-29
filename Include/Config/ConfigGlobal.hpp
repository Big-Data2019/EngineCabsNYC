#pragma once
#include <string>
#include <string_view>
#include "Config/ConfigYaml.hpp"

struct configGlobal{
    static constexpr std::string_view baseUrl = "https://d37ci6vzurychx.cloudfront.net/trip-data";
    const std::string carType;
    const std::string outputDir;

    explicit configGlobal(const configYaml& dataYaml);

    std::string getDownloadUrl(int year, int month) const;
    std::string getOutputPath(int year, int month) const;
};