// NodeDecoder.hh
#ifndef __NODEDECODER_HPP__
#define __NODEDECODER_HPP__

#include "NodeComplex.hpp"
#include "NodeLogics.hpp"


//-----------------------------------------------------------
// NodeDecoder4to16 - 4 bits binary to 16 bits
// In-port  : X0, X1, X2, X3 
// Out0port : Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7, Y8, Y9, Y10, Y11, Y12, Y13, Y14, Y15
class NodeDecoder4to16 : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeDecoder4to16(void)
    {
        auto& gb = getGraphBuilder();

        auto X00 = gb.createNode<NodeNot>();
        auto X01 = gb.createNode<NodeNot>();
        auto X02 = gb.createNode<NodeNot>();
        auto X03 = gb.createNode<NodeNot>();

        auto U00  = gb.createNode<NodeNand>();
        auto U01  = gb.createNode<NodeNand>();
        auto U10  = gb.createNode<NodeNand>();
        auto U11  = gb.createNode<NodeNand>();
        auto L00  = gb.createNode<NodeNand>();
        auto L01  = gb.createNode<NodeNand>();
        auto L10  = gb.createNode<NodeNand>();
        auto L11  = gb.createNode<NodeNand>();

        auto Y00  = gb.createNode<NodeNor>();
        auto Y01  = gb.createNode<NodeNor>();
        auto Y02  = gb.createNode<NodeNor>();
        auto Y03  = gb.createNode<NodeNor>();
        auto Y04  = gb.createNode<NodeNor>();
        auto Y05  = gb.createNode<NodeNor>();
        auto Y06  = gb.createNode<NodeNor>();
        auto Y07  = gb.createNode<NodeNor>();
        auto Y08  = gb.createNode<NodeNor>();
        auto Y09  = gb.createNode<NodeNor>();
        auto Y10  = gb.createNode<NodeNor>();
        auto Y11  = gb.createNode<NodeNor>();
        auto Y12  = gb.createNode<NodeNor>();
        auto Y13  = gb.createNode<NodeNor>();
        auto Y14  = gb.createNode<NodeNor>();
        auto Y15  = gb.createNode<NodeNor>();

        gb.outto(Port(X00, 1), Ports{Port(L10, 2), Port(L00, 2)});
        gb.outto(Port(X01, 1), Ports{Port(L01, 1), Port(L00, 1)});
        gb.outto(Port(X02, 1), Ports{Port(U10, 2), Port(U00, 2)});
        gb.outto(Port(X03, 1), Ports{Port(U01, 1), Port(U00, 1)});

        gb.outto(Port(U11,1), Ports{Port(Y15,1), Port(Y14,1), Port(Y13,1), Port(Y12,1)});
        gb.outto(Port(U10,1), Ports{Port(Y11,1), Port(Y10,1), Port(Y09,1), Port(Y08,1)});
        gb.outto(Port(U01,1), Ports{Port(Y07,1), Port(Y06,1), Port(Y05,1), Port(Y04,1)});
        gb.outto(Port(U00,1), Ports{Port(Y03,1), Port(Y02,1), Port(Y01,1), Port(Y00,1)});
        gb.outto(Port(L11,1), Ports{Port(Y07,2), Port(Y03,2), Port(Y15,2), Port(Y11,2)});
        gb.outto(Port(L10,1), Ports{Port(Y06,2), Port(Y02,2), Port(Y14,2), Port(Y10,2)});
        gb.outto(Port(L01,1), Ports{Port(Y05,2), Port(Y01,2), Port(Y13,2), Port(Y09,2)});
        gb.outto(Port(L00,1), Ports{Port(Y04,2), Port(Y00,2), Port(Y12,2), Port(Y08,2)});


        setInPortss(Ports{ Port(L11, 2), Port(L01, 2), Port(X00, 1) },    // X0
                    Ports{ Port(L11, 1), Port(L10, 1), Port(X01, 1) },    // X1
                    Ports{ Port(U11, 2), Port(U01, 2), Port(X02, 1) },    // X2
                    Ports{ Port(U11, 1), Port(U10, 1), Port(X03, 1) });   // X3

        setOutPorts(Port(Y00, 1),
                    Port(Y01, 2),
                    Port(Y02, 3),
                    Port(Y03, 4),
                    Port(Y04, 5),
                    Port(Y05, 6),
                    Port(Y06, 7),
                    Port(Y07, 8),
                    Port(Y08, 9),
                    Port(Y09, 10),
                    Port(Y10, 11),
                    Port(Y11, 12),
                    Port(Y12, 13),
                    Port(Y13, 14),
                    Port(Y14, 15),
                    Port(Y15, 16));

        commit();
    }
};


#endif

