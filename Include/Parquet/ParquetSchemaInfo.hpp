#pragma once

#include <string>

struct SchemaInfo{
    std::string file;
    std::string schema;

    int64_t numColumns = 0;
    int64_t numRowGroups = 0;
    int64_t numRows = 0;
};