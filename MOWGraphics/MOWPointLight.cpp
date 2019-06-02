#include "MOWPointLight.h"
#include "MOWObject.h"
#include "MOWRenderableObject.h"

//---------------------------------------------
CMOWPointLight::CMOWPointLight(const char* name):
	CMOWLight(name)
{
    CMOWLight::HasPosition(true);
    m_type = SHADER_LIGHT_TYPE_POINT;
}
//---------------------------------------------
CMOWPointLight::~CMOWPointLight()
{

}
//---------------------------------------------
CMOWRenderableObject* CMOWPointLight::RenderableObject()
{
    return this;
}
//---------------------------------------------
CMOWModelPtrC CMOWPointLight::Model() const
{
    CMOWModelPtrC pThis = shared_from_this();
    return pThis;
}
//------------------------------------------------------
CMOWModelPtr CMOWPointLight::MutableModel()
{
    return shared_from_this();
}
//---------------------------------------------
CMOWLight* CMOWPointLight::Create(
    const char* name,
    const CMOWPhysics& physics
    )
{
    CMOWPointLight* light = new CMOWPointLight(name);
    light->SetupModelAndPhysics(physics);
    return light;
}
//---------------------------------------
void CMOWPointLight::SetupModelAndPhysics(
    const CMOWPhysics& physics
    )
{
    SphereConstruct(1.0f,0,0,physics,false,10,0.0f,false);
    //PhysicalBody()->enableDeactivation(false);
}
//------------------------------------------------------
void CMOWPointLight::Scale(
    float xScale, 
    float yScale, 
    float zScale
    )
{
    CMOWModel::Scale(xScale,yScale,zScale);
}
//------------------------------------------------------
void CMOWPointLight::Position(
    float xPos, 
    float yPos, 
    float zPos
    )
{
    CMOWModel::Position(xPos,yPos,zPos);
}
//------------------------------------------------------
const CMOWObject* CMOWPointLight::GeometryObject() const
{
    return this;
}
//------------------------------------------------------
CMOWObject* CMOWPointLight::MutableGeometryObject()
{
    return this;
}