#include "ProcessorsTrips/Processors/RevenueByDay.hpp"
#include <arrow/io/api.h>
#include <arrow/api.h>
#include <parquet/arrow/reader.h>
#include <chrono>
#include <filesystem>
#include <spdlog/spdlog.h>

RevenueByDay::RevenueByDay(RevenueDayStats& stats_)
    : revenueDStats(stats_)
{}

arrow::Status RevenueByDay::process(const int64_t& batchSize,const std::vector<std::string>& files)
{
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

        std::shared_ptr<arrow::Schema> schema;
        ARROW_RETURN_NOT_OK(arrowReader->GetSchema(&schema));

        const int dateColumn = schema->GetFieldIndex("tpep_pickup_datetime");
        const int totalAmountColumn = schema->GetFieldIndex("total_amount");
        if(dateColumn == -1 || totalAmountColumn == -1){
            return arrow::Status::Invalid("The required column was not found");
        }

        std::shared_ptr<arrow::RecordBatch> batch;
        std::shared_ptr<arrow::RecordBatchReader> batchReader;

        auto start = std::chrono::high_resolution_clock::now();
        for(int idxRowGroup = 0; idxRowGroup < arrowReader->num_row_groups(); idxRowGroup++){
            ARROW_ASSIGN_OR_RAISE(batchReader,
                arrowReader->GetRecordBatchReader({idxRowGroup}, {dateColumn, totalAmountColumn}));

            while(true){
                ARROW_RETURN_NOT_OK(batchReader->ReadNext(&batch));
                // Break the while loop when there are no more batches left
                if(!batch) break;
                revenueDStats.batchCounter++;

                auto dateArray = std::static_pointer_cast<arrow::TimestampArray>(batch->column(0));
                for(int64_t i = 0; i < batch->num_rows(); i++){
                    if(dateArray->IsNull(i)){
                        continue;
                    }

                    int64_t timestamp = dateArray->Value(i);
                    int diaId = timestamp / 86400000000;
                }
            }
        }
        auto end = std::chrono::high_resolution_clock::now();

        revenueDStats.elapsedSeconds += std::chrono::duration_cast<std::chrono::seconds>
            (end - start).count();
        revenueDStats.files++;
        auto size = std::filesystem::file_size(file);
        revenueDStats.bytesProcessed += size;
    }
    
    revenueDStats.numBatch = batchSize;

    spdlog::info("MB/s: {}", revenueDStats.megabytesPerSecond());
    spdlog::info("Rows/s: {}", revenueDStats.quantityPerSecond());

    return arrow::Status::OK();
}

void RevenueByDay::finish(const std::string& output){
    
}