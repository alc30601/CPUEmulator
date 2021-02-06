// main.cpp
//
// Remote git repository is below.
// https://github.com/alc30601/DataFlowGraph.git

#include <iostream>
#include <memory>

#include "Edge.hpp"
#include "Node.hpp"

#include "StartNode.hpp"
#include "TerminateNode.hpp"




//-----------------------------------------------------------
class MyNode : public Node
{
    std::shared_ptr<Edge<int>> _input;
    std::shared_ptr<Edge<int>> _output;

public:

    //-------------------------------------------------------
    void execute(void)
    {
        int vIn = _input->getValue();
        _output->setValue(vIn);
    }

    //-------------------------------------------------------
    void set_edge(std::shared_ptr<Edge<int>> sp_input, std::shared_ptr<Edge<int>> sp_output)
    {
        _input = sp_input;
        _output = sp_output;
    }

};





//-----------------------------------------------------------
int main(void)
{
    std::cout << "-- DataFlowGraph -- " << std::endl;

    StartNode sNode;
    TerminateNode tNode;
    MyNode node;

    std::shared_ptr<Edge<int>> spEdge1(new Edge<int>());
    std::shared_ptr<Edge<int>> spEdge2(new Edge<int>());

    node.set_edge(spEdge1, spEdge2);
    sNode.set_edge(spEdge1);
    tNode.set_edge(spEdge2);

    sNode.execute();

    return 0;
}

