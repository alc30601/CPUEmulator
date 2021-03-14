// NodeTestBase.hpp
#ifndef __NODETESTBASE_HPP__
#define __NODETESTBASE_HPP__

#include <any>

#include "Edge.hpp"
#include "Node.hpp"
#include "Executor.hpp"
#include "GraphBuilder.hpp"


//-----------------------------------------------------------
// ０入力、複数出力ノード
template <typename T>
class NodeTestEntry : public Node
{
    std::vector<T> _values;

public:
    //-------------------------------------------------------
    void setValues(std::vector<T> values)
    {
        _values = values;
    }

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();


        std::vector<Edge*> outEdges = getOutEdges();

        std::cout << "Input Values : ";
        for(int i=0; i<outEdges.size();i++){
            T value = _values.at(i);
            outEdges.at(i)->setValue(value);
            std::cout << value << " , ";
        }
        std::cout << std::endl;
    }

};


//-----------------------------------------------------------
// 複数入力、0出力ノード
template <typename T>
class NodeTestExit : public Node
{
    std::vector<T> _values;

public:
    //-------------------------------------------------------
    std::vector<T> getValues(void)
    {
        return _values;
    }

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        std::vector<Edge*> inEdges = getInEdges();

        std::cout << "Output Values : ";

        if(inEdges.size() > _values.size()){
            _values.resize(inEdges.size());
        }

        for(int i=0;i<inEdges.size();i++){
            auto edge = inEdges[i];
            T value = std::any_cast<T>(edge->getValue());
            _values[i] = value;
            std::cout << value << " , ";
        }

        std::cout << std::endl;
    }
};





//-----------------------------------------------------------
// obsolate but still used by test_06...
// instead of this function, use test_2to1_template()
template <typename TYPENODE, typename T>
std::tuple<Executor*, NodeTestEntry<T>*> test_2to1(void)
{
    NodeTestEntry<T>* nEntry(new NodeTestEntry<T>);
    NodeTestExit<T>* nExit(new NodeTestExit<T>);
    TYPENODE* nOp(new TYPENODE);

    Edge* e1(new Edge);
    Edge* e2(new Edge);
    Edge* e3(new Edge);
    Edge* e4(new Edge);

    nEntry->addOutEdges(std::vector<Edge*>{e1,e2});
    nOp->addInEdges(std::vector<Edge*>{e1,e2});
    nOp->addOutEdges(std::vector<Edge*>{e3});
    nExit->addInEdges(std::vector<Edge*>{e3});

    e1->addOutNode(nOp);
    e2->addOutNode(nOp);
    e3->addOutNode(nExit);

    Executor* exe(new Executor(nEntry, std::vector<Node*>{nEntry,nExit,nOp}, std::vector<Edge*>{e1,e2,e3}));

    std::tuple<Executor*, NodeTestEntry<T>*> ret = std::make_tuple(exe, nEntry);
    return ret;
}

//-----------------------------------------------------------
template <typename TYPENODE, typename T>
std::tuple<Executor*, QuasiNode, QuasiNode> test_2to1_template(void)
{
    GraphBuilder gb;
    auto nTarget = gb.createNode<TYPENODE>();
    auto nEntry = gb.createNode<NodeTestEntry<T>>();
    auto nExit = gb.createNode<NodeTestExit<T>>();

    gb.outto(Port(nEntry, 1), Port(nTarget,1));
    gb.outto(Port(nEntry, 2), Port(nTarget,2));
    gb.outto(Port(nTarget,1), Port(nExit,1));

    Executor* exe = gb.createExecutor(nEntry);
    auto ret = std::tie(exe, nEntry, nExit);
    return ret;
}


//-----------------------------------------------------------
template <typename TYPENODE, typename T>
std::tuple<Executor*, NodeTestEntry<T>*> test_1to1(T param)
{
    NodeTestEntry<T>* nEntry(new NodeTestEntry<T>);
    NodeTestExit<T>* nExit(new NodeTestExit<T>);
    TYPENODE* nOp(new TYPENODE());
    nOp->setParam(param);

    Edge* e1(new Edge);
    Edge* e2(new Edge);

    nEntry->addOutEdges(std::vector<Edge*>{e1});
    nOp->addInEdges(std::vector<Edge*>{e1});
    nOp->addOutEdges(std::vector<Edge*>{e2});
    nExit->addInEdges(std::vector<Edge*>{e2});

    e1->addOutNode(nOp);
    e2->addOutNode(nExit);

    Executor* exe(new Executor(nEntry, std::vector<Node*>{nEntry,nExit,nOp}, std::vector<Edge*>{e1,e2}));

    std::tuple<Executor*, NodeTestEntry<T>*> ret = std::make_tuple(exe, nEntry);
    return ret;

}



#endif

