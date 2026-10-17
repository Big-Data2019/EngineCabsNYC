#include "ProcessorsTrips/Processors/TripsByHour.hpp"
#include "Manifest/JsonGenerator.hpp"
#include "Manifest/JsonBuilder.hpp"
#include <filesystem>

void TripsByHour::finish(const std::string& output){
    JsonBuilder data;
    std::vector<nlohmann::ordered_json> tripsHours;
    for(const auto& [hourId, trip] : tripsByHour){
        nlohmann::ordered_json tripHour;

        tripHour["hourId"] = hourId;
        tripHour["trips"] = trip;

        tripsHours.push_back(tripHour);
    }

    data.newData("Files", tripsHStats.files);
    data.newData("Batches", tripsHStats.batchCounter);
    data.newData("Input size", tripsHStats.bytesProcessed);
    data.newData("Elapsed", tripsHStats.elapsedSeconds);
    data.newData("Batch size", tripsHStats.numBatch);
    data.newData("Rows/sec", tripsHStats.rowsPerSecond());
    data.newData("Troughput", tripsHStats.megabytesPerSecond());
    data.manyNewData("Trips by hour", tripsHours);

    std::filesystem::create_directories(output);
    const std::filesystem::path fileName = std::filesystem::path(output) / "TripsByHourManifest.json";

    JsonGenerator generator;
    generator.generateManifest(fileName.string(), data);
}