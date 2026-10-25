#include "ProcessorsTrips/Processors/RevenueByDay.hpp"
#include "Parquet/IdxColumnSchema.hpp"
#include "Parquet/ParquetReader.hpp"
#include <spdlog/spdlog.h>
#include <filesystem>
#include <chrono>

RevenueByDay::RevenueByDay(RunStats& stats_)
    : revenueDayStats(stats_)
{}

arrow::Status RevenueByDay::process(std::int64_t batchSize, const std::string& file){
    ARROW_ASSIGN_OR_RAISE(const auto parquetContext, openParquetReader(file, batchSize));
    ARROW_ASSIGN_OR_RAISE(const int dateColumn, getIdxColumn(
        parquetContext.schema, "tpep_pickup_datetime"));
    ARROW_ASSIGN_OR_RAISE(const int totalAmountColumn, getIdxColumn(
        parquetContext.schema, "total_amount"));

    std::shared_ptr<arrow::RecordBatch> batch;
    std::shared_ptr<arrow::RecordBatchReader> batchReader;

    auto start = std::chrono::high_resolution_clock::now();
    for(int idxRowGroup = 0; idxRowGroup < parquetContext.reader->num_row_groups(); idxRowGroup++){
        ARROW_ASSIGN_OR_RAISE(batchReader,
            parquetContext.reader->GetRecordBatchReader(
                {idxRowGroup}, {dateColumn, totalAmountColumn}));

        while(true){
            ARROW_RETURN_NOT_OK(batchReader->ReadNext(&batch));
            // Break the while loop when there are no more batches left
            if(!batch) break;
            revenueDayStats.batchCounter++;
            revenueDayStats.numRows += batch->num_rows();

            auto dateArray = std::static_pointer_cast<arrow::TimestampArray>(batch->column(0));
            auto amountArray = std::static_pointer_cast<arrow::DoubleArray>(batch->column(1));
            for(std::int64_t i = 0; i < batch->num_rows(); i++){
                if(dateArray->IsNull(i) || amountArray->IsNull(i)) continue;

                std::int64_t timestamp = dateArray->Value(i); // Get the value in format Timestamp
                std::int64_t dayId = timestamp / 86400000000; // The value is in UNIX Epoch
                double amount = amountArray->Value(i);

                Accumulator[dayId] += amount; // Add up the corresponding values each day
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    revenueDayStats.elapsedSeconds += std::chrono::duration<double>(end - start).count();
    revenueDayStats.files++;
    revenueDayStats.bytesProcessed += std::filesystem::file_size(file);
    revenueDayStats.numBatch = batchSize;

    spdlog::info("MB/s: {}", revenueDayStats.megabytesPerSecond());
    spdlog::info("Rows/s: {}", revenueDayStats.rowsPerSecond());

    return arrow::Status::OK();
}