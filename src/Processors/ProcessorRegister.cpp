#include "Include/Processors/ProcessorRegister.hpp"
#include "Include/Processors/RawCounter.hpp"

void ProcessorRegister::registerProcessors(){
    processors["raw"] = std::make_unique<RawCounter>();
}

void ProcessorRegister::searchProcessor(const ProcessorData& data){
    for(const auto& processor : data.processors){
        auto it = processors.find(processor);

        if(it != processors.end()){
            it->second->process(data.batchSize, data.filesToProcess);
            it->second->finish(data.output);
        }
    }
}