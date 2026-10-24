#include "ProcessorsTrips/Processors/TopPickupZones.hpp"
#include "Parquet/IdxColumnSchema.hpp"
#include "Parquet/ParquetReader.hpp"
#include <spdlog/spdlog.h>
#include <filesystem>
#include <chrono>

TopPickupZones::TopPickupZones(PickupZoneStats& stats_)
	: pickupZStats(stats_)	
{}

arrow::Status TopPickupZones::process(int64_t batchSize, const std::string& file){
    ARROW_ASSIGN_OR_RAISE(const auto parquetContext, openParquetReader(file, batchSize));
    ARROW_ASSIGN_OR_RAISE(int puLocationId, getIdxColumn(
		parquetContext.schema, "PULocationID"));

	std::shared_ptr<arrow::RecordBatch> batch;
	std::shared_ptr<arrow::RecordBatchReader> batchReader;

	auto start = std::chrono::high_resolution_clock::now();
	for(int idxRowGroup = 0; idxRowGroup < parquetContext.reader->num_row_groups(); idxRowGroup++){
		ARROW_ASSIGN_OR_RAISE(batchReader,
			parquetContext.reader->GetRecordBatchReader({idxRowGroup}, {puLocationId}));

		while(true){
			ARROW_RETURN_NOT_OK(batchReader->ReadNext(&batch));
			// Break the while loop when there are no more batches left
			if(!batch) break;
			pickupZStats.batchCounter++;
			pickupZStats.numRows += batch->num_rows();

			auto zoneArray = std::static_pointer_cast<arrow::Int32Array>(batch->column(0));
			for(int64_t i = 0; i < batch->num_rows(); i++){
				if(zoneArray->IsNull(i)) continue;

				int32_t zoneId = zoneArray->Value(i);
				pickUpZone[zoneId]++;
			}
		}
	}
	auto end = std::chrono::high_resolution_clock::now();

	pickupZStats.elapsedSeconds += std::chrono::duration<double>(end - start).count();
	pickupZStats.files++;
	auto size = std::filesystem::file_size(file);
	pickupZStats.bytesProcessed += size;
	pickupZStats.numBatch = batchSize;

    spdlog::info("MB/s: {}", pickupZStats.megabytesPerSecond());
	spdlog::info("Rows/s: {}", pickupZStats.rowsPerSecond());

	return arrow::Status::OK();
}