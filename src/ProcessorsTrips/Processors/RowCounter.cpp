#include "ProcessorsTrips/Processors/RowCounter.hpp"
#include "Parquet/ParquetReader.hpp"
#include <arrow/record_batch.h>
#include <spdlog/spdlog.h>
#include <filesystem>
#include <chrono>

RowCounter::RowCounter(RunStats& stats_)
    : rowCounterStats(stats_)
{}

arrow::Status RowCounter::process(int64_t batchSize, const std::string& file){
    ARROW_ASSIGN_OR_RAISE(const auto parquetContext, openParquetReader(file, batchSize));

    int idxColumn = 0;
    std::shared_ptr<arrow::RecordBatch> batch;
    std::shared_ptr<arrow::RecordBatchReader> batchReader;

    auto start = std::chrono::high_resolution_clock::now();

    for(int idxRowGroup = 0; idxRowGroup < parquetContext.reader->num_row_groups(); idxRowGroup++){
        ARROW_ASSIGN_OR_RAISE(batchReader,
            parquetContext.reader->GetRecordBatchReader({idxRowGroup}, {idxColumn}));

        while(true){
            ARROW_RETURN_NOT_OK(batchReader->ReadNext(&batch));
            // Break the while loop when there are no more batches left
            if(!batch) break;

            rowCounterStats.batchCounter++;
            rowCounterStats.numRows += batch->num_rows();
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    rowCounterStats.elapsedSeconds += std::chrono::duration<double>(end - start).count();
    rowCounterStats.files++;
    auto size = std::filesystem::file_size(file);
    rowCounterStats.bytesProcessed += size;
    rowCounterStats.numBatch = batchSize;

    spdlog::info("MB/s: {}", rowCounterStats.megabytesPerSecond());
    spdlog::info("Rows/s: {}", rowCounterStats.rowsPerSecond());

    return arrow::Status::OK();
}