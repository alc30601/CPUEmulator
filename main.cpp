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
#include "test_08.hpp"



//////////////////////////////////////////////////////////////////////////////////

//------------------------
class DummyClass
{
public:
    std::string _name;
    DummyClass(std::string name) : _name(name){}
};

using Classes = std::vector<DummyClass>;

//------------------------
template <typename... Args>
void sub(Args... ports)
{
    size_t sizePorts = sizeof...(Args);
    // Classes array[] = { static_cast<Classes>(ports)... };
    // std::vector<DummyClass> inPorts(std::begin(array), std::end(array));

     std::cout << "sub" << std::endl;
}

//------------------------
void test(void)
{
    DummyClass a1("This is A1");
    DummyClass a2("This is A2");
    DummyClass a3("This is A3");
    DummyClass b1("This is B1");
    DummyClass b2("This is B2");
    DummyClass c1("This is C1");

    Classes clsesA{a1, a2, a3};
    Classes clsesB{b1, b2};
    Classes clsesC{c1};
    sub(clsesA, clsesB, clsesC);
}

//////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------
int main(void)
{
    std::cout << "-- Start - DataFlowGraph -- " << std::endl;

    test();

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

    std::cout << "  -- test08" << std::endl;
    test08();

    std::cout << "... Done." << std::endl;
    return 0;
}

