#include "Network/CurlGlobal.hpp"
#include <stdexcept>

namespace netGlobal{
CurlGlobal::CurlGlobal(long flags){
    if(curl_global_init(flags) != CURLE_OK){
        throw std::runtime_error("Failed to global initialize libcurl handler");
    }
}

CurlGlobal::~CurlGlobal(){
    curl_global_cleanup();
}
}