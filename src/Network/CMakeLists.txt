# Network library
add_library(Network
    CurlGlobal.cpp
    CurlEasy.cpp
)

target_include_directories(Network PUBLIC ${PROJECT_SOURCE_DIR}/Include)

target_link_libraries(Network
PUBLIC
    CURL::libcurl
)