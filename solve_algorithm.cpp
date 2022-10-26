#pragma once;
#include <chrono>
#include "abstract_steiner_graph.cpp"
class AbstractAlgorithm
{
private:
    AbstractSteinerGraph *solution = nullptr;

public:
    AbstractSteinerGraph *getSolution()
    {
        return solution;
    }
    virtual AbstractSteinerGraph *solve(AbstractSteinerGraph *graph) = 0;
    std::chrono::nanoseconds run(AbstractSteinerGraph *graph)
    {
        auto start = std::chrono::system_clock::now();
        this->solution = solve(graph);
        auto end = std::chrono::system_clock::now();
        auto elapsed = end - start;
        return elapsed;
    }
};
class SimpleAlgorithm : public AbstractAlgorithm
{
    virtual AbstractSteinerGraph *solve(AbstractSteinerGraph *graph)
    {
        auto backup=graph->clone();
        for(auto edge:backup->getEdges()){
            auto clone=graph->clone();
            clone->removeEdge(edge.first,edge.second);
            if(backup->isValid(clone)){
                graph->removeEdge(edge.first,edge.second);
            }
            delete clone;
        }
        auto adjBackup=(AdjacencyBasedSteinerGraph*)backup;
        auto adjGraph=(AdjacencyBasedSteinerGraph*)graph;
        delete backup;
        return graph;
    }
};