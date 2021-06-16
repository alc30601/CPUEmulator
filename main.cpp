// main.cpp
//
// Remote git repository is below.
// https://github.com/alc30601/DataFlowGraph.git

#include <iostream>
#include <memory>

// #include "test_01.hpp"
// #include "test_02.hpp"
// #include "test_03.hpp"
#include "test_04.hpp"
// #include "test_05.hpp"
// #include "test_06.hpp"
// #include "test_07.hpp"
// #include "test_08.hpp"
// #include "test_09.hpp"
#include "test_10.hpp"
#include "test_11.hpp"
#include "test_12.hpp"
#include "test_13.hpp"
#include "test_14.hpp"




///////////////////////////////////////////////////////////
// ２つのstd::anyの値の比較を行う。
bool compareAB(std::any a, std::any b)
{
    bool result = false;
    if((a.type() == typeid(int)) || (b.type() == typeid(int))){
        auto aa = std::any_cast<int>(a);
        auto bb = std::any_cast<int>(b);

        if(aa == bb){
            std::cout << "same integer" << std::endl;
            return true;
        }
    }
    else if((a.type() == typeid(std::string)) || (b.type() == typeid(std::string))){
        auto aa = std::any_cast<std::string>(a);
        auto bb = std::any_cast<std::string>(b);

        if(aa == bb){
            std::cout << "same string" << std::endl;
            return true;
        }
    }
    return result;
}

void test1(void)
{
    std::cout << "-- test -- " << std::endl;

    std::any a;
    std::any b;

    a = 3;
    b = 4;
    compareAB(a, b);

    a = std::string("Hello");
    b = std::string("Bye");
    compareAB(a, b);


}

///////////////////////////////////////////////////////////
// 型情報をテンプレートとして指定する。
// この場合テンプレート関数は型ごとに別物となってしまう。
template <typename T>
void test2_sub(T a)
{
    T value = T();
    std::cout << value << std::endl;

}

void test2(void)
{
    std::cout << "-- test2 -- " << std::endl;
    test2_sub<int>(0);
    test2_sub<double>(3.14);
    test2_sub<bool>(true);
}

///////////////////////////////////////////////////////////
// 型情報を関数引数として渡す。
#include <typeinfo>


class MyClassForTest3
{
    int _a;
};

void test3_sub(std::type_info const &ti)
{
    std::cout << ti.name() << std::endl;

    if(ti == typeid(int)){
        int a =int();
        std::cout << a << std::endl;
    }
    else if(ti == typeid(double)){
        double a = double();
        std::cout << a << std::endl;
    }
    else if(ti == typeid(bool)){
        bool a = bool();
        std::cout << a << std::endl;
    }

}

void test3(void)
{
    std::cout << "-- test3 -- " << std::endl;
    auto name = typeid(int).name();
    // auto ti = typeid(int);
    std::type_info const & t_int = typeid(int);
    test3_sub(t_int);

    test3_sub(typeid(double));
    test3_sub(typeid(bool));
    test3_sub(typeid(MyClassForTest3));
}

///////////////////////////////////////////////////////////
// サブクラス化されたクラスの型情報
class SomeZ
{
     int z; 
public:
    virtual ~SomeZ() = default;
};
class SomeA : public SomeZ{ int a; };
class SomeB : public SomeA{ int b; };
class SomeC : public SomeZ{ int c; };

bool checkIfSomeA(SomeZ* sz)
{
    bool result;

    SomeA* psa = dynamic_cast<SomeA*>(sz);
    if(psa != nullptr){
        std::cout << "sb is SomeA" << std::endl;
        result = true;
    }
    else{
        std::cout << "sb is not SomeA" << std::endl;
        result = false;
    }
    return result;
}

void test4(void)
{
    SomeB sb;
    SomeC sc;

    // SomeA* pas1 = dynamic_cast<SomeA*>(&sb);
    // SomeA* pas2 = dynamic_cast<SomeA*>(&sc);

    checkIfSomeA(&sb);
    checkIfSomeA(&sc);

}


//-----------------------------------------------------------
int main(void)
{
    std::cout << "-- Start - DataFlowGraph -- " << std::endl;

    // test1();
    // test2();
    // test3();
    // test4();


    std::cout << "  -- test13" << std::endl;
    test13();

    std::cout << "  -- test04" << std::endl;
    test04();

    std::cout << "  -- test14" << std::endl;
    test14();

    std::cout << "  -- test12" << std::endl;
    test12();

    std::cout << "... Done." << std::endl;
    return 0;
}


