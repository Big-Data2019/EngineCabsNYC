#include "Parquet/ValidateParquet.hpp"
#include "Parquet/ParquetReader.hpp"
#include "Parquet/IdxColumnSchema.hpp"
#include <spdlog/spdlog.h>
#include <arrow/io/api.h>
#include <vector>
#include <utility>
#include <algorithm>

void printTopErrors(const InvalidInfo& info){
    std::vector<std::pair<std::string, std::int64_t>> errors = {
        {"Trip duration", info.tripDurationInvalids},
        {"Trip distance", info.tripDistanceInvalids},
        {"Fare amount", info.fareAmountInvalids},
        {"Total amount", info.totalAmountInvalids},
        {"Locations ID", info.locationIdInvalids},
        {"Passenger count", info.passengerCountInvalids},
        {"Payment type", info.paymentTypeInvalids}
    };

    std::sort(errors.begin(), errors.end(), [](const auto& a, const auto& b){
        return a.second > b.second;
    });

    for(std::size_t i = 0; i < 5; i++){
        spdlog::info("{}. {}: {}", i + 1, errors[i].first, errors[i].second);
    }
}

arrow::Result<InvalidInfo> validateParquetData(const std::string& file){
    ARROW_ASSIGN_OR_RAISE(const auto parquetContext, openParquetReader(file));

    ARROW_ASSIGN_OR_RAISE(const int pickUpColumn, getIdxColumn(
        parquetContext.schema, "tpep_pickup_datetime"));
    ARROW_ASSIGN_OR_RAISE(const int dropOffColumn, getIdxColumn(
        parquetContext.schema, "tpep_dropoff_datetime"));
    ARROW_ASSIGN_OR_RAISE(const int tripDisColumn, getIdxColumn(
        parquetContext.schema, "trip_distance"));
    ARROW_ASSIGN_OR_RAISE(const int fareAmountColumn, getIdxColumn(
        parquetContext.schema, "fare_amount"));
    ARROW_ASSIGN_OR_RAISE(const int totalAmountColumn, getIdxColumn(
        parquetContext.schema, "total_amount"));
    ARROW_ASSIGN_OR_RAISE(const int PULocationIdColumn, getIdxColumn(
        parquetContext.schema, "PULocationID"));
    ARROW_ASSIGN_OR_RAISE(const int DOLocationIdColumn, getIdxColumn(
        parquetContext.schema, "DOLocationID"));
    ARROW_ASSIGN_OR_RAISE(const int passengerCountColumn, getIdxColumn(
        parquetContext.schema, "passenger_count"));
    ARROW_ASSIGN_OR_RAISE(const int paymentTypeColumn, getIdxColumn(
        parquetContext.schema, "payment_type"));

    InvalidInfo info;
    std::shared_ptr<arrow::RecordBatch> batch;
    std::shared_ptr<arrow::RecordBatchReader> batchReader;

    for(int idxRowGroup = 0; idxRowGroup < parquetContext.reader->num_row_groups(); idxRowGroup++){
        ARROW_ASSIGN_OR_RAISE(batchReader,
            parquetContext.reader->GetRecordBatchReader({idxRowGroup}, {
                pickUpColumn, dropOffColumn, tripDisColumn, 
                fareAmountColumn, totalAmountColumn, PULocationIdColumn,
                DOLocationIdColumn, passengerCountColumn, paymentTypeColumn
            }));

        while(true){
            ARROW_RETURN_NOT_OK(batchReader->ReadNext(&batch));
            // Break the while loop when there are no more batches left
            if(!batch) break;

            info.totalRows += batch->num_rows();

            auto pickUp = std::static_pointer_cast<arrow::TimestampArray>(batch->column(0));
            auto dropOff = std::static_pointer_cast<arrow::TimestampArray>(batch->column(1));
            auto tripDistance = std::static_pointer_cast<arrow::DoubleArray>(batch->column(2));
            auto fareAmount = std::static_pointer_cast<arrow::DoubleArray>(batch->column(3));
            auto totalAmount = std::static_pointer_cast<arrow::DoubleArray>(batch->column(4));
            auto PULocationId = std::static_pointer_cast<arrow::Int32Array>(batch->column(5));
            auto DOLocationId = std::static_pointer_cast<arrow::Int32Array>(batch->column(6));
            auto passengerCount = std::static_pointer_cast<arrow::Int32Array>(batch->column(7));
            auto paymentType = std::static_pointer_cast<arrow::Int64Array>(batch->column(8));

            for(std::int64_t i = 0; i < batch->num_rows(); i++){
                bool invalidRow = false;

                const bool nullPickUp = pickUp->IsNull(i);
                const bool nullDropOff = dropOff->IsNull(i);
                const bool nullTripDistance = tripDistance->IsNull(i);
                const bool nullFareAmount = fareAmount->IsNull(i);
                const bool nullTotalAmount = totalAmount->IsNull(i);
                const bool nullPULocationId = PULocationId->IsNull(i);
                const bool nullDOLocationId = DOLocationId->IsNull(i);
                const bool nullPassengerCount = passengerCount->IsNull(i);
                const bool nullPaymentType = paymentType->IsNull(i);

                const int nullCount =
                    static_cast<int>(nullPickUp) +
                    static_cast<int>(nullDropOff) +
                    static_cast<int>(nullTripDistance) +
                    static_cast<int>(nullFareAmount) +
                    static_cast<int>(nullTotalAmount) +
                    static_cast<int>(nullPULocationId) +
                    static_cast<int>(nullDOLocationId) +
                    static_cast<int>(nullPassengerCount) +
                    static_cast<int>(nullPaymentType);

                if(nullCount > 0){
                    info.nullValues += nullCount;
                    invalidRow = true;
                }

                if(!nullPickUp && !nullDropOff){
                    if(dropOff->Value(i) <= pickUp->Value(i)){
                        info.tripDurationInvalids++;
                        invalidRow = true;
                    }
                }

                if(!nullTripDistance){
                    if(tripDistance->Value(i) < 0){
                        info.tripDistanceInvalids++;
                        invalidRow = true;
                    }
                }

                if(!nullFareAmount){
                    if(fareAmount->Value(i) < 0){
                        info.fareAmountInvalids++;
                        invalidRow = true;
                    }
                }

                if(!nullTotalAmount){
                    if(totalAmount->Value(i) < 0){
                        info.totalAmountInvalids++;
                        invalidRow = true;
                    }
                }

                if(!nullPULocationId && PULocationId->Value(i) <= 0 ||
                (!nullDOLocationId) && DOLocationId->Value(i) <= 0){
                    info.locationIdInvalids++;
                    invalidRow = true;
                }

                if(!nullPassengerCount){
                    if(passengerCount->Value(i) <= 0 || passengerCount->Value(i) >= 5){
                        info.passengerCountInvalids++;
                        invalidRow = true;
                    }
                }

                if(!nullPaymentType){
                    if(paymentType->Value(i) < 0 || paymentType->Value(i) > 6){
                        info.paymentTypeInvalids++;
                        invalidRow = true;
                    }
                }

                if(invalidRow){
                    info.invalidRows++;
                } else{
                    info.validRows++;
                }
            }
        }
    }

    spdlog::info("Rows processed: {}", info.totalRows);
    spdlog::info("Valid Rows: {}", info.validRows);
    spdlog::info("Invalid Rows: {}", info.invalidRows);
    spdlog::info("Null Values: {}", info.nullValues);

    spdlog::info("--Top Errors--");
    printTopErrors(info);

    return info;
}