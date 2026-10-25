#include "ProcessorsTrips/Processors/FareDistribution.hpp"
#include "Parquet/IdxColumnSchema.hpp"
#include "Parquet/ParquetReader.hpp"
#include <spdlog/spdlog.h>
#include <filesystem>
#include <chrono>

FareDistribution::FareDistribution(FareDistributionResult& statsResult_, RunStats& stats_)
	: fareDisResult(statsResult_), fareDisStats(stats_) 
{}

arrow::Status FareDistribution::process(int64_t batchSize, const std::string& file){
    ARROW_ASSIGN_OR_RAISE(const auto parquetContext, openParquetReader(file, batchSize));
    ARROW_ASSIGN_OR_RAISE(const int fareAmountColumn, getIdxColumn(
        parquetContext.schema, "fare_amount"));

    std::shared_ptr<arrow::RecordBatch> batch;
    std::shared_ptr<arrow::RecordBatchReader> batchReader;

    auto start = std::chrono::high_resolution_clock::now();
    for(int idxRowGroup = 0; idxRowGroup < parquetContext.reader->num_row_groups(); idxRowGroup++){
        ARROW_ASSIGN_OR_RAISE(batchReader,
            parquetContext.reader->GetRecordBatchReader({idxRowGroup}, {fareAmountColumn}));

        while(true){
            ARROW_RETURN_NOT_OK(batchReader->ReadNext(&batch));
            // Break the while loop when there are no more batches left
            if(!batch) break;
            fareDisStats.batchCounter++;
            fareDisStats.numRows += batch->num_rows();

            auto fareArray = std::static_pointer_cast<arrow::DoubleArray>(batch->column(0));
            for(int64_t i = 0; i < batch->num_rows(); i++){
                if(fareArray->IsNull(i)) continue;

                const double fare = fareArray->Value(i);
                if(fare < 0.0) continue;
                if(fareDisResult.validFares == 0){
                    fareDisResult.minFare = fare;
                    fareDisResult.maxFare = fare;
                } else{
                    if(fare < fareDisResult.minFare){
                        fareDisResult.minFare = fare;
                    } else if(fare > fareDisResult.maxFare){
                        fareDisResult.maxFare = fare;
                    }
                }

                fareDisResult.fareSum += fare;
                fareDisResult.validFares++;

                std::size_t bucketIndex = static_cast<std::size_t>(fare / bucketWidth);
                if(bucketIndex >= regularBucket) bucketIndex = regularBucket;

                buckets[bucketIndex]++;
                fares.push_back(fare);
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    fareDisStats.elapsedSeconds += std::chrono::duration<double>(end - start).count();
    fareDisStats.files++;
    fareDisStats.bytesProcessed += std::filesystem::file_size(file);
    fareDisStats.numBatch = batchSize;

    if(fareDisResult.validFares > 0){
        fareDisResult.averageFare =
            fareDisResult.fareSum / static_cast<double>(fareDisResult.validFares);
    }

    if(!fares.empty()){
        std::sort(fares.begin(), fares.end());
        fareDisResult.percentile25 = fares[fares.size() * 25 / 100];
        fareDisResult.percentile50 = fares[fares.size() * 50 / 100];
        fareDisResult.percentile75 = fares[fares.size() * 75 / 100];
        fareDisResult.percentile99 = fares[fares.size() * 99 / 100];
    }

    spdlog::info("MB/s: {}", fareDisStats.megabytesPerSecond());
    spdlog::info("Rows/s: {}", fareDisStats.rowsPerSecond());

    return arrow::Status::OK();
}