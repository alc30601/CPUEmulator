// main.cpp
//
// Remote git repository is below.
// https://github.com/alc30601/DataFlowGraph.git

#include <iostream>
#include <memory>

#include "test_01.hpp"
#include "test_02.hpp"
#include "test_03.hpp"
#include "test_04.hpp"
#include "test_05.hpp"
#include "test_06.hpp"
#include "test_07.hpp"


// class Aaa{ int _a; };
// class Bbb{ std::string _a; };

// template <class T>
// T* test_func(void)
// {
//     T* a = new T();
//     return a;
// }

class Parent
{
public:
    int _a;
    Parent(){}
    Parent(int a) : _a(a)
    {
        std::cout << "Parent" << _a << std::endl;
    }
};

class Child : public Parent
{
public:
    int _b;
    // Child(int b) : Parent(b)
    Child(int b)
    {
        std::cout << "Child" << _a << std::endl;

    }
};

//-----------------------------------------------------------
int main(void)
{
    std::cout << "-- Start - DataFlowGraph -- " << std::endl;


    Child child(3);
    std::cout << child._a << std::endl;



    std::cout << "  -- test01" << std::endl;
    test01();

    std::cout << "  -- test02" << std::endl;
    test02();

    std::cout << "  -- test03" << std::endl;
    test03();

    std::cout << "  -- test04" << std::endl;
    test04();

    std::cout << "  -- test05" << std::endl;
    test05();

    std::cout << "  -- test06" << std::endl;
    test06();

    std::cout << "  -- test07" << std::endl;
    test07();

    std::cout << "... Done." << std::endl;
    return 0;
}

