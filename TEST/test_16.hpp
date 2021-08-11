// test_16.hpp
// CPUパーツの単体評価
#ifndef __TEST_16_HPP__
#define __TEST_16_HPP__

#include <iostream>

#include "NodeTestBase.hpp"
#include "NodeRegister.hpp"
#include "NodeMultiplexer.hpp"
#include "NodePseudoParts.hpp"


//-----------------------------------------------------------
void test16_01(void)
{
    std::cout << "-- TEST 16-01 ROM --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
        {F, F, F, F, T}, // [ 0]
        {T, F, F, F, T}, // [ 1]
        {F, T, F, F, T}, // [ 2]
        {T, T, F, F, T}, // [ 3]
        {F, F, T, F, T}, // [ 4]
        {T, F, T, F, T}, // [ 5]
        {F, T, T, F, T}, // [ 6]
        {T, T, T, F, T}, // [ 7]
        {F, F, F, T, T}, // [ 8]
    };

    std::vector<std::vector<bool>> expected{
        {F, F, F, F, F, F, F, F},
        {F, F, F, F, F, F, F, T},
        {F, F, F, F, F, F, T, F},
        {F, F, F, F, F, T, F, F},
        {F, F, F, F, T, F, F, F},
        {F, F, F, T, F, F, F, F},
        {F, F, T, F, F, F, F, F},
        {F, T, F, F, F, F, F, F},
        {T, F, F, F, F, F, F, F},
    };

    std::vector<bool> do_asserts{
        true
    };

     test_NtoM_template<NodeROM, bool, bool>(testVector, expected, do_asserts);

}

//-----------------------------------------------------------
void test16_02(void)
{
    std::cout << "-- TEST 16-02 Register --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
        {F, F, T, F, F, F, F}, // [ 0] RESET
        {T, F, T, F, F, F, F}, // [ 1] non-RESET
        {T, T, T, F, F, F, F}, // [ 2] edge-up
        {T, F, T, F, T, F, T}, // [ 3] edge-down
        {T, T, T, F, T, F, T}, // [ 4] edge-up
        {T, F, T, T, F, T, F}, // [ 5] edge-down
        {T, T, T, T, F, T, F}, // [ 6] edge-up
        {T, F, F, T, T, T, T}, // [ 7] edge-down
        {T, T, F, T, T, T, T}, // [ 8] edge-up/LD=0
        {T, F, T, T, T, T, T}, // [ 9] edge-down
        {T, T, T, T, T, T, T}, // [10] edge-up
        {F, T, T, T, T, T, T}, // [11] RESET
    };

    std::vector<std::vector<bool>> expected{
        {F, F, F, F}, // [ 0] 
        {F, F, F, F}, // [ 1] 
        {F, F, F, F}, // [ 2] 0000
        {F, F, F, F}, // [ 3] 
        {F, T, F, T}, // [ 4] 0101
        {F, T, F, T}, // [ 5] 
        {T, F, T, F}, // [ 6] 1010
        {T, F, T, F}, // [ 7] 
        {T, F, T, F}, // [ 8] 1010(stay)
        {T, F, T, F}, // [ 9] 
        {T, T, T, T}, // [10] 1111
        {F, F, F, F}, // [11] 0000
    };

    std::vector<bool> do_asserts{
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
    };

     test_NtoM_template<NodeRegister, bool, bool>(testVector, expected, do_asserts);

}

//-----------------------------------------------------------
void test16_03_01(void)
{
    std::cout << "-- TEST 16-03-01 Node3Selector --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
    //   S0, S1, S2, D0, D1, D2
        {T,  F,  F,  F,  F,  F}, // [ 0]
        {T,  F,  F,  T,  F,  F}, // [ 1]
        {F,  T,  F,  F,  F,  F}, // [ 2]
        {F,  T,  F,  F,  T,  F}, // [ 3]
        {F,  F,  T,  F,  F,  F}, // [ 4]
        {F,  F,  T,  F,  F,  T}, // [ 5]
    };

    std::vector<std::vector<bool>> expected{
    //   Q
        {F}, // [ 0] 
        {T}, // [ 1] 
        {F}, // [ 2] 
        {T}, // [ 3] 
        {F}, // [ 4] 
        {T}, // [ 5] 
    };

    std::vector<bool> do_asserts{
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
    };

     test_NtoM_template<Node3Selector, bool, bool>(testVector, expected, do_asserts);
}

//-----------------------------------------------------------
void test16_03_02(void)
{
    std::cout << "-- TEST 16-03-01 NodeUpLdSelector --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
    //   UP, LD
        {F,  F}, // [ 0]
        {T,  F}, // [ 1]
        {F,  T}, // [ 2]
        {T,  T}, // [ 3]
    };

    std::vector<std::vector<bool>> expected{
    //   HOLD, UP_EN, LD
        {T,    F,     F}, // [ 0] 
        {F,    T,     F}, // [ 1] 
        {F,    F,     T}, // [ 2] 
        {F,    F,     T}, // [ 3] 
    };

    std::vector<bool> do_asserts{
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
    };

     test_NtoM_template<NodeUpLdSelector, bool, bool>(testVector, expected, do_asserts);
}

//-----------------------------------------------------------
void test16_03(void)
{
    std::cout << "-- TEST 16-03 PC --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
    //   RST, CK, LD, UP, D0, D1, D2, D3
        {F,   F,  F,  F,  F,  F,  F,  F}, // [ 0] RESET
        {T,   F,  F,  F,  F,  F,  F,  F}, // [ 1] LD/UP = 0/0
        {T,   T,  F,  F,  F,  F,  F,  F}, // [ 2] clock-up
        {T,   F,  F,  T,  F,  F,  F,  F}, // [ 3] LD/UP = 0/1
        {T,   T,  F,  T,  F,  F,  F,  F}, // [ 4] clock-up
        {T,   F,  F,  T,  F,  F,  F,  F}, // [ 5] LD/UP = 0/1
        {T,   T,  F,  T,  F,  F,  F,  F}, // [ 6] clock-up
        {T,   F,  F,  T,  F,  F,  F,  F}, // [ 7] LD/UP = 0/1
        {T,   T,  F,  T,  F,  F,  F,  F}, // [ 8] clock-up
        {T,   F,  F,  T,  F,  F,  F,  F}, // [ 9] LD/UP = 0/1
        {T,   T,  F,  T,  F,  F,  F,  F}, // [10] clock-up
        {T,   F,  T,  F,  F,  F,  T,  T}, // [11] LD/UP = 1/0
        {T,   T,  T,  F,  F,  F,  T,  T}, // [12] clock-up
        {T,   F,  F,  F,  F,  F,  T,  T}, // [13] LD/UP = 0/0
        {T,   T,  F,  F,  F,  F,  T,  T}, // [14] clock-up
    };

    std::vector<std::vector<bool>> expected{
    //   Q0, Q1, Q2, Q3
        {F,  F,  F,  F}, // [ 0] 
        {F,  F,  F,  F}, // [ 1]
        {F,  F,  F,  F}, // [ 2] Q(0)
        {F,  F,  F,  F}, // [ 3] 
        {T,  F,  F,  F}, // [ 4] Q+1(1)
        {T,  F,  F,  F}, // [ 5] 
        {F,  T,  F,  F}, // [ 6] Q+1(2)
        {F,  T,  F,  F}, // [ 7] 
        {T,  T,  F,  F}, // [ 8] Q+1(3)
        {T,  T,  F,  F}, // [ 9]
        {F,  F,  T,  F}, // [10] Q+1(4)
        {F,  F,  T,  F}, // [11] 
        {F,  F,  T,  T}, // [12] Q <- D(12) 
        {F,  F,  T,  T}, // [13]
        {F,  F,  T,  T}, // [14] stay
    };

    std::vector<bool> do_asserts{
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
    };

     test_NtoM_template<NodePC, bool, bool>(testVector, expected, do_asserts);
}

//-----------------------------------------------------------
void test16_04_01(void)
{
    std::cout << "-- TEST 16-04 NodeZeroDetector --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
    //   A  B  C  D
        {F, F, F, F }, // [ 0] All-0
        {T, F, F, F }, // [ 1] A=1
        {F, T, F, F }, // [ 0] B=1
        {F, F, T, F }, // [ 0] C=1
        {F, F, F, T }, // [ 0] D=1
        {T, T, T, T }, // [ 0] All-1
    };

    std::vector<std::vector<bool>> expected{
    //   1   2   3   4   5  6
    //   Q0, Q1, Q2, Q3, C, Z
        {T}, // [ 0] 
        {F}, // [ 1] 
        {F}, // [ 2] 
        {F}, // [ 3] 
        {F}, // [ 4] 
        {F}, // [ 5] 
    };

    std::vector<bool> do_asserts{
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
    };

     test_NtoM_template<NodeZeroDetector, bool, bool>(testVector, expected, do_asserts);
}


//-----------------------------------------------------------
void test16_04(void)
{
    std::cout << "-- TEST 16-04 ALU --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
    //   1    2   3   4   5      6      7   8   9   10    11  12  13  14
    //   RST, CK, LD, AS, MUX_A, MUX_B, A0, A1, A2, A3,   B0, B1, B2, B3
        {F,   F,  T,  F,  F,     F,     F,  F,  F,  F,    F,  F,  F,  F }, // [ 0] 初期リセット
        {T,   F,  T,  F,  F,     F,     F,  F,  F,  F,    F,  F,  F,  F }, // [ 1] リセット解除
        {T,   F,  T,  F,  F,     F,     T,  T,  F,  F,    T,  T,  T,  F }, // [ 2] A=3, B=7/但しMUX=F
        {T,   T,  T,  F,  F,     F,     T,  T,  F,  F,    T,  T,  T,  F }, // [ 3] A=3, B=7/但しMUX=F, Clock-Up
        {T,   F,  T,  F,  T,     F,     T,  T,  F,  F,    T,  T,  T,  F }, // [ 4] A=3, B=7/MUX_A=T
        {T,   F,  T,  F,  F,     T,     T,  T,  F,  F,    T,  T,  T,  F }, // [ 5] A=3, B=7/MUX_B=T
        {T,   F,  T,  F,  T,     T,     T,  T,  F,  F,    T,  T,  T,  F }, // [ 6] A=3, B=7/MUX_A=T,MUX_B=T
        {T,   T,  T,  F,  T,     T,     T,  T,  F,  F,    T,  T,  T,  F }, // [ 7] Clock-Up
        {T,   F,  T,  F,  T,     T,     F,  F,  T,  T,    F,  T,  T,  F }, // [ 8] A=12, B=6/MUX_A=T,MUX_B=T
        {T,   T,  T,  F,  T,     T,     F,  F,  T,  T,    F,  T,  T,  F }, // [ 9] Clock-Up
        {T,   F,  T,  T,  T,     T,     F,  F,  T,  T,    F,  T,  T,  F }, // [10] Clock-down
        {T,   T,  T,  T,  T,     T,     F,  F,  T,  T,    F,  T,  T,  F }, // [11] AS=T(引き算)
        {T,   F,  T,  T,  T,     T,     F,  T,  F,  F,    T,  F,  T,  F }, // [12] A=2, B=5
        {T,   T,  T,  T,  T,     T,     F,  T,  F,  F,    T,  F,  T,  F }, // [13] Clock-Up
    };

    std::vector<std::vector<bool>> expected{
    //   1   2   3   4    5  6
    //   Q0, Q1, Q2, Q3,  C, Z
        {F,  F,  F,  F,   F, F }, // [ 0] 入力強制0
        {F,  F,  F,  F,   F, F }, // [ 1] 0維持
        {F,  F,  F,  F,   F, F }, // [ 2] 0維持
        {F,  F,  F,  F,   F, T }, // [ 3] 0維持, C=F, Z=T クロックアップでフラグ更新。0なのでZフラグが立つ。
        {T,  T,  F,  F,   F, T }, // [ 4] A値出力=3 MUX_Aのみ立っているのでAの値を出力
        {T,  T,  T,  F,   F, T }, // [ 5] B値出力=7 MUX_Bのみ立っているのでBの値を出力
        {F,  T,  F,  T,   F, T }, // [ 6] A+B=10 MUX_A,MUX_B両方立っており、ASが＋を意味するのでA+Bを出力
        {F,  T,  F,  T,   F, F }, // [ 7] C=F, Z=F クロックアップでフラグ更新。0以外(10)なのでZフラグが落ちる。
        {F,  T,  F,  F,   F, F }, // [ 8] 12+6=18=10010なので4bitは0010
        {F,  T,  F,  F,   T, F }, // [ 9] 12+6=18でキャリーオーバするのでCが立つ。
        {F,  T,  T,  F,   T, F }, // [10] 引き算12-6=6 フラグはそのまま
        {F,  T,  T,  F,   T, F }, // [11] 引き算の結果が正なのでCが立つ。
        {T,  F,  T,  T,   T, F }, // [12] 2-5=-3(1101)
        {T,  F,  T,  T,   F, F }, // [13] 引き算の結果が負なのでCが落ちる。
    };

    std::vector<bool> do_asserts{
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
    };

     test_NtoM_template<NodeALU, bool, bool>(testVector, expected, do_asserts);
}

//-----------------------------------------------------------
void test16_05(void)
{
    std::cout << "-- TEST 16-04 ALU --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
    //   1    2   3     4
    //   RST, CK, HALT, PC_LD
        {F,   F,  T,    F    }, // [ 0] 初期リセット
        {T,   F,  T,    F    }, // [ 1] リセット解除
        {T,   T,  T,    F    }, // [ 2] clock-up (PC_LD=0)
        {T,   F,  T,    F    }, // [ 3] clock-down
        {T,   T,  T,    F    }, // [ 4] clock-up (PC_LD=0)
        {T,   F,  T,    F    }, // [ 5] clock-down
        {T,   T,  T,    F    }, // [ 6] clock-up (PC_LD=0)
        {T,   F,  T,    F    }, // [ 7] clock-down
        {T,   T,  T,    F    }, // [ 8] clock-up (PC_LD=0)
        {T,   F,  T,    F    }, // [ 9] clock-down

        {T,   T,  T,    T    }, // [10] clock-up (PC_LD=1) ←ジャンプ命令
        {T,   F,  T,    F    }, // [11] clock-down
        {T,   T,  T,    F    }, // [12] clock-up (PC_LD=0)
        {T,   F,  T,    F    }, // [13] clock-down


    };

    std::vector<std::vector<bool>> expected{
    //   1    2   3      4
    //   RST, CK, PC_UP, LD_EN
        {F,   F,  F,     T      }, // [ 0] 入力強制0
        {T,   F,  F,     T      }, // [ 1] リセット解除だけなので出力変わらず
        {T,   T,  T,     F      }, // [ 2] 状態0→1
        {T,   F,  T,     F      }, // [ 3] 
        {T,   T,  F,     T      }, // [ 4] 状態1→0
        {T,   F,  F,     T      }, // [ 5] 
        {T,   T,  T,     F      }, // [ 6] 状態0→1
        {T,   F,  T,     F      }, // [ 7] 
        {T,   T,  F,     T      }, // [ 8] 状態1→0
        {T,   F,  F,     T      }, // [ 9] 

        {T,   T,  F,     T      }, // [10] 状態0→0 ←状態維持
        {T,   F,  F,     T      }, // [11] 
        {T,   T,  T,     F      }, // [12] 状態0→1
        {T,   F,  T,     F      }, // [13] 

    };

    std::vector<bool> do_asserts{
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
    };

     test_NtoM_template<NodeStateMachine, bool, bool>(testVector, expected, do_asserts);
}


//-----------------------------------------------------------
void test16_06(void)
{
    std::cout << "-- TEST 16-04 ID --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
    //   1   2   3   4   5  6  7
    //   D0, D1, D2, D3, C, Z, LD_EN
        {F,  F,  F,  F,  F, F, T    }, // [ 1] LD A
        {T,  F,  F,  F,  F, F, T    }, // [ 2] LD B
        {F,  T,  F,  F,  F, F, T    }, // [ 3] LD A B
        {T,  T,  F,  F,  F, F, T    }, // [ 4] LD B A
        {F,  F,  T,  F,  F, F, T    }, // [ 5] ADD A B
        {T,  F,  T,  F,  F, F, T    }, // [ 6] SUB A B
        {F,  T,  T,  F,  F, F, T    }, // [ 7] ADD A [Data]
        {T,  T,  T,  F,  F, F, T    }, // [ 8] SUB A [Data]
        {F,  F,  F,  T,  F, F, T    }, // [ 9] OUT A
        {T,  F,  F,  T,  F, F, T    }, // [10] OUT B
        {F,  T,  F,  T,  F, F, T    }, // [11] OUT [Data]
        {T,  T,  F,  T,  F, F, T    }, // [12] IN A
        {F,  F,  T,  T,  F, F, T    }, // [13] JUMP [Address]
        {T,  F,  T,  T,  F, F, T    }, // [14] JNC [Address]
        {F,  T,  T,  T,  F, F, T    }, // [15] JNZ [Address]
        {T,  T,  T,  T,  F, F, T    }, // [16] HALT

        {T,  F,  T,  T,  T, F, T    }, // [17] JNC [Address] (C=T)
        {F,  T,  T,  T,  F, T, T    }, // [18] JNZ [Address] (Z=T)

        {F,  F,  T,  F,  F, F, F    }, // [19] ADD A B (LD_EN=F)
        {T,  F,  F,  F,  F, F, F    }, // [20] LD B (LD_EN=F)
        {F,  F,  F,  T,  F, F, F    }, // [21] OUT A (LD_EN=F)
    };

    std::vector<std::vector<bool>> expected{
    //   1       2      3     4       5       6       7          8          9         10        11        12      13     14 15 16 17 18 19
    //   ROM_OE, PC_LD, HALT, ALU_LD, ALU_OE, ALU_AS, ALU_MUX_A, ALU_MUX_B, A_REG_LD, B_REG_LD, B_REG_OE, OUT_LD, IN_OE  PROBE1-6
        {T,      F,     T,    F,      T,      F,      F,         T,         T,        F,        F,        F,      F    }, // [ 1] LD A [Data]
        {T,      F,     T,    F,      T,      F,      F,         T,         F,        T,        F,        F,      F    }, // [ 2] LD B [Data]
        {F,      F,     T,    F,      T,      F,      F,         T,         T,        F,        T,        F,      F    }, // [ 3] LD A B
        {F,      F,     T,    F,      T,      F,      T,         F,         F,        T,        T,        F,      F    }, // [ 4] LD B A
        {F,      F,     T,    T,      T,      F,      T,         T,         T,        F,        T,        F,      F    }, // [ 5] ADD A B
        {F,      F,     T,    T,      T,      T,      T,         T,         T,        F,        T,        F,      F    }, // [ 6] SUB A B
        {T,      F,     T,    T,      T,      F,      T,         T,         T,        F,        F,        F,      F    }, // [ 7] ADD A [Data]
        {T,      F,     T,    T,      T,      T,      T,         T,         T,        F,        F,        F,      F    }, // [ 8] SUB A [Data]
        {F,      F,     T,    F,      T,      F,      T,         F,         F,        F,        T,        T,      F    }, // [ 9] OUT A
        {F,      F,     T,    F,      T,      F,      F,         T,         F,        F,        T,        T,      F    }, // [10] OUT B
        {T,      F,     T,    F,      T,      F,      F,         T,         F,        F,        F,        T,      F    }, // [11] OUT [Data]
        {F,      F,     T,    F,      F,      F,      F,         T,         T,        F,        T,        F,      T    }, // [12] IN A
        {T,      T,     T,    F,      T,      F,      F,         T,         F,        F,        F,        F,      F    }, // [13] JUMP [Address]
        {T,      T,     T,    F,      T,      F,      F,         T,         F,        F,        F,        F,      F    }, // [14] JNC [Address]
        {T,      T,     T,    F,      T,      F,      F,         T,         F,        F,        F,        F,      F    }, // [15] JNZ [Address]
        {T,      F,     F,    F,      T,      F,      F,         T,         F,        F,        F,        F,      F    }, // [16] HALT

        {T,      F,     T,    F,      T,      F,      F,         T,         F,        F,        F,        F,      F    }, // [17] JNC [Address] (C=T)
        {T,      F,     T,    F,      T,      F,      F,         T,         F,        F,        F,        F,      F    }, // [18] JNZ [Address] (Z=T)

        {F,      F,     T,    F,      T,      F,      T,         T,         F,        F,        T,        F,      F    }, // [19] ADD A B (LD_EN=F)
        {T,      F,     T,    F,      T,      F,      F,         T,         F,        F,        F,        F,      F    }, // [20] LD B [Data] (LD_EN=F)
        {F,      F,     T,    F,      T,      F,      T,         F,         F,        F,        T,        F,      F    }, // [21] OUT A (LD_EN=F)
    };



    std::vector<bool> do_asserts{
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
    };

     test_NtoM_template<NodeID, bool, bool>(testVector, expected, do_asserts);
}


//-----------------------------------------------------------
void test16_07(void)
{
    std::cout << "-- TEST 16-07 4bit MUX --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
    //   1    2   3   4   5   6   7   8   9
    //   sel, A0, A1, A2, A3, B0, B1, B2, B3
        {F,   F,  T,  F,  T,  T,  F,  T,  F  },
        {T,   F,  T,  F,  T,  T,  F,  T,  F  },
    };

    std::vector<std::vector<bool>> expected{
    //   1   2   3   4
    //   Y0, Y1, Y2, Y3
        {F,  T,  F,  T},
        {T,  F,  T,  F },
    };



    std::vector<bool> do_asserts{
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
    };

     test_NtoM_template<Node4bitMultiplexer, bool, bool>(testVector, expected, do_asserts);
}


//-----------------------------------------------------------
void test16(void)
{
    test16_01();
    test16_02();
    test16_03_01();
    test16_03_02();
    test16_03();
    test16_04_01();
    test16_04();
    test16_05();
    test16_06();
    test16_07();
}


#endif


