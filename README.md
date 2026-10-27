# EngineCabsNYC

*Analysis engine for intelligent decision-making based on hard data*
---

**EngineCabsNYC** is a modular, command-line project designed to work with datasets in .parquet format and perform analyses on *NYC taxi trips*
**Status:** The "validate" command works 
**Goal:** Add a .csv format to generate manifests


This project use C++ 20 with libraries Apache arrow and Parquet to work with batches and schema, also independets modules to configuraton, network, download files

## Features

- Data processors: Analysis using processors such as row counters or revenue by day
- Command-Line Interface: Managing Options and Commands Using CLI11
- Download configuration: Using a dataset in .yaml format and the configuration module
- Modular architecture: Each library is separated by responsibilities

## Technologies

 - Windows
 - C++ 20
 - CMake 3.20
 - Ninja
 - vcpkg
 - GCC/G++

### Stack

 - fmt
 - CLI11
 - yaml-cpp
 - Apache Arrow
 - Apache Parquet
 - spdlog
 - Curl
 - nlohmann_json
 - Google Tests
 - Google Benchmarks

## Requeriments

- Windows
- MSYS2 UCRT64
- GCC/G++
- CMake 3.20 or posterior
- Ninja
- The dependencies of the stack

## Installation

In a UCRT64 terminal execute:
```
pacman -S --needed \
    mingw-w64-ucrt-x86_64-gcc \
    mingw-w64-ucrt-x86_64-cmake \
    mingw-w64-ucrt-x86_64-ninja \
    mingw-w64-ucrt-x86_64-arrow \
    mingw-w64-ucrt-x86_64-curl \
    mingw-w64-ucrt-x86_64-yaml-cpp \
    mingw-w64-ucrt-x86_64-spdlog \
    mingw-w64-ucrt-x86_64-fmt \
    mingw-w64-ucrt-x86_64-cli11 \
    mingw-w64-ucrt-x86_64-nlohmann-json \
    mingw-w64-ucrt-x86_64-gtest \
    mingw-w64-ucrt-x86_64-benchmark \
```

The --needed option avoid re-install packages already uptaded

## Usage

1. Open the project 
- Open MSYS2 UCRT64 and stay in the project root

2. Configure
```
cmake --preset ucrt64-ninja
```

This command configure the project using the preset of ucrt64-ninja, using Ninja and GCC/G++ compilers

3. Compile
```
cmake --build -preset ucrt64-build
```

This command compile the project using the preset of ucrt64-build

4. Execute
```
./build-ucrt64/EngineCabsNYC.exe --help
```

This command request help to the Command-Line Interface

## Configuration

## Project Structure
*Modules*
- Network: Red operation and RAII of curl's library
- Config: Dataset configuration of .yaml file
- Cli: Command-line interface
- Download: Download of .parquet files from *NYC taxi trips*
- Filesystem: Operation over the file system
- Manifest: Management of manifest generation following operations
- Parquet: Integration and analysis of .parquet files
- ProcessorsTrips: Processors over the .parquet files
- Utils: Shared utilities in the project