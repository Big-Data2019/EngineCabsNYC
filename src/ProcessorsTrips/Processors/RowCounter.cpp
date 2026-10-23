#include "ProcessorsTrips/Processors/RowCounter.hpp"
#include "Parquet/ParquetReader.hpp"
#include <arrow/record_batch.h>
#include <spdlog/spdlog.h>
#include <filesystem>
#include <chrono>

RowCounter::RowCounter(RowStats& stats_)
    : rowStats(stats_)
{}

arrow::Status RowCounter::process(int64_t batchSize, const std::string& file){
    ARROW_ASSIGN_OR_RAISE(auto parquetContext, openParquetReader(file, batchSize));

    int idxColumn = 0;
    auto start = std::chrono::high_resolution_clock::now();

    for(int idxRowGroup = 0; idxRowGroup < parquetContext.reader->num_row_groups();
    idxRowGroup++){
        std::shared_ptr<arrow::RecordBatch> batch;
        std::shared_ptr<arrow::RecordBatchReader> batchReader;

        ARROW_ASSIGN_OR_RAISE(batchReader,
            parquetContext.reader->GetRecordBatchReader({idxRowGroup}, {idxColumn}));

        while(true){
            ARROW_RETURN_NOT_OK(batchReader->ReadNext(&batch));
            // Break the while loop when there are no more batches left
            if(!batch) break;

            rowStats.batchCounter++;
            rowStats.numRows += batch->num_rows();
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    rowStats.elapsedSeconds += std::chrono::duration<double>(end - start).count();
    rowStats.files++;
    auto size = std::filesystem::file_size(file);
    rowStats.bytesProcessed += size;
    rowStats.numBatch = batchSize;

    spdlog::info("MB/s: {}", rowStats.megabytesPerSecond());
    spdlog::info("Rows/s: {}", rowStats.rowsPerSecond());

    return arrow::Status::OK();
}