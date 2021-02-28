// NodeArithmetic.hpp
#ifndef __NODEARITHMETIC_HPP__
#define __NODEARITHMETIC_HPP__

#include <iostream>
#include <functional>

#include "NodeBase.hpp"

//-----------------------------------------------------------
// The way of caclulating
// v1 ----+
//       _|_
// v2 --|_f |--+
//             |
// v3 -------|_f |-- ...     --+
//  .....                      |
// vn -------------- .... ---|_f |----> output value
template <typename T>
class NodeN2One : public Node
{
    // function object that takes two input and put one output
    // this object must be set at constructor
    std::function<T(T,T)> _f;

public:
    NodeN2One(std::function<T(T,T)> f) : _f(f){}

    //-------------------------------------------------------
    void execute(void)
    {
        std::vector<Edge*> inEdges = getInEdges();
        std::vector<Edge*> outEdges = getOutEdges();

        // check number of input edge -- must be equal or largeer than 2
        std::size_t size = inEdges.size();
        if(size < 2){
            std::cout << "Input Edges of this Node is less than two" << std::endl;
        }

        // at least 2 input edges exist
        // first get value from first edge, then loop from second to last
        // for example, function is plus(+), the output value is
        // value1 + value2 + value3 + ...
        auto value = std::any_cast<T>(inEdges.at(0)->getValue());
        for(int i=1;i<size;i++){
            auto value2 = std::any_cast<T>(inEdges.at(i)->getValue());
            value = _f(value, value2);
        }

        // set output edge -- assume number of output edge must be one
        outEdges.at(0)->setValue(value);
    }

};

//-----------------------------------------------------------
template <typename T>
class NodeOne2One : public Node
{
    std::function<T(T)> _f;

public:
    NodeOne2One(std::function<T(T)> f) : _f(f){}

    //-------------------------------------------------------
    void execute(void)
    {
        std::vector<Edge*> inEdges = getInEdges();
        std::vector<Edge*> outEdges = getOutEdges();

        // check number of input edge -- must be one
        std::size_t size = inEdges.size();
        if(size != 1){
            std::cout << "Input Edges of this Node is not one" << std::endl;
        }

        auto input = std::any_cast<double>(inEdges.at(0)->getValue());
        auto output = _f(input);

        // set output edge -- assume number of output edge must be one
        outEdges.at(0)->setValue(output);
    }

};


//-----------------------------------------------------------
// two or more input values added
class NodeAdd : public NodeN2One<double>
{
public:
    NodeAdd() : NodeN2One([](double a, double b) -> double { return (a + b); }){}
};

//-----------------------------------------------------------
// two or more input values multiplied
class NodeMul : public NodeN2One<double>
{
public:
    NodeMul() : NodeN2One([](double a, double b) -> double { return (a * b); }){}
};

//-----------------------------------------------------------
// output value is input value multiply gain
class NodeGain : public NodeOne2One<double>
{
public:
    NodeGain(double gain) 
        : NodeOne2One(std::bind([](double a, double b) -> double { return (a * b); }, gain, std::placeholders::_1)){}
};

//-----------------------------------------------------------
// always puts constant value to out edge
// This node doesnot need input value. But Executor mechanism requires
// at least one input edge in order to execute the node.
class NodeConst : public NodeOne2One<double>
{
public:
    NodeConst(double c) : NodeOne2One(std::bind([](double a) -> double { return a; }, c)){}
};

#endif

