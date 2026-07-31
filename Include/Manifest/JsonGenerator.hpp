#pragma once
#include "Manifest/JsonBuilder.hpp"
#include <string>

class JsonGenerator{
public:
    void generateManifest(const std::string& fileName, const JsonBuilder& readyBuilder);
};