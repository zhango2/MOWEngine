#ifndef CDXLightCreator_H
#define CDXLightCreator_H

#include <iostream>
#include <fstream>
#include "MOWMessages/MOWGraphics.pb.h"

class CMOWPhysics;
class CMOWLight;

class CMOWLightCreator
{
public:

    static CMOWLight*               
        CreateLight(
            const PbMOWGraphics::PbMOWLight& fromPb,
            const CMOWPhysics& physics
            );
};

#endif