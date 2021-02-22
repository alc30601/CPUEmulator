// test_03.hpp
// 入力ノード１つと出力ノード２つ
// 入力ノードから1本エッジが分岐しそれぞれの出力ノードに
//
//               e01    +---------+
// +--------+  +----->  | MyNode1 |
// |MyNode0 |--+        +---------+
// +--------+  +----->  +---------+
//               e02    | MyNode2 |
//                      +---------+


#ifndef __TEST_03_HPP__
#define __TEST_03_HPP__

#include <string>
#include <memory>

#include "Edge.hpp"
#include "Node.hpp"
#include "Executor.hpp"


//-----------------------------------------------------------
class Test03MyNode0 : public Node
{
    Edge* _e0;

public:
    void setEdge(Edge* e0)
    {
        addOutEdge(e0);
        _e0 = e0;
    }

    void execute(void)
    {
        std::cout << "Test03MyNode0::execute()" <<std::endl;
        Node::execute();
        std::string str = "Hello World";
        _e0->setValue(str);  // just trigger
    }
};

//-----------------------------------------------------------
class Test03MyNode1 : public Node
{
    Edge* _e01;

public:
    void setEdge(Edge* e01)
    {
        addInEdge(e01);
        _e01 = e01;
    }

    void execute(void)
    {
        std::cout << "Test03MyNode1::execute()" <<std::endl;
        Node::execute();

        std::string a;
        a = std::any_cast<std::string>(_e01->getValue());
        std::cout << "flowed value is : " << a << std::endl;
    }
};

//-----------------------------------------------------------
class Test03MyNode2 : public Node
{
    Edge* _e02;

public:
    void setEdge(Edge* e02)
    {
        addInEdge(e02);
        _e02 = e02;
    }

    void execute(void)
    {
        std::cout << "Test03MyNode2::execute()" <<std::endl;
        Node::execute();

        std::string a;
        a = std::any_cast<std::string>(_e02->getValue());
        std::cout << "flowed value is : " << a << std::endl;
    }
};


//-----------------------------------------------------------
void test03(void)
{
    // ノード生成
    Test03MyNode0* n0(new Test03MyNode0);
    Test03MyNode1* n1(new Test03MyNode1);
    Test03MyNode2* n2(new Test03MyNode2);

    // エッジ生成
    std::string str = "";
    Edge* e0(new Edge(str));

    // ノードにエッジを紐付ける
    n0->setEdge(e0);
    n1->setEdge(e0);
    n2->setEdge(e0);

    // エッジにノードを紐付ける
    e0->addOutNode(n1);
    e0->addOutNode(n2);

    std::vector<Node*> nodes = {n0, n1, n2};
    std::vector<Edge*> edges = {e0};

    // 実行
    Executor exe(n0, nodes, edges);
    exe.step();

}


#endif
