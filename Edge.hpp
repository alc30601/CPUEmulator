// Edge.hpp

#ifndef __EDGE_HPP__
#define __EDGE_HPP__

class Node;

//-----------------------------------------------------------
// Definition of Edge class.
// Edge is between Nodes.
// Edge itself has a current value.
// Edge has link to Ndestination nodes 
template <class T>
class Edge
{
    T _v;   // value that hold this edge

public:

    //-------------------------------------------------------
    T getValue(void)
    {
        return _v;
    }

    //-------------------------------------------------------
    void setValue(T value)
    {
        _v = value;
    }
};

#endif


