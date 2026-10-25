#include "ProcessorsTrips/Processors/RowCounter.hpp"
#include "Manifest/JsonGenerator.hpp"
#include "Manifest/JsonBuilder.hpp"
#include <filesystem>
#include <string>

void RowCounter::finish(const std::string& output){
    JsonBuilder data;

    data.newData("Files", rowCounterStats.files);
    data.newData("Batches", rowCounterStats.batchCounter);
    data.newData("Rows", rowCounterStats.numRows);
    data.newData("Input size", rowCounterStats.bytesProcessed);
    data.newData("Elapsed", rowCounterStats.elapsedSeconds);
    data.newData("Batch size", rowCounterStats.numBatch);
    data.newData("Rows/sec", rowCounterStats.rowsPerSecond());
    data.newData("Troughput", rowCounterStats.megabytesPerSecond());

    std::filesystem::create_directories(output);
    const std::filesystem::path fileName = std::filesystem::path(output) / "RowCounterManifest.json";

    JsonGenerator generator;
    generator.generateManifest(fileName.string(), data);
}