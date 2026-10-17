# Benchmark executable
add_executable(EngineCabsNYCBench BenchMain.cpp)

target_link_libraries(EngineCabsNYCBench
PRIVATE
    benchmark::benchmark_main
)