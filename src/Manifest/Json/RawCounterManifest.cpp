#include "Manifest/JsonBuilder.hpp"
#include "Manifest/JsonGenerator.hpp"
#include "ProcessorsTrips/Processors/RawCounter.hpp"
#include <string>

void RawCounter::finish(const std::string& output){
    JsonBuilder data;

    data.newData("Files", stats.files);
    data.newData("Batches", stats.batchCounter);
    data.newData("Rows", stats.numRows);
    data.newData("Input size", stats.bytesProcessed);
    data.newData("Elapsed", stats.elapsedSeconds);
    data.newData("Batch size", stats.numBatch);
    data.newData("Rows/sec", stats.rowsPerSecond());
    data.newData("Troughput", stats.megabytesPerSecond());

    const std::string fileName = "manifest.json";

    JsonGenerator generator;
    generator.generateManifest(fileName, data);
}