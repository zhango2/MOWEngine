#include "MOWPointLight.h"
#include "MOWObject.h"
#include "MOWRenderableObject.h"

//---------------------------------------------
CMOWPointLight::CMOWPointLight(const char* name):
	CMOWLight(name)
{
    HasPosition(true);
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
const CMOWModel* CMOWPointLight::Model() const
{
    return this;
}
//------------------------------------------------------
CMOWModel* CMOWPointLight::MutableModel()
{
    return this;
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
    SphereConstruct(1.0f,0,0,physics,false,100,0.0f,false);
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