#ifndef _DXTRIANGLE_H
#define _DXTRIANGLE_H

#include "MOWModel.h"

class CMOWTriangle : public CMOWModel
{
    typedef CMOWModel BaseClass;
public:

protected:
    CMOWTriangle(
        );

    void                        
        PopulateVerticesAndIndices();

    D3D11_PRIMITIVE_TOPOLOGY    
        Topology(
            )const override;
};

#endif