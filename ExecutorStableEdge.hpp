// ExecutorStableEdge.hpp
#ifndef __EXECUTORSTABLEEDGE_HPP__
#define __EXECUTORSTABLEEDGE_HPP__

#include "Executor.hpp"


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
    // 使用される可能性のある型をすべて列挙している。
    // (std::anyの型を知ることなく比較できないものか)
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
            std::cout << "Edges have no values" << std::endl;
            assert(false);
            result = true;
        }
        // 片方のみ値が設定されている場合は不一致とする。
        else{
            std::cout << "Edge has no value" << std::endl;
            assert(false);
            result = false;
        }


        return result;
    }

    //-------------------------------------------------------
    // ２つのVectorのstd::anyの値を比較。
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


    //-------------------------------------------------------
    void printEdgeValues(std::vector<std::any>& values)
    {
#if 0
        for(auto value : values){
            bool v = std::any_cast<bool>(value);
            std::cout << v << " ";
        }
        std::cout << std::endl;
#endif
    }

public:
    //-------------------------------------------------------
    // データが到達しているかどうかにかかわらずNodeの処理を実行する。
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
            printEdgeValues(edgeValuesAfter);
            stabilized = compareValues(edgeValuesBefore, edgeValuesAfter);
            edgeValuesBefore = edgeValuesAfter;
        }
    }

};




#endif

