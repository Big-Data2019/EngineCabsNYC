#pragma once
#include "Config/ConfigGlobal.hpp"
#include "Config/ConfigYaml.hpp"
#include <nlohmann/json.hpp>

nlohmann::ordered_json buildManifestJson(const configYaml& dataYAML, const configGlobal& configGl);