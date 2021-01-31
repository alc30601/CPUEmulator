// Executor.hpp
#ifndef __EXECUTOR_HPP__
#define __EXECUTOR_HPP__

#include <memory>

#include "Node.hpp"


//-----------------------------------------------------------
// Start node of the graph
class Executor
{
    std::shared_ptr<Node> _startNode;

public:
    //-------------------------------------------------------
    Executor(std::shared_ptr<Node> sNode)
    {
        _startNode = sNode;
    }

    //-------------------------------------------------------
    // execute flow graph from start node
    void step(void)
    {

    }

};

#endif



 