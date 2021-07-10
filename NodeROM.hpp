// NodeROM.hpp
#ifndef __NODEROM_HPP__
#define __NODEROM_HPP__

#include "Node.hpp"
#include "LogicDefines.hpp"

//-----------------------------------------------------------
// 4 bit register
// In-Ports  : A0, A1, A2, A3, OE
// Out-Ports : D0, D1, D2, D3, D4, D5, D6, D7
// 指定されたアドレス値(A0-A3)のROM値を出力(D0-D7)する。
// アドレスは4bit指定で8bitのデータを出力する。
// 実装として、固定データを保持する。
// → ゆくゆくは、ファイルからCSV形式のデータを読み込むなどにしたい。
// ノード実行時に、入力アドレスに対応するデータを出力する。
// 本ノードは論理回路から構成されるのではなく、簡易のため疑似実装とする。
class NodeROM : public Node
{
    bool _data[16][8] = {
        {F, F, F, F, F, F, F, F},
        {F, F, F, F, F, F, F, T},
        {F, F, F, F, F, F, T, F},
        {F, F, F, F, F, T, F, F},
        {F, F, F, F, T, F, F, F},
        {F, F, F, T, F, F, F, F},
        {F, F, T, F, F, F, F, F},
        {F, T, F, F, F, F, F, F},
        {T, F, F, F, F, F, F, F},
        {F, F, F, F, F, F, F, F},
        {F, F, F, F, F, F, F, F},
        {F, F, F, F, F, F, F, F},
        {F, F, F, F, F, F, F, F},
        {F, F, F, F, F, F, F, F},
        {F, F, F, F, F, F, F, F},
        {F, F, F, F, F, F, F, F},
    };

public:
    //-------------------------------------------------------
    NodeROM(void)
    {
    }

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        // アドレスバス数を求める。
        // 入力Edgeの最後はOEなので入力Edge数-1をアドレスバス数とみなす。
        int numAddressBus = _inEdges.size() - 1;
        std::vector<Edge*> inEdges = getInEdges();
        unsigned int address = 0x00000000;

        // アドレスバスの値から指定されているアドレス値を得る。
        for(int i=0;i<numAddressBus;i++){
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

#endif

