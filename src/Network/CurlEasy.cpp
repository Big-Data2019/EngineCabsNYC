#include "Network/CurlEasy.hpp"
#include <stdexcept>

namespace netEasy{
CurlEasy::CurlEasy() :
    curl(curl_easy_init())
{
    if(!curl){
        throw std::runtime_error("Failed to initialize libcurl easy handle");
    }
}

CurlEasy::~CurlEasy(){
    if(curl){
        curl_easy_cleanup(curl);
    }
}

CURL* CurlEasy::get() const{
    return curl;
}
}