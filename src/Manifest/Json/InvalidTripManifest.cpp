#include "ProcessorsTrips/Processors/InvalidTripDetector.hpp"
#include "Manifest/JsonBuilder.hpp"
#include "Manifest/JsonGenerator.hpp"
#include <string>

void InvalidTripDetector::finish(const std::string& output){
    JsonBuilder data;

    data.newData("Files", invalidTStats.files);
    data.newData("Batches", invalidTStats.batchCounter);
    data.newData("Valid Trips", invalidTStats.validTrips);
    data.newData("Invalid Trips", invalidTStats.invalidTrips);
    data.newData("Input size", invalidTStats.bytesProcessed);
    data.newData("Elapsed", invalidTStats.elapsedSeconds);
    data.newData("Batch size", invalidTStats.numBatch);
    data.newData("Rows/sec", invalidTStats.rowsPerSecond());
    data.newData("Troughput", invalidTStats.megabytesPerSecond());

    std::filesystem::create_directories(output);
    const std::filesystem::path fileName = std::filesystem::path(output) / "InvalidTripsManifest.json";

    JsonGenerator generator;
    generator.generateManifest(fileName.string(), data);
}