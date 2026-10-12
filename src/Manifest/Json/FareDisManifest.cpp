#include "ProcessorsTrips/Processors/FareDistribution.hpp"
#include "Manifest/JsonBuilder.hpp"
#include "Manifest/JsonGenerator.hpp"

void FareDistribution::finish(const std::string& output){
    JsonBuilder data;

    data.newData("Files", fareDisStats.files);
    data.newData("Batches", fareDisStats.batchCounter);
    data.newData("Input size", fareDisStats.bytesProcessed);
    data.newData("Elapsed", fareDisStats.elapsedSeconds);
    data.newData("Batch size", fareDisStats.numBatch);
    data.newData("Rows/sec", fareDisStats.rowsPerSecond());
    data.newData("Troughput", fareDisStats.megabytesPerSecond());

    data.newData("Minimun", fareDisStats.minFare);
    data.newData("Average fare", fareDisStats.averageFare);
    data.newData("Max fare", fareDisStats.maxFare);

    data.newData("P25", fareDisStats.percentile25);
    data.newData("P50", fareDisStats.percentile50);
    data.newData("P75", fareDisStats.percentile75);
    data.newData("P99", fareDisStats.percentile99);

    std::vector<nlohmann::ordered_json> distribution;

    for(std::size_t i = 0; i < regularBucket; i++){
        nlohmann::ordered_json bucket;

        bucket["min"] = i * bucketWidth;
        bucket["max"] = (i + 1) * bucketWidth;
        bucket["trips"] = buckets[i];

        distribution.push_back(bucket);
        data.manyNewData("Fare distribution", distribution);
    }

    std::filesystem::create_directories(output);

    const std::filesystem::path fileName = std::filesystem::path(output) / "FareDisManifest.json";

    JsonGenerator generator;
    generator.generateManifest(fileName.string(), data);
}