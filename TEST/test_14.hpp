// test_14.hpp
// GraphFlatten.hppのテスト
#ifndef __TEST_14_HPP__
#define __TEST_14_HPP__

#include <algorithm>

#include "Node.hpp"
#include "NodeComplex.hpp"
#include "NodeTestBase.hpp"
#include "ExecutorFactory.hpp"


//-----------------------------------------------------------
class NodeThrough : public Node
{
public:
    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        std::vector<Edge*> inEdges = getInEdges();
        std::vector<Edge*> outEdges = getOutEdges();

        int numEdges = std::min(inEdges.size(), outEdges.size());
        for(int i=0;i<numEdges;i++){
            auto value = inEdges[i]->getValue();
            outEdges[i]->setValue(value);
        }
    }
};

//-----------------------------------------------------------
class NodeTest14_01 : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeTest14_01(void)
    {
        auto& gb = getGraphBuilder();
        auto enty = getEntryNode();
        auto exit = getExitNode();

        auto n1 = gb.createNode<NodeThrough>("n1");
        auto n2 = gb.createNode<NodeThrough>("n2");
        auto n3 = gb.createNode<NodeThrough>("n3");

        gb.outto(Port(enty, 1), Ports{ Port(n1, 1), Port(n2, 1), Port(n3, 1) }, typeid(bool));
        gb.outto(Port(n1, 1), Ports{Port(exit, 1), Port(exit, 2), Port(exit, 3)}, typeid(bool));

        commit();
    }
};

//-----------------------------------------------------------
void test14_01(void)
{
    GraphBuilder gb;
    auto qnT = gb.createNode<NodeTest14_01>("NodeTest14_01");
    auto qnS = gb.createNode<NodeTestEntry<bool>>("NodeTestEntry");
    auto qnE = gb.createNode<NodeTestExit<bool>>("NodeTestExit");
    auto e1 = gb.createNode<NodeThrough>("e1");
    auto e2 = gb.createNode<NodeThrough>("e2");

    gb.outto(Port(qnS, 1), Ports{ Port(qnT, 1), Port(e1, 1), Port(e2, 1) }, typeid(bool));
    gb.outto(Port(qnT, 1), Ports{ Port(qnE, 1), Port(qnE, 2), Port(qnE, 3) }, typeid(bool));

    std::vector<Node*>& nodes = gb.getNodes();
    std::vector<Edge*>& edges = gb.getEdges();
    std::pair<std::vector<Node*>, std::vector<Edge*>> p = graphFlatten(nodes, edges);
    nodes = p.first;
    edges = p.second;

    auto exe = createExecutor();
    exe->setStartNode(static_cast<Node*>(qnS.getNode()));
    exe->addNodes(nodes);
    exe->addEdges(edges);

    std::vector<bool> test_vector = {T};
    std::vector<bool> expected = {T, T, T};
    evaluation<bool, bool>(exe, qnS, qnE, test_vector, expected, false);
}

//-----------------------------------------------------------
void test14(void)
{
    test14_01();
}

#endif

