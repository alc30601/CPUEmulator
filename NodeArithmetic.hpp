// NodeArithmetic.hpp
#ifndef __NODEARITHMETIC_HPP__
#define __NODEARITHMETIC_HPP__

#include <iostream>

#include "NodeBase.hpp"



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

//-----------------------------------------------------------
// output value is input value multiply gain
class NodeGain : public Node1In1Out<double>
{
    double _gain;

public:
    //-------------------------------------------------------
    NodeGain(void)
    {
        _gain = 1.0;    // デフォルト値を1とする。
    }

    //-------------------------------------------------------
    void setParam(double gain)
    {
        _gain = gain;
    }

    //-------------------------------------------------------
    double calculate(double input_value)
    {
        double result = _gain * input_value;
        return result;
    }


};



//-----------------------------------------------------------
// ***** OBSOLATE *****
// always puts constant value to out edge
// This node doesnot need input value. But Executor mechanism requires
// at least one input edge in order to execute the node.
// class NodeConst : public Node1In1Out<double>
// {
//     double _value;

// public:
//     //-------------------------------------------------------
//     NodeConst(void)
//     {
//         _value = 1.0;    // デフォルト値を1とする。
//     }

//     //-------------------------------------------------------
//     void setParam(double value)
//     {
//         _value = value;
//     }

//     //-------------------------------------------------------
//     double calculate(double input_value)
//     {
//         return _value;
//     }

// };



#endif

