#include "Network/CurlEasy.hpp"

namespace netEasy{
CurlEasy::CurlEasy()
    : curl(curl_easy_init()){
    if(!curl){
        throw std::runtime_error("Failed to global initialize libcurl handler");
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