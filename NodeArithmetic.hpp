// NodeArithmetic.hpp
#ifndef __NODEARITHMETIC_HPP__
#define __NODEARITHMETIC_HPP__

#include <iostream>
#include <functional>

#include "Edge.hpp"
#include "Node.hpp"
// #include "NodeBase.hpp"


//-----------------------------------------------------------
// template class for N input and one output
// 
// The way of caclulating
// v1 ----+
//       _|_
// v2 --|_f |--+
//             |
// v3 -------|_f |-- ...     --+
//  .....                      |
// vn -------------- .... ---|_f |----> output value
// That is ...
// output value = ((((v1 f v2) f v3) f v4) ... vn)
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
        Node::execute();

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
// two or more input values added
template <typename T>
class NodeAdd : public NodeN2One<T>
{
public:
    NodeAdd() : NodeN2One<T>([](T a, T b) -> T { return (a + b); }){}
};

//-----------------------------------------------------------
// two or more input values multiplied
template <typename T>
class NodeMul : public NodeN2One<T>
{
public:
    NodeMul() : NodeN2One<T>([](T a, T b) -> T { return (a * b); }){}
};



#endif

