#ifndef CDXLightCreator_H
#define CDXLightCreator_H

#include <iostream>
#include <fstream>
#include "MOWMessages/MOWGraphics.pb.h"
#include "MOWCommon/MOWSharedPtr.h"

class CMOWPhysics;


DECLARE_SHARED_PTR(CMOWLight)

class CMOWLightCreator
{
public:

    static CMOWLightPtr               
        CreateLight(
            const PbMOWGraphics::PbMOWLight& fromPb,
            const CMOWPhysics& physics
            );
};

#endif