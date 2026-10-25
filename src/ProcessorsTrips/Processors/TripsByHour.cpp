#include "ProcessorsTrips/Processors/TripsByHour.hpp"
#include "Parquet/IdxColumnSchema.hpp"
#include "Parquet/ParquetReader.hpp"
#include <spdlog/spdlog.h>
#include <filesystem>
#include <chrono>

TripsByHour::TripsByHour(RunStats& stats_)
    : tripsHourStats(stats_)
{}

arrow::Status TripsByHour::process(int64_t batchSize, const std::string& file){
    ARROW_ASSIGN_OR_RAISE(const auto parquetContext, openParquetReader(file, batchSize));
    ARROW_ASSIGN_OR_RAISE(const int dateColumn, getIdxColumn(
        parquetContext.schema, "tpep_pickup_datetime"));

    std::shared_ptr<arrow::RecordBatch> batch;
    std::shared_ptr<arrow::RecordBatchReader> batchReader;

    auto start = std::chrono::high_resolution_clock::now();
    for(int idxRowGroup = 0; idxRowGroup < parquetContext.reader->num_row_groups(); idxRowGroup++){
        ARROW_ASSIGN_OR_RAISE(batchReader,
            parquetContext.reader->GetRecordBatchReader({idxRowGroup}, {dateColumn}));

        while(true){
            ARROW_RETURN_NOT_OK(batchReader->ReadNext(&batch));
            // Break the while loop when there are no more batches left
            if(!batch) break;
            tripsHourStats.batchCounter++;
            tripsHourStats.numRows += batch->num_rows();

            auto dateArray = std::static_pointer_cast<arrow::TimestampArray>(batch->column(0));
            for(int64_t i = 0; i < batch->num_rows(); i++){
                if(dateArray->IsNull(i)) continue;

                int64_t timestamp = dateArray->Value(i); // Get the value in format Timestamp
                int64_t hourId = timestamp / 3600000000; // The value is in UNIX Epoch                  

                tripsByHour[hourId]++;
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    tripsHourStats.elapsedSeconds += std::chrono::duration<double>(end - start).count();
    tripsHourStats.files++;
    tripsHourStats.bytesProcessed += std::filesystem::file_size(file);
    tripsHourStats.numBatch = batchSize;

    spdlog::info("MB/s: {}", tripsHourStats.megabytesPerSecond());
    spdlog::info("Rows/s: {}", tripsHourStats.rowsPerSecond());

    return arrow::Status::OK();
}