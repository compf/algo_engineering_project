#pragma once
#include <fstream>
#include <iostream>
#include <set>
#include <algorithm>
#include <tuple>
#include <string>
#include <stack>
#include <queue>
typedef unsigned long NODE_ID_TYPE;
class AbstractSteinerGraph
{
protected:
    std::string name;

public:
    virtual bool isImportant(NODE_ID_TYPE id)
    {
        auto importantNodes = getAllImportantNodes();
        return importantNodes.find(id) != importantNodes.end();
    }
    virtual bool isConnected(NODE_ID_TYPE from, NODE_ID_TYPE to) = 0;
    virtual const std::set<NODE_ID_TYPE> getOutgoingNeighbors(NODE_ID_TYPE id) = 0;
    virtual const std::set<NODE_ID_TYPE> getIncomingNeighbors(NODE_ID_TYPE id) = 0;
    virtual bool removeEdge(NODE_ID_TYPE from, NODE_ID_TYPE to) = 0;
    virtual const std::set<NODE_ID_TYPE> &getAllNodes() = 0;
    virtual const std::set<NODE_ID_TYPE> &getAllImportantNodes() = 0;
    virtual const std::set<std::pair<NODE_ID_TYPE, NODE_ID_TYPE>> &getEdges() = 0;
    virtual  std::set<std::pair<NODE_ID_TYPE, NODE_ID_TYPE>> load(std::string path) = 0;
    virtual AbstractSteinerGraph *createSubGraph(std::set<std::pair<NODE_ID_TYPE, NODE_ID_TYPE>> &validSolutions) = 0;
    virtual AbstractSteinerGraph *clone() = 0;
    virtual void save(std::string path, std::set<std::pair<NODE_ID_TYPE, NODE_ID_TYPE>> &validSolutions) = 0;
    std::string getName()
    {
        return name;
    }
    template <typename Waiting>
    bool isReachable(NODE_ID_TYPE from, NODE_ID_TYPE to)
    {
        Waiting list;
        std::set<NODE_ID_TYPE> visited;
        list.push(from);
        while (list.size() > 0)
        {
            NODE_ID_TYPE curr = list.top();list.pop();
            if (visited.find(curr) != visited.end())
                continue;
            visited.emplace(curr);
             if (curr == to) return true;
            for (auto neigh : this->getOutgoingNeighbors(curr))
            {
                list.push(neigh);
            }
        }
        return false;
    }

    virtual bool isValid(AbstractSteinerGraph *subGraph)
    {
        for (auto n1 : this->getAllImportantNodes())
        {
            for (auto n2 : this->getAllImportantNodes())
            {
                if(n1==n2)continue;;
                if(this->isReachable<std::stack<NODE_ID_TYPE>>(n1,n2) && !subGraph->isReachable<std::stack<NODE_ID_TYPE>>(n1,n2) ){
                    return false;
                }
            }
        }
        return true;
    }
};