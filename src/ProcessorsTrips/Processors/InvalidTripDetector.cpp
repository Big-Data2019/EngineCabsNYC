#include "ProcessorsTrips/Processors/InvalidTripDetector.hpp"
#include "Parquet/IdxColumnSchema.hpp"
#include "Parquet/ParquetReader.hpp"
#include <spdlog/spdlog.h>
#include <chrono>
#include <filesystem>

InvalidTripDetector::InvalidTripDetector(InvalidTripStats& stats_)
    : invalidTStats(stats_)
{}

arrow::Status InvalidTripDetector::process(const int64_t& batchSize,
    const std::vector<std::string>& files)
{
    for(const auto& file : files){
        ARROW_ASSIGN_OR_RAISE(auto parquetContext, openParquetReader(file, batchSize));
        ARROW_ASSIGN_OR_RAISE(const int pickupTimeColumn, getIdxColumn(
            parquetContext.schema, "tpep_pickup_datetime"));
        ARROW_ASSIGN_OR_RAISE(const int dropoffTimeColumn, getIdxColumn(
            parquetContext.schema, "tpep_dropoff_datetime"));
        ARROW_ASSIGN_OR_RAISE(const int tripDistanceColumn, getIdxColumn(
            parquetContext.schema, "trip_distance"));
        ARROW_ASSIGN_OR_RAISE(const int totalAmountColumn, getIdxColumn(
            parquetContext.schema, "total_amount"));

        std::shared_ptr<arrow::RecordBatch> batch;
        std::shared_ptr<arrow::RecordBatchReader> batchReader;

        auto start = std::chrono::high_resolution_clock::now();
        for(int idxRowGroup = 0; idxRowGroup < parquetContext.reader->num_row_groups();
        idxRowGroup++){
            ARROW_ASSIGN_OR_RAISE(batchReader,
                parquetContext.reader->GetRecordBatchReader({idxRowGroup}, {
                    pickupTimeColumn, dropoffTimeColumn,
                    tripDistanceColumn, totalAmountColumn
                }));

            while(true){
                ARROW_RETURN_NOT_OK(batchReader->ReadNext(&batch));
                // Break the while loop when there are no more batches left
                if(!batch) break;

                invalidTStats.batchCounter++;
                invalidTStats.numRows += batch->num_rows();
                auto pickUpArray =
                    std::static_pointer_cast<arrow::TimestampArray>(batch->column(0));
                auto dropOffArray =
                    std::static_pointer_cast<arrow::TimestampArray>(batch->column(1));
                auto tripDistance =
                    std::static_pointer_cast<arrow::DoubleArray>(batch->column(2));
                auto totalAmount =
                    std::static_pointer_cast<arrow::DoubleArray>(batch->column(3));

                for(int64_t i = 0; i < batch->num_rows(); i++){
                    if(pickUpArray->IsNull(i) || dropOffArray->IsNull(i) ||
                     tripDistance->IsNull(i) || totalAmount->IsNull(i)){
                        invalidTStats.invalidTrips++;
                        continue;
                    }

                    const int64_t pickUp = pickUpArray->Value(i);
                    const int64_t dropOff = dropOffArray->Value(i);
                    const double distance = tripDistance->Value(i);
                    const double amount = totalAmount->Value(i);

                    if(dropOff <= pickUp) invalidTStats.invalidTrips++;
                    else if(distance < 0 || amount < 0) invalidTStats.invalidTrips++;
                    else invalidTStats.validTrips++;
                }
            }
        }
        auto end = std::chrono::high_resolution_clock::now();

        invalidTStats.elapsedSeconds += std::chrono::duration<double>(end-start).count();
        invalidTStats.files++;
        auto size = std::filesystem::file_size(file);
        invalidTStats.bytesProcessed += size;
    }
    invalidTStats.numBatch = batchSize;

    spdlog::info("MB/s: {}", invalidTStats.megabytesPerSecond());
    spdlog::info("Rows/s: {}", invalidTStats.rowsPerSecond());

    return arrow::Status::OK();
}