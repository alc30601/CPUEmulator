// NodeArithmetic.hpp
#ifndef __NODEARITHMETIC_HPP__
#define __NODEARITHMETIC_HPP__

#include <iostream>

#include "NodeBase.hpp"



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
// always puts constant value to out edge
// This node doesnot need input value. But Executor mechanism requires
// at least one input edge in order to execute the node.
// class NodeConst : public NodeOne2One<double>
// {
// public:
//     NodeConst(double c) : NodeOne2One(std::bind([](double a) -> double { return a; }, c)){}
// };
// コンストラクタを引数なしにしたいので親クラスを変更
class NodeConst : public Node1In1Out<double>
{
    double _value;

public:
    //-------------------------------------------------------
    NodeConst(void)
    {
        _value = 1.0;    // デフォルト値を1とする。
    }

    //-------------------------------------------------------
    void setParam(double value)
    {
        _value = value;
    }

    //-------------------------------------------------------
    double calculate(double input_value)
    {
        return _value;
    }

};



#endif

