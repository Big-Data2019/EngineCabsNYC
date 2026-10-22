#pragma once

struct ValidateInfo{
    int64_t totalRows = 0;
    int64_t validRows = 0;
    int64_t invalidRows = 0;

    int64_t tripDistanceInvalids = 0;
    int64_t tripDurationInvalids = 0;

    int64_t fareAmountInvalids = 0;
    int64_t totalAmountInvalids = 0;

    int64_t PULocationIdInvalids = 0;
    int64_t DOLocationIdInvalids = 0;

    int64_t passengerCountInvalids = 0;
    int64_t paymentTypeINvalids = 0;
};