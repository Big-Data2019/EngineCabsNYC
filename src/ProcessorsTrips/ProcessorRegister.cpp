#include "ProcessorsTrips/ProcessorRegister.hpp"

void ProcessorRegister::registerProcessors(RowStats& rowStats, RevenueDayStats& revenueDStats){
    processors["row"] = std::make_unique<RowCounter>(rowStats);
    processors["revenueD"] = std::make_unique<RevenueByDay>(revenueDStats);
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