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
        auto enty = getEntryNode();
        auto exit = getExitNode();

        auto X00 = gb.createNode<NodeNot>("X00");
        auto X01 = gb.createNode<NodeNot>("X01");
        auto X02 = gb.createNode<NodeNot>("X02");
        auto X03 = gb.createNode<NodeNot>("X03");

        auto U00  = gb.createNode<NodeNand>("U00");
        auto U01  = gb.createNode<NodeNand>("U01");
        auto U10  = gb.createNode<NodeNand>("U10");
        auto U11  = gb.createNode<NodeNand>("U11");
        auto L00  = gb.createNode<NodeNand>("L00");
        auto L01  = gb.createNode<NodeNand>("L01");
        auto L10  = gb.createNode<NodeNand>("L10");
        auto L11  = gb.createNode<NodeNand>("L11");

        auto Y00  = gb.createNode<NodeNor>("Y00");
        auto Y01  = gb.createNode<NodeNor>("Y01");
        auto Y02  = gb.createNode<NodeNor>("Y02");
        auto Y03  = gb.createNode<NodeNor>("Y03");
        auto Y04  = gb.createNode<NodeNor>("Y04");
        auto Y05  = gb.createNode<NodeNor>("Y05");
        auto Y06  = gb.createNode<NodeNor>("Y06");
        auto Y07  = gb.createNode<NodeNor>("Y07");
        auto Y08  = gb.createNode<NodeNor>("Y08");
        auto Y09  = gb.createNode<NodeNor>("Y09");
        auto Y10  = gb.createNode<NodeNor>("Y10");
        auto Y11  = gb.createNode<NodeNor>("Y11");
        auto Y12  = gb.createNode<NodeNor>("Y12");
        auto Y13  = gb.createNode<NodeNor>("Y13");
        auto Y14  = gb.createNode<NodeNor>("Y14");
        auto Y15  = gb.createNode<NodeNor>("Y15");

        gb.outto<bool>(Port(X00, 1), Ports{Port(L10, 2), Port(L00, 2)});
        gb.outto<bool>(Port(X01, 1), Ports{Port(L01, 1), Port(L00, 1)});
        gb.outto<bool>(Port(X02, 1), Ports{Port(U10, 2), Port(U00, 2)});
        gb.outto<bool>(Port(X03, 1), Ports{Port(U01, 1), Port(U00, 1)});

        gb.outto<bool>(Port(U11,1), Ports{Port(Y15,1), Port(Y14,1), Port(Y13,1), Port(Y12,1)});
        gb.outto<bool>(Port(U10,1), Ports{Port(Y11,1), Port(Y10,1), Port(Y09,1), Port(Y08,1)});
        gb.outto<bool>(Port(U01,1), Ports{Port(Y07,1), Port(Y06,1), Port(Y05,1), Port(Y04,1)});
        gb.outto<bool>(Port(U00,1), Ports{Port(Y03,1), Port(Y02,1), Port(Y01,1), Port(Y00,1)});
        gb.outto<bool>(Port(L11,1), Ports{Port(Y07,2), Port(Y03,2), Port(Y15,2), Port(Y11,2)});
        gb.outto<bool>(Port(L10,1), Ports{Port(Y06,2), Port(Y02,2), Port(Y14,2), Port(Y10,2)});
        gb.outto<bool>(Port(L01,1), Ports{Port(Y05,2), Port(Y01,2), Port(Y13,2), Port(Y09,2)});
        gb.outto<bool>(Port(L00,1), Ports{Port(Y04,2), Port(Y00,2), Port(Y12,2), Port(Y08,2)});

        gb.outto<bool>(Port(enty, 1), Ports{ Port(L11, 2), Port(L01, 2), Port(X00, 1) }); // X0
        gb.outto<bool>(Port(enty, 2), Ports{ Port(L11, 1), Port(L10, 1), Port(X01, 1) }); // X1
        gb.outto<bool>(Port(enty, 3), Ports{ Port(U11, 2), Port(U01, 2), Port(X02, 1) }); // X2
        gb.outto<bool>(Port(enty, 4), Ports{ Port(U11, 1), Port(U10, 1), Port(X03, 1) }); // X3

        gb.outto<bool>(Port(Y00, 1), Ports{ Port(exit, 1) });
        gb.outto<bool>(Port(Y01, 1), Ports{ Port(exit, 2) });
        gb.outto<bool>(Port(Y02, 1), Ports{ Port(exit, 3) });
        gb.outto<bool>(Port(Y03, 1), Ports{ Port(exit, 4) });
        gb.outto<bool>(Port(Y04, 1), Ports{ Port(exit, 5) });
        gb.outto<bool>(Port(Y05, 1), Ports{ Port(exit, 6) });
        gb.outto<bool>(Port(Y06, 1), Ports{ Port(exit, 7) });
        gb.outto<bool>(Port(Y07, 1), Ports{ Port(exit, 8) });
        gb.outto<bool>(Port(Y08, 1), Ports{ Port(exit, 9) });
        gb.outto<bool>(Port(Y09, 1), Ports{ Port(exit, 10) });
        gb.outto<bool>(Port(Y10, 1), Ports{ Port(exit, 11) });
        gb.outto<bool>(Port(Y11, 1), Ports{ Port(exit, 12) });
        gb.outto<bool>(Port(Y12, 1), Ports{ Port(exit, 13) });
        gb.outto<bool>(Port(Y13, 1), Ports{ Port(exit, 14) });
        gb.outto<bool>(Port(Y14, 1), Ports{ Port(exit, 15) });
        gb.outto<bool>(Port(Y15, 1), Ports{ Port(exit, 16) });

        commit();
    }
};

#endif

