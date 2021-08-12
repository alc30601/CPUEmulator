// NodePseudoParts.hpp
#ifndef __NODEPSEUDOPARTS_HPP__
#define __NODEPSEUDOPARTS_HPP__

#include "Edge.hpp"
#include "Node.hpp"
#include "LogicDefines.hpp"

//-----------------------------------------------------------
// 4 bit ROM
// In-Ports  : A0, A1, A2, A3
// Out-Ports : D0, D1, D2, D3, D4, D5, D6, D7
// 指定されたアドレス値(A0-A3)のROM値を出力(D0-D7)する。
// 
// 参考書ではOE(Output Enable)入力ピンがあり、このピンがHighの場合のみ
// データを出力するようになっている。実際にはトライステート・バッファを出力端子に
// 挿入し、OEの値により出力を制御している。しかし、このエミュレータでは
// トライステーtバッファの模擬は行わなず、競合する出力をマルチプレクサで
// 制御する実現方式とする。
// 
// アドレスは4bit指定で8bitのデータを出力する。
// 実装として、固定データを保持する。
// ノード実行時に、入力アドレスに対応するデータを出力する。
// 本ノードは論理回路から構成されるのではなく、簡易のため疑似実装とする。
// 
// ROM内のデータは以下のように配置される。
// D7 D6 D5 D4 D3 D2 D1 D0
// 資料では例えば、LD B 5 のバイナリ表現が
// 0001 0101 となっている。これはこのROM上では
// 1010 1000 と表現される。
// 
class NodeROM : public Node
{
    bool _data[16][8] = {
        {F, F, F, F, F, F, F, F}, // [ 0]
        {F, F, F, F, F, F, F, T}, // [ 1]
        {F, F, F, F, F, F, T, F}, // [ 2]
        {F, F, F, F, F, T, F, F}, // [ 3]
        {F, F, F, F, T, F, F, F}, // [ 4]
        {F, F, F, T, F, F, F, F}, // [ 5]
        {F, F, T, F, F, F, F, F}, // [ 6]
        {F, T, F, F, F, F, F, F}, // [ 7]
        {T, F, F, F, F, F, F, F}, // [ 8]
        {F, F, F, F, F, F, F, F}, // [ 9]
        {F, F, F, F, F, F, F, F}, // [10]
        {F, F, F, F, F, F, F, F}, // [11]
        {F, F, F, F, F, F, F, F}, // [12]
        {F, F, F, F, F, F, F, F}, // [13]
        {F, F, F, F, F, F, F, F}, // [14]
        {F, F, F, F, F, F, F, F}, // [15]
    };

    int _numAddressBus = 4;

public:
    //-------------------------------------------------------
    NodeROM(void){}

    //-------------------------------------------------------
    // ROMのデータを更新する。
    void setROMData(const bool (&rom)[16][8])
    {
        for(int i=0;i<16;i++){
            for(int j=0;j<8;j++){
                _data[i][j] = rom[i][j];
            }
        }
    }

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        std::vector<Edge*> inEdges = getInEdges();
        unsigned int address = 0x00000000;

        // アドレスバスの値から指定されているアドレス値を得る。
        for(int i=_numAddressBus-1;i>=0;i--){
            bool value = inEdges[i]->value<bool>();
            address = (address << 1) | static_cast<unsigned int>(value);
        }

        // アドレス値のデータを取り出す。
        bool* data = _data[address];

        // データの個数を取得
        int dataSize = sizeof(data) / sizeof(*data);

        // データの各bitをデータバスに出力する。
        std::vector<Edge*> outEdges = getOutEdges();
        for(int i=0;i<dataSize;i++){
            outEdges[i]->setValue(data[i]);
         }
    }

};


//-----------------------------------------------------------
// リセット・クロックノード
// In-Ports  : 
// Out-Ports : RST, CK
class NodeRstClk : public Node{

    bool _reset; // 負論理(F:リセット / T:リセット解除)
    bool _clock; // クロックレベル

public:
    //-------------------------------------------------------
    NodeRstClk(void)
    {
        _reset = F;
        _clock = F;
    }

    //-------------------------------------------------------
    // リセットレベルを設定する。
    // F : リセット
    // T : リセット解除
    void setReset(bool level)
    {
        _reset = level;
    }

    //-------------------------------------------------------
    // 現在クロックがHighならばLowに、LowならばHighにする。
    // クロックは基本、ステップ毎にHigh/Lowを繰り返すが、
    // Executorで１ステップに何度もexecute()を繰り返すことになるため
    // クロック値の更新はexecute()とは別で行う。
    void doClock(void)
    {
        _clock = !_clock;
    }

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();
        std::vector<Edge*> outEdges = getOutEdges();
        outEdges[0]->setValue(_reset);
        outEdges[1]->setValue(_clock);
    }

};


//-----------------------------------------------------------
// Input
// T型のデータがNUM個入力できる疑似入力ポート
// In-Ports  : 
// Out-Ports : Out1, ...
template <typename T, int NUM>
class NodeInput : public Node{

    std::vector<T> _values;

public:
    //-------------------------------------------------------
    NodeInput(void)
    {
        _values.resize(NUM);
    }

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();
        std::vector<Edge*> outEdges = getOutEdges();

        assert(NUM == outEdges.size());
        for(int i=0;i<NUM;i++){
            T value = _values[i];
            outEdges[i]->setValue(value);
        }
    }

    //-------------------------------------------------------
    void input(std::vector<T>& values)
    {
        std::copy(values.begin(), values.end(), _values.begin());
    }
};

//-----------------------------------------------------------
// Output
// T型のデータがNUM個出力できる疑似出力ポート
// In-Ports  : In1, ...
// Out-Ports : 
template <typename T, int NUM>
class NodeOutput : public Node{

    std::vector<T> _values;

public:
    //-------------------------------------------------------
    NodeOutput(void)
    {
        _values.resize(NUM);
    }

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();
        std::vector<Edge*> inEdges = getInEdges();

        assert(NUM == inEdges.size());
        for(int i=0;i<NUM;i++){
            T value = inEdges[i]->value<T>();
            _values[i] = value;
        }
    }

    //-------------------------------------------------------
    std::vector<T> output(void)
    {
        return _values;
    }

};

#endif



