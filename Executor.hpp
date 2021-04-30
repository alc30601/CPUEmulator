// Executor.hpp
#ifndef __EXECUTOR_HPP__
#define __EXECUTOR_HPP__

#include <iostream>
#include <memory>
#include <typeinfo>
#include <vector>
#include <iterator>
#include <any>
#include <algorithm>

#include "Node.hpp"
#include "Edge.hpp"


//-----------------------------------------------------------
// Executor実行クラスのベースクラス
// 純仮想クラス
class Executor
{
protected:
    Node* _startNode;
    std::vector<Node*> _nodes;
    std::vector<Edge*> _edges;

    //-------------------------------------------------------
    // ノード一覧のうち、未実行のノード数を計算する。
    int countUnExecutedNodes(void)
    {
        int rest = 0;
        for(auto node : _nodes){

            if(node->isExecuted() == false){
                rest++;
            }
        }
        return rest;
    }

   //-------------------------------------------------------
    void clearNodeStatus(void)
    {
        for(auto node : _nodes){
            node->clearStatus();
        }
    }

    //-------------------------------------------------------
    void clearEdgeStatus(void)
    {
        for(auto edge : _edges){
            edge->clearStatus();
        }
    }

public:
    //-------------------------------------------------------
    virtual void step(void) = 0;

    //-------------------------------------------------------
    // 開始Nodeを登録する。
    void setStartNode(Node* startNode)
    {
        _startNode = startNode;
    }

    //-------------------------------------------------------
    // Nodeを一括追加する。
    void addNodes(std::vector<Node*>& nodes)
    {
        std::copy(nodes.begin(), nodes.end(), std::back_inserter(_nodes));
    }

    //-------------------------------------------------------
    // edgeを一括追加する。
    void addEdges(std::vector<Edge*>& edges)
    {
        std::copy(edges.begin(), edges.end(), std::back_inserter(_edges));
    }

};


//-----------------------------------------------------------
// 再帰を用いた深さ優先探索によるノード巡回実行
class ExecutorDepthFirstTraversal : public Executor
{
    Node* _startNode;

    //-------------------------------------------------------
    // 再帰を用いた深さ優先探索によるノード巡回実行
    // 効率は比較的いいが、深さ優先探索のため木構造のグラフにしか対応しない。
    void stepRecursive(Node* node)
    {
        // if this node already executed, do nothing
        if(node->isExecuted() == true){
            return;
        }
        
        // if datas are not ready yet, do nothing
        if(node->isInputDataCompleted() == false){
            return;
        }

        // do this node
        node->execute();

        // traverse out edges and nodes, then execute each node recursively
        for(auto edge : node->getOutEdges()){
            for(auto child_node : edge->getOutNodes()){
                stepRecursive(child_node);
            }
        }
    }

public:
    //-------------------------------------------------------
    void step(void)
    {
        // _startNodeから始めてグラフをたどる。
        // あるNodeからその先に行くにはNodeクラスのgetOutputEdge()によりつながるEdgeを取得する。
        // あるEdgeからその先のNodeを見つけるにはEdgeのメソッドgetoutputNode()によりNode一覧を取得する。

        // 個々のノードに到達したらそのノードの処理を実行する(execute())。
        // 但し、到達したNodeにまだデータが到達せずに処理が実行できない場合がある。
        // その判定のため、そのNodeに入力されるEdge一覧を取得し、全てのEdgeでデータが到達しているか
        // をチェックする。

        stepRecursive(_startNode);
        clearNodeStatus();
        clearEdgeStatus();
    }
};


//-----------------------------------------------------------
// ノードリストを全ノード実行済まで何度もなめることによる実行
class ExecutorNodeListTraversal : public Executor
{
    //-------------------------------------------------------
    // ノード一覧をなめ、実行できるものから実行していく。
    // 実行していないノードが無くなるまで何度も繰り返す。
    // 探索効率は悪いが、木構造以外のグラフでも全ノード実行可能
    // void doStep(void)
    // {
    //     auto rest = _nodes.size();
    //     while(rest > 0){
    //         doOneRound();
    //         rest = countUnExecutedNodes();
    //     }
    // }

    //-------------------------------------------------------
    // ノード一覧を一巡して、実行出来るノードは実行する。
    void doOneRound(void)
    {
        for(auto node : _nodes){

            // if this node already executed, do nothing
            if(node->isExecuted() == true){
                continue;
            }
            
            // if datas are not ready yet, do nothing
            if(node->isInputDataCompleted() == false){
                continue;
            }

            // do this node
            node->execute();
        }
    }


public:
    //-------------------------------------------------------
    // ノード一覧をなめ、実行できるものから実行していく。
    // 実行していないノードが無くなるまで何度も繰り返す。
    // 探索効率は悪いが、木構造以外のグラフでも全ノード実行可能
    void step(void)
    {
        auto rest = _nodes.size();
        while(rest > 0){
            doOneRound();
            rest = countUnExecutedNodes();
        }

        // doStep();

        clearNodeStatus();
        clearEdgeStatus();
    }
};


//-----------------------------------------------------------
// Edgeの状態が安定するまで実行を繰り返すExecutor
class ExecutorStableEdge : public Executor
{
    //-------------------------------------------------------
    // 現在の全Edgeの保持する値を集める。
    std::vector<std::any> edgeSnapShot(void)
    {
        std::vector<std::any> snapShot;
        for(auto edge : _edges){
            snapShot.push_back(edge->getValue());
        }
        return snapShot;
    }

    //-------------------------------------------------------
    // 全Nodeを実行する。
    void doOneRound(void)
    {
        for(auto node : _nodes){
            // このExecutorでは入力Edgeが値を持っているか否かに関わらず
            // Nodeの処理を実行する。
            // このExecutorで実行されるNodeは入力Edgeに
            // 値が無い場合の処理を実装する必要がある。
            node->execute();
        }
    }

    //-------------------------------------------------------
    // 全Nodeを実行する。
    void executeAllNode(void)
    {
        auto rest = _nodes.size();
        while(rest > 0){
            doOneRound();
            rest = countUnExecutedNodes();
            clearNodeStatus();
        }
    }

    //-------------------------------------------------------
    // ２つのstd::anyの値を比較する。
    // 何れかに値が設定されている場合は値の比較を行い一致判定を行う。
    // 両方とも値が設定されていない場合も一致とみなす。
    // 
    // 非常に醜い関数。
    // 使用される可能性のある型をすべて列挙している。
    // std::anyの型を知ることなく比較できないものか。。。
    bool compareValue(std::any& v1, std::any& v2)
    {
        bool result = true;

        // 両方のstd::anyに値が設定されている場合、値の比較を行う。
        if((v1.has_value() == true) && (v2.has_value() == true)){
            // 以下可能性のある型での比較を並べる。
            if((v1.type() == typeid(bool)) || (v2.type() == typeid(bool))){
                auto vv1 = std::any_cast<bool>(v1);
                auto vv2 = std::any_cast<bool>(v2);
                if(vv1 != vv2){
                    result = false;
                }
            }
            else if((v1.type() == typeid(int)) || (v2.type() == typeid(int))){
                auto vv1 = std::any_cast<int>(v1);
                auto vv2 = std::any_cast<int>(v2);
                if(vv1 != vv2){
                    result = false;
                }
            }
            else if((v1.type() == typeid(double)) || (v2.type() == typeid(double))){
                auto vv1 = std::any_cast<double>(v1);
                auto vv2 = std::any_cast<double>(v2);
                if(vv1 != vv2){
                    result = false;
                }
            }
            else if((v1.type() == typeid(std::string)) || (v2.type() == typeid(std::string))){
                auto vv1 = std::any_cast<std::string>(v1);
                auto vv2 = std::any_cast<std::string>(v2);
                if(vv1 != vv2){
                    result = false;
                }
            }
            // 認識していない型の場合
            else{
                std::cout << "Unknown data type" << std::endl;
                assert(false);
            }
        }
        // 両方とも値が設定されていない場合も合致とみなす。
        // 本来は双方に値があり、かつその値が等しいときのみ合致とみなす
        // べきであるが、値の伝搬が遅れて局所ループにはまり込み
        // 抜けられなくなることを回避するために値がないままも合致とする。
        else if((v1.has_value() == false) && (v2.has_value() == false)){ 
            result = true;
        }
        // 片方のみ値が設定されている場合は不一致とする。
        else{
            result = false;
        }


        return result;
    }

    //-------------------------------------------------------
    // ２つのVectorのasd::anyの値を比較。
    // 全ての値が同じであればtrue
    // １つでも異なる値があればfalse
    bool compareValues(std::vector<std::any>& vec1, std::vector<std::any>& vec2)
    {
        bool result = true;

        // Vectorサイズが違っていたら駄目
        if(vec1.size() != vec2.size()){
            return false;
        }

        for(int i=0;i<vec1.size();i++){
            result = compareValue(vec1[i], vec2[i]);
            if(result == false){
                break;
            }
        }
        return result;
    }

public:
    //-------------------------------------------------------
    // データが到達している以内にかかわらずNodeの処理を実行する。
    // データが届いていない場合はデフォルト値若しくは、前回値を
    // 入力データとしてNodeが実行されることになる。
    // 全Node実行の結果、前回のEdgeの値と処理実行後のEdgeの値を比較し
    // 全てのEdgeがNodeの処理の実行前後で変化がなければ安定化したとみなし
    // 処理を終える。
    void step(void)
    {
        bool stabilized = false;
        std::vector<std::any> edgeValuesBefore = edgeSnapShot();
        std::vector<std::any> edgeValuesAfter;
        while(stabilized == false){
            executeAllNode();
            edgeValuesAfter = edgeSnapShot();
            stabilized = compareValues(edgeValuesBefore, edgeValuesAfter);
            edgeValuesBefore = edgeValuesAfter;
        }
    }

};




//-----------------------------------------------------------
// Executor生成関数
// この関数内で生成するクラスを変えることにより
// Executorを切り替えることが出来る。
Executor* getExecutor(void)
{
    // Executor* executor = new ExecutorNodeListTraversal();
    Executor* executor = new ExecutorStableEdge();

    return executor;
}

//-----------------------------------------------------------
// Executor生成関数
// ゆくゆくは削除したい。
Executor* getExecutor(Node* startNode, std::vector<Node*>& nodes, std::vector<Edge*>& edges)
{
    Executor* executor = getExecutor();
    executor->setStartNode(startNode);
    executor->addNodes(nodes);
    executor->addEdges(edges);
    return executor;
}


#endif



 