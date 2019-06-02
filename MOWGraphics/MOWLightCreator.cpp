#include "MOWLightCreator.h"
#include "MOWLight.h"
#include "MOWPointLight.h"
#include "MOWCommon/MOWSerializer.h"
#include "MOWPhysics/MOWPhysics.h"
#include <string>

//------------------------------------------------------
CMOWLightPtr CMOWLightCreator::CreateLight(
    const PbMOWGraphics::PbMOWLight& fromPb,
    const CMOWPhysics& physics
    )
{
    CMOWLightPtr light = 0;

    switch(fromPb.type())
    {
        
        case PbMOWGraphics::PbMOWLight_Type_POINT: { light = CMOWLightPtr(new CMOWPointLight(fromPb.name().c_str())); break; }
        /*case PbMOWGraphics::PbMOWLight_Type_SPOT: { light->m_type = SHADER_LIGHT_TYPE_SPOT; break; }
        case PbMOWGraphics::PbMOWLight_Type_DIRECTIONAL: { light->m_type = SHADER_LIGHT_TYPE_DIRECTIONAL; break; }*/
        default:{ break; }
    }
    light->LightDirection(fromPb.direction().x(), fromPb.direction().y(), fromPb.direction().z());
    light->AmbientColor((float)fromPb.ambientcolor().x(), (float)fromPb.ambientcolor().y(), (float)fromPb.ambientcolor().z(), (float)fromPb.ambientcolor().w());
    light->DiffuseColor((float)fromPb.diffusecolor().x(), (float)fromPb.diffusecolor().y(), (float)fromPb.diffusecolor().z(), (float)fromPb.diffusecolor().w());
    light->SpecularColor((float)fromPb.specularcolor().x(), (float)fromPb.specularcolor().y(), (float)fromPb.specularcolor().z(), (float)fromPb.specularcolor().w());
    light->Attenuation((float)fromPb.attenuation().x(), (float)fromPb.attenuation().y(), (float)fromPb.attenuation().z());

    light->HasPosition(fromPb.hasposition());

    if( fromPb.has_model() )
    {
        light->MutableModel()->PropertiesFromPb(fromPb.model(),physics);
    }

    return light;
}
