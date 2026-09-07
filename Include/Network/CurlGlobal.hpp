#pragma once

#include <curl/curl.h>

namespace netGlobal{
class CurlGlobal{
public:
    explicit CurlGlobal(long flags = CURL_GLOBAL_ALL);
    ~CurlGlobal();

    CurlGlobal(const CurlGlobal&) = delete;
    CurlGlobal& operator = (const CurlGlobal&) = delete;
};
}