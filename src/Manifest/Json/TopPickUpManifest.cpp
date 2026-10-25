#include "ProcessorsTrips/Processors/TopPickupZones.hpp"
#include "Manifest/JsonGenerator.hpp"
#include "Manifest/JsonBuilder.hpp"
#include <filesystem>
#include <algorithm>

void TopPickupZones::finish(const std::string& output){
    JsonBuilder data;

    std::vector<std::pair<std::int32_t, std::int64_t>> zones(pickUpZone.begin(), pickUpZone.end());
    std::sort(zones.begin(), zones.end(), [](const auto& a, const auto& b){
        return a.second > b.second;
    });

    std::vector<nlohmann::ordered_json> topZones;
    const std::size_t amount = std::min<std::size_t>(10, zones.size());

    for(std::size_t i = 0; i < amount; i++){
        nlohmann::ordered_json zone;

        zone["rank"] = i + 1;
        zone["zoneId"] = zones[i].first;
        zone["trips"] = zones[i].second;

        topZones.push_back(zone);
    }

    data.newData("Files", pickupZoneStats.files);
    data.newData("Batches", pickupZoneStats.batchCounter);
    data.newData("Input size", pickupZoneStats.bytesProcessed);
    data.newData("Elapsed", pickupZoneStats.elapsedSeconds);
    data.newData("Batch size", pickupZoneStats.numBatch);
    data.newData("Rows/sec", pickupZoneStats.rowsPerSecond());
    data.newData("Troughput", pickupZoneStats.megabytesPerSecond());
    data.manyNewData("Pickup zones", topZones);

    std::filesystem::create_directories(output);
    const std::filesystem::path fileName = std::filesystem::path(output) / "TopPickUpManifest.json";

    JsonGenerator generator;
    generator.generateManifest(fileName.string(), data);
}