#include "ProcessorsTrips/Processors/RowCounter.hpp"
#include "Manifest/JsonGenerator.hpp"
#include "Manifest/JsonBuilder.hpp"
#include <filesystem>
#include <string>

void RowCounter::finish(const std::string& output){
    JsonBuilder data;

    data.newData("Files", rowStats.files);
    data.newData("Batches", rowStats.batchCounter);
    data.newData("Rows", rowStats.numRows);
    data.newData("Input size", rowStats.bytesProcessed);
    data.newData("Elapsed", rowStats.elapsedSeconds);
    data.newData("Batch size", rowStats.numBatch);
    data.newData("Rows/sec", rowStats.rowsPerSecond());
    data.newData("Troughput", rowStats.megabytesPerSecond());

    std::filesystem::create_directories(output);
    const std::filesystem::path fileName = std::filesystem::path(output) / "RowCounterManifest.json";

    JsonGenerator generator;
    generator.generateManifest(fileName.string(), data);
}