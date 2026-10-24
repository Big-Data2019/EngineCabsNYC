#include "Parquet/IdxColumnSchema.hpp"
#include <arrow/status.h>

arrow::Result<int>
getIdxColumn(const std::shared_ptr<arrow::Schema>& schema, const std::string& nameColumn){
    if(!schema){
        return arrow::Status::Invalid("The schema is null");
    }

    const int idx = schema->GetFieldIndex(nameColumn); // Get the index based on the name of column
    if(idx == -1){
        return arrow::Status::Invalid("The required column was not found");
    }

    return idx;
}