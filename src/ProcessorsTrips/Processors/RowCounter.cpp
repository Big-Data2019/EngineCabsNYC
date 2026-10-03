#include "ProcessorsTrips/Processors/RowCounter.hpp"
#include <arrow/io/api.h>
#include <arrow/api.h>
#include <parquet/arrow/reader.h>
#include <spdlog/spdlog.h>
#include <chrono>
#include <filesystem>

RowCounter::RowCounter(RowStats& stats_)
    : rowStats(stats_)
{}

arrow::Status RowCounter::process(const int64_t& batchSize, const std::vector<std::string>& files){
    for(const auto& file : files){
        std::shared_ptr<arrow::io::ReadableFile> input;
        ARROW_ASSIGN_OR_RAISE(input, arrow::io::ReadableFile::Open(file));

        parquet::ArrowReaderProperties readerProperties;
        readerProperties.set_batch_size(batchSize);

        // Builder to create the properties
        parquet::arrow::FileReaderBuilder readerBuilder;
        readerBuilder.properties(readerProperties);

        ARROW_RETURN_NOT_OK(readerBuilder.Open(input));

        std::unique_ptr<parquet::arrow::FileReader> arrowReader;
        ARROW_ASSIGN_OR_RAISE(arrowReader, readerBuilder.Build());

        int idxColumn = 0;
        auto start = std::chrono::high_resolution_clock::now();

        for(int idxRowGroup = 0; idxRowGroup < arrowReader->num_row_groups(); idxRowGroup++){
            std::shared_ptr<arrow::RecordBatch> batch;
            std::shared_ptr<arrow::RecordBatchReader> batchReader;

            ARROW_ASSIGN_OR_RAISE(batchReader,
            arrowReader->GetRecordBatchReader({idxRowGroup}, {idxColumn}));

            while(true){
                ARROW_RETURN_NOT_OK(batchReader->ReadNext(&batch));
                // Break the while loop when there are no more batches left
                if(!batch) break;

                rowStats.batchCounter++;
                rowStats.numRows += batch->num_rows();
            }
        }

        auto end = std::chrono::high_resolution_clock::now();

        rowStats.elapsedSeconds += std::chrono::duration_cast<std::chrono::seconds>(end - start)
            .count();
        rowStats.files++;
        auto size = std::filesystem::file_size(file);
        rowStats.bytesProcessed += size;
    }
    
    rowStats.numBatch = batchSize;

    spdlog::info("MB/s: {}", rowStats.megabytesPerSecond());
    spdlog::info("Rows/s: {}", rowStats.rowsPerSecond());

    return arrow::Status::OK();
}