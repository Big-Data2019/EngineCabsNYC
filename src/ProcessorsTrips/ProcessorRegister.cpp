#include "ProcessorsTrips/ProcessorRegister.hpp"

void ProcessorRegister::registerProcessors(RawStats& stats){
    processors["raw"] = std::make_unique<RawCounter>(stats);
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