#include "Processors/RawCounter.hpp"
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>
#include <arrow/record_batch.h>
#include <vector>
#include <spdlog/spdlog.h>

RawCounter::RawCounter(const Config& _config)
    : config(_config)
{}

arrow::Status RawCounter::process(){
    std::shared_ptr<arrow::io::ReadableFile> input;
    ARROW_ASSIGN_OR_RAISE(input, arrow::io::ReadableFile::Open(config.outputPath(2024, 01)));

    parquet::ArrowReaderProperties readerProperties;
    readerProperties.set_batch_size(65536);

    // Builder to create the properties
    parquet::arrow::FileReaderBuilder readerBuilder;
    ARROW_RETURN_NOT_OK(readerBuilder.Open(input));
    readerBuilder.properties(readerProperties);

    std::unique_ptr<parquet::arrow::FileReader> arrowReader;
    ARROW_ASSIGN_OR_RAISE(arrowReader, readerBuilder.Build());

    int idxColumn = 0;
    int batchCounter = 0;
    
    for(int idxRowGroup = 0; idxRowGroup < arrowReader->num_row_groups(); idxRowGroup++){
        std::shared_ptr<arrow::RecordBatch> batch;
        std::shared_ptr<arrow::RecordBatchReader> batchReader;

        ARROW_ASSIGN_OR_RAISE(batchReader,
            arrowReader->GetRecordBatchReader({idxRowGroup}, {idxColumn})
        );

        while(true){
            ARROW_RETURN_NOT_OK(batchReader->ReadNext(&batch));
            if(!batch) break;

            spdlog::info("Info: {}", batch->num_rows());

            numRows += batch->num_rows();
            batchCounter++;
        }
    }

    return arrow::Status::OK();
}