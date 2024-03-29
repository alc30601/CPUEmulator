// ExecutorFactory.hpp
#ifndef __EXECUTORFACTORY_HPP__
#define __EXECUTORFACTORY_HPP__

#include "Executor.hpp"
#include "ExecutorDepthFirstTraversal.hpp"
#include "ExecutorNodeListTraversal.hpp"
#include "ExecutorStableEdge.hpp"
#include "ExecutorForCircuit.hpp"


//-----------------------------------------------------------
// Executor生成関数
// この関数内で生成するクラスを変えることにより
// Executorを切り替えることが出来る。
Executor* createExecutor(void)
{
    // Executor* executor = new ExecutorDepthFirstTraversal();
    // Executor* executor = new ExecutorNodeListTraversal();
    // Executor* executor = new ExecutorStableEdge();
    Executor* executor = new ExecutorForCircuit();

    return executor;
}


#endif

