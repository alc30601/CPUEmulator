// test_08.hpp
#ifndef __TEST_08_HPP__
#define __TEST_08_HPP__

#include "Node.hpp"
#include "Edge.hpp"
#include "GraphBuilder.hpp"

#include "NodeLogics.hpp"
#include "NodeArithmetic.hpp"
#include "NodeTestBase.hpp"


//-----------------------------------------------------------
void test08(void)
{
    GraphBuilder gb;

    auto n0 = gb.createNode<NodeTestEntry<double>>();
    auto n1 = gb.createNode<NodeGain>();
    auto n2 = gb.createNode<NodeAdd>();
    auto n3 = gb.createNode<NodeAdd>();
    auto n4 = gb.createNode<NodeMul>();
    auto n5 = gb.createNode<NodeTestExit<double>>();

    gb.outto(Port(n0, 1), Port(n2, 1));
    gb.outto(Port(n0, 2), Port(n1, 1));
    gb.outto(Port(n0, 3), Port(n3, 2));
    gb.outto(Port(n1, 1), Port(n2, 2), Port(n3, 1));
    gb.outto(Port(n2, 1), Port(n4,1));
    gb.outto(Port(n3, 1), Port(n4, 2));
    gb.outto(Port(n4, 1), Port(n5, 1));

}

#endif


