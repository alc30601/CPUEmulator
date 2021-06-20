// exp_02.hpp
#ifndef __EXP_02_HPP__
#define __EXP_02_HPP__

#include <string>
#include <iostream>
#include <vector>


/////////////////////////////////////////////////////////////
// erase()の振る舞い確認

//-----------------------------------------------------------
class Object
{
    int _value;
    std::string _name;
public:
    //-------------------------------------------------------
    Object(int value, std::string name)
    {
        _value = value;
        _name = name;

        std::cout << "Object(" << _name << ":" << std::to_string(_value) + ") created" << std::endl;
    }

    //-------------------------------------------------------
    ~Object(void)
    {
        std::cout << "Object(" << _name << ":" << std::to_string(_value) + ") deleted" << std::endl;
    }

    //-------------------------------------------------------
    void print(void)
    {
        std::cout << "Object(" << _name + ":" << std::to_string(_value) + ")" << std::endl;
    }
};

//-----------------------------------------------------------
void exp_02_01(void)
{
    Object* obj1 = new Object(1, "red");
    Object* obj2 = new Object(2, "yellow");
    Object* obj3 = new Object(3, "blue");

    std::vector<Object*> objsA = {obj1, obj2, obj3};
    std::vector<Object*> objsB = {obj1, obj2, obj3};

    for(auto it = objsA.begin(); it != objsA.end();){
        if(*it == obj2){
            it = objsA.erase(it);
        }
        else{
            ++it;
        }
    }

    for(auto obj : objsA){
        obj->print();
    }

    for(auto obj : objsB){
        obj->print();
    }
}

/////////////////////////////////////////////////////////////
// vectorの参照からのコピー

//-----------------------------------------------------------
void exp_02_02_sub(std::vector<Object*>& objs)
{
    std::vector<Object*> copiedObjs = objs;
    copiedObjs.clear();
}

//-----------------------------------------------------------
void exp_02_02(void)
{
    Object* obj1 = new Object(1, "red");
    Object* obj2 = new Object(2, "yellow");
    Object* obj3 = new Object(3, "blue");

    std::vector<Object*> objsA = {obj1, obj2, obj3};
    std::vector<Object*> objsB = {obj1, obj2, obj3};

    exp_02_02_sub(objsA);

    for(auto obj : objsA){
        obj->print();
    }

}

/////////////////////////////////////////////////////////////
// std::set

//-----------------------------------------------------------
void exp_02_03(void)
{
    std::set<int> intSet;

    intSet.insert(1);
    intSet.insert(2);
    intSet.insert(3);
    intSet.insert(2);
    intSet.insert(1);

    auto result  = std::find(intSet.begin(), intSet.end(), 2);
    if(result == intSet.end()){
        std::cout << "Not Found" << std::endl;
    }
    else{
        std::cout << "Found : " <<  std::to_string(*result) << std::endl;
    }

}

//-----------------------------------------------------------
void exp_02(void)
{
    exp_02_01();
    exp_02_02();
    exp_02_03();
}

#endif
