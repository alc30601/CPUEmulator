// Node.hpp
#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <memory>
#include <vector>

class Edge;

//-----------------------------------------------------------
class Node
{
    // ★★★自身の出力先のEdge一覧を保持する。
    std::vector<std::shared_ptr<Edge>> _outputEdges;

    // ★★★自身の入力元のEdge一覧を保持する。
    std::vector<std::shared_ptr<Edge>> _inputEdges;

public:
    // Execute the operation of this node
    virtual void execute(void) = 0;

    //-------------------------------------------------------
    // ★★★
    // 保持するEdge一覧を返すメソッド
    // 個々のNodeにおけるEdgeの持ち方は様々なため、
    // 本クラスでは純仮想関数とし、継承先のクラスで実装することとする。
    // 汎用メソッドとするにはEdgeの型も共通にしないといけないが、
    // 保持するデータ毎に対応するためにテンプレートを使うと難しいかもしれない。
    // テンプレートが使えないならば、継承で持たせる？
    // 
    // virtual List<std::shread_ptr<Edge>> getOutputEdges();
    // 
    // virtual List<std::shread_ptr<Edge>> getIntputEdges();

};


#endif
