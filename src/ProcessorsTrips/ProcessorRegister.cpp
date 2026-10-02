#include "ProcessorsTrips/ProcessorRegister.hpp"

void ProcessorRegister::registerProcessors(RowStats& stats){
    processors["row"] = std::make_unique<RowCounter>(stats);
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