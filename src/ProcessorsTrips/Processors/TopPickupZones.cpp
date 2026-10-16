#include "ProcessorsTrips/Processors/TopPickupZones.hpp"
#include "Parquet/IdxColumnSchema.hpp"
#include <spdlog/spdlog.h>
#include <arrow/io/api.h>
#include <arrow/api.h>
#include <chrono>
#include <filesystem>
#include <parquet/arrow/reader.h>

TopPickupZones::TopPickupZones(PickupZoneStats& stats_)
	: pickupZStats(stats_)	
{}

arrow::Status TopPickupZones::process(const int64_t& batchSize,
	const std::vector<std::string>& files)
{
	for(const auto& file : files){
		std::shared_ptr<arrow::io::ReadableFile> input;
		ARROW_ASSIGN_OR_RAISE(input, arrow::io::ReadableFile::Open(file));

		parquet::ArrowReaderProperties readerProperties;
		readerProperties.set_batch_size(batchSize);

		parquet::arrow::FileReaderBuilder readerBuilder;
		readerBuilder.properties(readerProperties);

		ARROW_RETURN_NOT_OK(readerBuilder.Open(input));

		std::unique_ptr<parquet::arrow::FileReader> arrowReader;
		ARROW_ASSIGN_OR_RAISE(arrowReader, readerBuilder.Build());

        ARROW_ASSIGN_OR_RAISE(const int puLocationId, getIdxColumn(file, "PULocationID"));

		std::shared_ptr<arrow::RecordBatch> batch;
		std::shared_ptr<arrow::RecordBatchReader> batchReader;

		auto start = std::chrono::high_resolution_clock::now();
		for(int idxRowGroup = 0; idxRowGroup < arrowReader->num_row_groups(); idxRowGroup++){
			ARROW_ASSIGN_OR_RAISE(batchReader,
				arrowReader->GetRecordBatchReader({idxRowGroup}, {puLocationId}));

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
	}
	pickupZStats.numBatch = batchSize;

    spdlog::info("MB/s: {}", pickupZStats.megabytesPerSecond());
	spdlog::info("Rows/s: {}", pickupZStats.rowsPerSecond());

	return arrow::Status::OK();
}