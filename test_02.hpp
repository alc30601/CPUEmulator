// test_02.hpp
// 入力ノード２つと出力ノード１つ
// 各入力ノードから1本ずつエッジが出力ノードに
// トリガーをかけるために開始ノードを設ける(MyNode0)
//
//               e01    +---------+
// +--------+  +----->  | MyNode1 |---+            +---------+
// |MyNode0 |--+        +---------+   +--- e13 --->|         |
// |        |--+                                   | MyNode3 |
// +--------+  +----->  +---------+   +--- e23 --->|         |
//               e02    | MyNode2 |---+            +---------+
//                      +---------+


#ifndef __TEST_02_HPP__
#define __TEST_02_HPP__

#include "Edge.hpp"
#include "Node.hpp"
#include "Executor.hpp"


//-----------------------------------------------------------
class Test02MyNode0 : public Node
{
    Edge* _e01;
    Edge* _e02;

public:
    void setEdge(Edge* e01, Edge* e02)
    {
        addOutEdge(e01);
        addOutEdge(e02);
        _e01 = e01;
        _e02 = e02;
    }

    void execute(void)
    {
        std::cout << "Test02MyNode0::execute()" <<std::endl;
        Node::execute();
        _e01->setValue(0);  // just trigger
        _e02->setValue(0);  // just trigger
    }
};

//-----------------------------------------------------------
class Test02MyNode1 : public Node
{
    Edge* _e01;
    Edge* _e13;

public:
    void setEdge(Edge* e01, Edge* e13)
    {
        addInEdge(e01);
        addOutEdge(e13);
        _e01 = e01;
        _e13 = e13;
    }

    void execute(void)
    {
        std::cout << "Test02MyNode1::execute()" <<std::endl;
        Node::execute();
        _e13->setValue(3);
    }
};

//-----------------------------------------------------------
class Test02MyNode2 : public Node
{
    Edge* _e02;
    Edge* _e23;

public:
    void setEdge(Edge* e02, Edge* e23)
    {
        addInEdge(e02);
        addOutEdge(e23);
        _e02 = e02;
        _e23 = e23;
    }

    void execute(void)
    {
        std::cout << "Test02MyNode2::execute()" <<std::endl;
        Node::execute();
        _e23->setValue(3.2);
    }
};

//-----------------------------------------------------------
class Test02MyNode3 : public Node
{
    Edge* _e13;
    Edge* _e23;

public:
    void setEdge(Edge* e13, Edge* e23)
    {
        addInEdge(e13);
        addInEdge(e23);
        _e13 = e13;
        _e23 = e23;
    }

    void execute(void)
    {
        std::cout << "Test02MyNode3::execute()" <<std::endl;

        Node::execute();
        int a;
        double b;
        a = std::any_cast<int>(_e13->getValue());
        b = std::any_cast<double>(_e23->getValue());
        std::cout << "flowed value 1 is : " << a << std::endl;
        std::cout << "flowed value 2 is : " << b << std::endl;
    }
};

//-----------------------------------------------------------
void test02(void)
{
    // ノード生成
    Test02MyNode0* n0(new Test02MyNode0);
    Test02MyNode1* n1(new Test02MyNode1);
    Test02MyNode2* n2(new Test02MyNode2);
    Test02MyNode3* n3(new Test02MyNode3);

    // エッジ生成
    Edge* e01(new Edge(0));
    Edge* e02(new Edge(0));
    Edge* e13(new Edge(0));
    Edge* e23(new Edge(0.0));

    // ノードにエッジを紐付ける
    n0->setEdge(e01, e02);
    n1->setEdge(e01, e13);
    n2->setEdge(e02, e23);
    n3->setEdge(e13, e23);

    // エッジにノードを紐付ける
    e01->addOutNode(n1);
    e02->addOutNode(n2);
    e13->addOutNode(n3);
    e23->addOutNode(n3);

    std::vector<Node*> nodes = {n0, n1, n2, n3};
    std::vector<Edge*> edges = {e01, e02, e13, e23};

    // 実行
    Executor exe(n0, nodes, edges);
    exe.step();

}


#endif
