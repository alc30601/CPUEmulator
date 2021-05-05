// test_04.hpp
// NodeLogics評価
// 論理素子評価

#ifndef __TEST_04_HPP__
#define __TEST_04_HPP__

#include <string>
#include <memory>
#include <vector>
#include <cassert>
#include <tuple>

#include "Edge.hpp"
#include "Node.hpp"
#include "Executor.hpp"
#include "NodeLogics.hpp"

#include "NodeTestBase.hpp"



//-----------------------------------------------------------
void test04(void)
{
    std::cout << "-- test NOT -- " << std::endl;
    std::vector<std::vector<bool>> expectedNot{{T}, {F}};
    test_NtoM_template<NodeNot, bool, bool>(vectorBool1bits, expectedNot);

    std::cout << "-- test AND -- " << std::endl;
    std::vector<std::vector<bool>> expectedAND{{F}, {F}, {F}, {T}};
    test_NtoM_template<NodeAnd, bool, bool>(vectorBool2bits, expectedAND);

    std::cout << "-- test OR -- " << std::endl;
    std::vector<std::vector<bool>> expectedOR{{F}, {T}, {T}, {T}};
    test_NtoM_template<NodeOr, bool, bool>(vectorBool2bits, expectedOR);

    std::cout << "-- test NOR -- " << std::endl;
    std::vector<std::vector<bool>> expectedNOR{{T}, {F}, {F}, {F}};
    test_NtoM_template<NodeNor, bool, bool>(vectorBool2bits, expectedNOR);

    std::cout << "-- test NAND -- " << std::endl;
    std::vector<std::vector<bool>> expectedNAND{{T}, {T}, {T}, {F}};
    test_NtoM_template<NodeNand, bool, bool>(vectorBool2bits, expectedNAND);

    std::cout << "-- test EXOR -- " << std::endl;
    std::vector<std::vector<bool>> expectedEXOR{{F}, {T}, {T}, {F}};
    test_NtoM_template<NodeExor, bool, bool>(vectorBool2bits, expectedEXOR);

    std::cout << "-- test 3 AND -- " << std::endl;
    std::vector<std::vector<bool>> expected3AND{{F}, {F}, {F}, {F}, {F}, {F}, {F}, {T}};
    test_NtoM_template<Node3And, bool, bool>(vectorBool3bits, expected3AND);

    std::cout << "-- test 3 NAND -- " << std::endl;
    std::vector<std::vector<bool>> expected3NAND{{T}, {T}, {T}, {T}, {T}, {T}, {T}, {F}};
    test_NtoM_template<Node3Nand, bool, bool>(vectorBool3bits, expected3NAND);

}


#endif 
