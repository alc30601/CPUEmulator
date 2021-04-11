// NodeComplex.hpp
#ifndef __NODECOMPLEX_HPP__
#define __NODECOMPLEX_HPP__

#include "Edge.hpp"
#include "Node.hpp"
#include "NodeSubSystem.hpp"
#include "GraphBuilder.hpp"

//-----------------------------------------------------------
// GraphBuilderを用いてサブシステムのグラフを構成した結果を
// Nodeとして持つためのクラス。
// サブシステムのNodeはこのクラスを継承したNodeを生成する。
// コンストラクタにて個別の内部グラフを構築する。
class NodeComplex : public NodeSubSystem
{
    GraphBuilder _gb;

    // 自身の入力ポート、出力ポートを持つ。
    // 内部グラフの入出力を保持し、NodeSubSystem化する際に
    // Entry, Exitと紐付ける。
    // std::vector<Ports> _inPortss;
    // Ports _outPorts;


    //-------------------------------------------------------
    // 事前に設定された、入力ポート群、出力ポート群を
    // NodeSusSystemのEntryノード、Exitノードと接続する。
    // 事前にsetInPortss(), setOutPorts()が実行されていることが前提される。
    // void connectEntryExit(void)
    // {
    //     Node* nEntry = getNodeEntry();
    //     Node* nExit = getNodeExit();

    //     auto qnEntry = _gb.quasiNodelize(nEntry);
    //     auto qnExit = _gb.quasiNodelize(nExit);

    //     for(int i=0;i<_inPortss.size();i++){
    //         _gb.outto(Port(qnEntry, i+1), _inPortss[i]);
    //     }

    //     for(int i=0;i<_outPorts.size();i++){
    //         Ports dstPorts{ Port(qnExit, i+1) };
    //         _gb.outto(_outPorts[i], dstPorts);
    //     }
    // }

public:
    //-------------------------------------------------------
    NodeComplex(void){}

    //-------------------------------------------------------
    GraphBuilder& getGraphBuilder(void){ return _gb; }

    //-------------------------------------------------------
    QuasiNode& getEntryNode(void)
    {
        Node* nEntry = getNodeEntry();
        return _gb.quasiNodelize(nEntry);
    }

    //-------------------------------------------------------
    QuasiNode& getExitNode(void)
    {
        Node* nExit = getNodeExit();
        return _gb.quasiNodelize(nExit);
    }

    //-------------------------------------------------------
    // GraphBuilderを用いて本クラスをNodeSubSystemのノード化
    // (入口ノード、出口ノードとの接続)
    // 内部Executorに対してノード、エッジを登録する。
    // 本メソッドの実行により、本クラスが外部から利用できる複合ノードとなる。
    void commit(void)
    {
        // connectEntryExit();

        auto exe = NodeSubSystem::getInnerExecutor();
        auto nodes = _gb.getNodes();
        auto edges = _gb.getEdges();
        exe->addNodes(nodes);
        exe->addEdges(edges);
    }

    //-------------------------------------------------------
    // 本グラフの入力ポートを定義する。
    // 入力ポートは１つのエッジを分岐して複数のPortに分配することもあるため
    // 複数個のPorts(Portのvector)を受け取る形となる。
    // Portsはstd::vector<Port>型であり、Portの複数個の集まりからなる。
    // 即ち、Portの集まりが複数個引数として渡される。
    // ２次元配列のようなもの。
    // template <typename... Args>
    // void setInPortss(Args... ports)
    // {
    //     size_t sizePorts = sizeof...(Args);
    //     Ports array[] = { static_cast<Ports>(ports)... };
    //     std::vector<Ports> inPortss(std::begin(array), std::end(array));

    //     _inPortss = inPortss;
    // }

    //-------------------------------------------------------
    // 本グラフの出力ポートを定義する。
    // 出力ポート自体は分岐しないので入力ポートとは異なり、引数として
    // 単に複数個のPortを受け取る。
    // template <typename... Args>
    // void setOutPorts(Args... ports)
    // {
    //     size_t sizePorts = sizeof...(Args);
    //     Port array[] = { static_cast<Port>(ports)... };
    //     std::vector<Port> outPorts(std::begin(array), std::end(array));
    //     _outPorts = outPorts;
    // }


};

#endif


