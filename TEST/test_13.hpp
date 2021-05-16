// test_13.hpp
#ifndef __TEST_13_HPP__
#define __TEST_13_HPP__

#include "NodeLogics.hpp"
#include "NodeTestBase.hpp"

#include "GraphFlatten.hpp"

//-----------------------------------------------------------
void test13_01(void)
{
    bool result;

    NodeNot node_not;
    result = isTheClass<NodeSubSystem, Node>(&node_not);
    std::cout << "NodeNot is subclass of NodeSubSystem ? " << result << std::endl;

    NodeAnd node_and;
    result = isTheClass<NodeSubSystem, Node>(&node_and);
    std::cout << "NodeAnd is subclass of NodeSubSystem ? " << result << std::endl;

    NodeNor node_nor;
    result = isTheClass<NodeSubSystem, Node>(&node_nor);
    std::cout << "NodeNor is subclass of NodeSubSystem ? " << result << std::endl;
}

//-----------------------------------------------------------
void test13_02(void)
{
    GraphBuilder gb;
    auto qnT = gb.createNode<NodeNand>(typeid(NodeNand).name());
    auto qnS = gb.createNode<NodeTestEntry<bool>>("NodeTestEntry");
    auto qnE = gb.createNode<NodeTestExit<bool>>("NodeTestExit");

    for(int i=1;i<= 2; i++){
        gb.outto(Port(qnS, i), Ports{ Port(qnT,i) }, typeid(bool));
    }

    for(int i=1;i<= 1;i++){
        gb.outto(Port(qnT, i), Ports{ Port(qnE,i) }, typeid(bool));
    }

    auto edges = gb.getEdges();
    auto nodes = gb.getNodes();

    auto flatten = graphFlatten(nodes, edges);
    std::cout << flatten.first.size() << std::endl;
    std::cout << flatten.second.size() << std::endl;

}

//-----------------------------------------------------------
void test13(void)
{
    test13_01();
    test13_02();
}


#endif

