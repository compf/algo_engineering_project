#pragma once
#include "abstract_steiner_graph.cpp"
#include <vector>
#include<fstream>
#include <sstream>
class AdjacencyBasedSteinerGraph : public AbstractSteinerGraph
{
private:
    std::vector<std::vector<bool>> matrix;
    std::set<NODE_ID_TYPE> importantNodes;
    std::set<NODE_ID_TYPE> nodes;
    std::set<std::pair<NODE_ID_TYPE, NODE_ID_TYPE>> edges;

public:
    virtual bool isConnected(NODE_ID_TYPE from, NODE_ID_TYPE to){
        return matrix[from][to];
    }
    const std::set<NODE_ID_TYPE> getOutgoingNeighbors(NODE_ID_TYPE from) 
    {
        std::set<NODE_ID_TYPE> result;
        for (NODE_ID_TYPE to = 0; to < matrix.size(); to++)
        {
            if (isConnected(from, to))
                result.emplace(to);
        }
        return result;
    }
    virtual const std::set<NODE_ID_TYPE> getIncomingNeighbors(NODE_ID_TYPE to)
    {
        std::set<NODE_ID_TYPE> result;
        for (NODE_ID_TYPE from = 0; from <= matrix.size(); from++)
        {
            if (isConnected(from, to))
                result.emplace(from);
        }
        return result;
    }
    virtual AbstractSteinerGraph *clone(){
        auto graph=new AdjacencyBasedSteinerGraph();
        graph->nodes=nodes;
        graph->edges=edges;
        graph->matrix=matrix;
        graph->importantNodes=importantNodes;
        return graph;
    }

    virtual bool removeEdge(NODE_ID_TYPE from, NODE_ID_TYPE to){
        matrix[from][to]=false;
        edges.erase(std::make_pair(from,to));
    }
    virtual const std::set<NODE_ID_TYPE> &getAllNodes(){
        return nodes;

    }
    virtual const std::set<NODE_ID_TYPE> &getAllImportantNodes(){
        return importantNodes;
    }
    virtual std::set<std::pair<NODE_ID_TYPE, NODE_ID_TYPE>>  load(std::string path) {
        auto stream=std::ifstream(path);
        std::getline(stream,this->name);
        NODE_ID_TYPE numberNodes;
        stream>>numberNodes;
        matrix.resize(numberNodes,std::vector<bool>(numberNodes));
        std::string importantNodeString;
        stream>>importantNodeString;
        for(NODE_ID_TYPE i=0;i<numberNodes;i++){
            if(importantNodeString[i]=='1'){
                importantNodes.emplace(i);
            }
            nodes.emplace(i);
        }
        NODE_ID_TYPE numberEdges;
        stream>>numberEdges;
        std::set<std::pair<NODE_ID_TYPE,NODE_ID_TYPE>> validSolutions;
         std::string line;
        std::getline(stream,line);
        for(NODE_ID_TYPE i=0;i<numberEdges;i++){
           
         
            std::getline(stream,line);
            if(std::count(line.begin(),line.end(),' ')==2){
                NODE_ID_TYPE from,to,validSolution;
                std::stringstream str_stream(line);
                str_stream>>from>>to>>validSolution;
                matrix[from][to]=true;
                edges.emplace(std::make_pair(from,to));
                validSolutions.emplace(std::make_pair(from,to));
            }
            else{
                NODE_ID_TYPE from,to;
                 std::stringstream str_stream(line);
                str_stream>>from>>to;
                edges.emplace(std::make_pair(from,to));
                matrix[from][to]=true;
            }
        }
        return validSolutions;
    } 
         const std::set<std::pair<NODE_ID_TYPE, NODE_ID_TYPE>> &getEdges() override{
            return edges;
        }
  
    virtual AbstractSteinerGraph *createSubGraph(std::set<std::pair<NODE_ID_TYPE, NODE_ID_TYPE>> &validSolutions) {
        AdjacencyBasedSteinerGraph* subGraph=new AdjacencyBasedSteinerGraph();
        subGraph->name=this->name;
        subGraph->matrix.resize(this->matrix.size(),std::vector<bool>(this->matrix.size()));
        subGraph->importantNodes=this->importantNodes;
        for(auto pair:validSolutions){
            subGraph->nodes.emplace(pair.first);
            subGraph->nodes.emplace(pair.second);
            subGraph->matrix[pair.first][pair.second]=true;

        }
        return subGraph;
    }
    virtual void save(std::string path, std::set<std::pair<NODE_ID_TYPE, NODE_ID_TYPE>> &validSolutions) {

    }
};