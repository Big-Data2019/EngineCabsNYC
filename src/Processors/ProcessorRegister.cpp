#include "Include/Processors/ProcessorRegister.hpp"
#include "Include/Processors/RawCounter.hpp"

void ProcessorRegister::registerProcessors(){
    processors["raw"] = std::make_unique<RawCounter>();
}