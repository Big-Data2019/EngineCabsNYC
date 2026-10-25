#include "ProcessorsTrips/Processors/InvalidTripDetector.hpp"
#include "Manifest/JsonGenerator.hpp"
#include "Manifest/JsonBuilder.hpp"
#include <string>

void InvalidTripDetector::finish(const std::string& output){
    JsonBuilder data;

    data.newData("Files", invalidTripStats.files);
    data.newData("Batches", invalidTripStats.batchCounter);
    data.newData("Valid Trips", validTrips);
    data.newData("Invalid Trips", invalidTrips);
    data.newData("Input size", invalidTripStats.bytesProcessed);
    data.newData("Elapsed", invalidTripStats.elapsedSeconds);
    data.newData("Batch size", invalidTripStats.numBatch);
    data.newData("Rows/sec", invalidTripStats.rowsPerSecond());
    data.newData("Troughput", invalidTripStats.megabytesPerSecond());

    std::filesystem::create_directories(output);
    const std::filesystem::path fileName = std::filesystem::path(output) / "InvalidTripsManifest.json";

    JsonGenerator generator;
    generator.generateManifest(fileName.string(), data);
}