#pragma once
#include <curl/curl.h>

namespace netEasy{
class CurlEasy{
private:
    CURL* curl;

public:
    CurlEasy();
    ~CurlEasy();

    CURL* get() const;

    CurlEasy(const CurlEasy&) = delete;
    CurlEasy& operator = (const CurlEasy&) = delete;
};
}