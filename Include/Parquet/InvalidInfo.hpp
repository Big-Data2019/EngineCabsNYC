#pragma once

#include <cstdint>

struct InvalidInfo{
    std::int64_t totalRows = 0;
    std::int64_t validRows = 0;
    std::int64_t invalidRows = 0;

    std::int64_t nullValues = 0;

    std::int64_t tripDistanceInvalids = 0;
    std::int64_t tripDurationInvalids = 0;
    std::int64_t fareAmountInvalids = 0;
    std::int64_t totalAmountInvalids = 0;
    std::int64_t locationIdInvalids = 0;
    std::int64_t passengerCountInvalids = 0;
    std::int64_t paymentTypeInvalids = 0;
};