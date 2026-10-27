#include "Manifest/Json/ValidateParquetManifest.hpp"
#include "Manifest/JsonGenerator.hpp"
#include "Manifest/JsonBuilder.hpp"
#include <filesystem>

void generateValidationManifest(const InvalidInfo& info, const std::string& output){
    JsonBuilder data;

    data.newData("Total rows", info.totalRows);
    data.newData("Valid rows", info.validRows);
    data.newData("Invalid rows", info.invalidRows);
    data.newData("Null values", info.nullValues);
    data.newData("tripDistanceInvalids", info.tripDistanceInvalids);
    data.newData("tripDurationInvalids", info.tripDurationInvalids);
    data.newData("fareAmountInvalids", info.fareAmountInvalids);
    data.newData("totalAmountInvalids", info.totalAmountInvalids);
    data.newData("locationIdInvalids", info.locationIdInvalids);
    data.newData("passengerCountInvalids", info.passengerCountInvalids);
    data.newData("paymentTypeInvalids", info.paymentTypeInvalids);

    std::filesystem::create_directories(output);
    const std::filesystem::path fileName = std::filesystem::path(output) / "ValidateParquet.json";

    JsonGenerator generator;
    generator.generateManifest(fileName.string(), data);
}