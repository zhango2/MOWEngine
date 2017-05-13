#include "MOWGraphics.h"
#include "MOWMaterial.h"
#include "MOWLight.h"
//------------------------------------------------------
void MOWGraphics::InitializeClasses()
{

    CMOWMaterial::InitializeMOWClass();
    CMOWLight::InitializeMOWClass();
    /*CDXClass::RegisterDXClass("CDXModel");
    CDXClass::RegisterDXClass("CDXLight");
    CDXClass::RegisterDXClass("CDXPointLight");
    CDXClass::RegisterDXClass("CDXMaterial");*/
}

