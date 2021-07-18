// NodeIO.hpp
// I/O
#ifndef __NODEIO_HPP__
#define __NODEIO_HPP__

#include "Node.hpp"
#include "NodeRegister.hpp"
#include "NodeUtils.hpp"


//-----------------------------------------------------------
// 出力ポートはこのエミュレータ上はレジスタと同じ。
using NodeOutPort = NodeRegister;

//-----------------------------------------------------------
// 入力ポートはこのエミュレータ上は単に入力値を出力値に送るだけのもの
using NodeInPort = NodeTransparent;


#endif

