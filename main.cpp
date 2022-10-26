
#include "adjacency_steiner_graph.cpp"
#include "solve_algorithm.cpp"
#include<iostream>
int main(int argc, char **argv)
{
    SimpleAlgorithm algo;
    AdjacencyBasedSteinerGraph graph;
    graph.load("example.txt");
    auto elapsed= algo.run(&graph);
    std::cout<<elapsed.count()<<std::endl;
    // Not finnished but first building blocks
    if (argc == 1)
    {
        std::cout << "HELP: timo_firas --in dat1 [--out dat2]" << std::endl;
    }
}