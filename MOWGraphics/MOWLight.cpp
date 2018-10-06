#include "MOWLight.h"
#include "MOWCommon.h"
#include "MOWObject.h"
#include "MOWModel.h"
#include "MOWVariant.h"
#include "MOWLightCreator.h"
#include "MOWCommon/MOWVector.h"
#include "MOWCommon/MOWSerializer.h"

using namespace DirectX;

#define SHADER_LIGHT_TYPE_NONE_DESC "None"
#define SHADER_LIGHT_TYPE_POINT_DESC "Point"
#define SHADER_LIGHT_TYPE_DIRECTIONAL_DESC "Directional"
#define SHADER_LIGHT_TYPE_SPOT_DESC "Spot"

//--------------------------------------------

CMOWLight::CMOWLight(
    const char* name
    )
{
    memset(&m_ambientColor,0,sizeof(XMFLOAT4));
    memset(&m_diffuseColor,0,sizeof(XMFLOAT4));
    memset(&m_specularColor,0,sizeof(XMFLOAT4));
    memset(&m_attenuation,0,sizeof(XMFLOAT3));
    HasPosition(false);
    m_type = SHADER_LIGHT_TYPE_NONE;
	m_name = name;
}
//---------------------------------------------
CMOWLight::~CMOWLight()
{

}
//------------------------------------------------------
void CMOWLight::InitializeMOWClass()
{
    CMOWClass::RegisterMOWClass("CDXLight");

    std::vector<std::string> names;

    names.push_back("Red");
    names.push_back("Green");
    names.push_back("Blue");

    CMOWClass::RegisterAttribute("CDXLight","AmbientColor","Ambient Color",MOWAttributeType_Vector,names,CMOWLight::SetAmbientColorCB,CMOWLight::GetAmbientColorCB,0.0f,1.0f);
    CMOWClass::RegisterAttribute("CDXLight","DiffuseColor","Diffuse Color",MOWAttributeType_Vector,names,CMOWLight::SetDiffuseColorCB,CMOWLight::GetDiffuseColorCB,0.0f,1.0f);
    CMOWClass::RegisterAttribute("CDXLight","SpecularColor","Specular Color",MOWAttributeType_Vector,names,CMOWLight::SetSpecularColorCB,CMOWLight::GetSpecularColorCB,0.0f,1.0f);

    names.clear();
    names.push_back("X");
    names.push_back("Y");
    names.push_back("Z");

    CMOWClass::RegisterAttribute("CDXLight","Position","Position",MOWAttributeType_Vector,names,CMOWLight::SetPositionCB,CMOWLight::GetPositionCB,-5000.0f,5000.0f);

    names.clear();
    names.push_back("A0");
    names.push_back("A1");
    names.push_back("A2");

    CMOWClass::RegisterAttribute("CDXLight","Attenuation","Attenuation",MOWAttributeType_Vector,names,CMOWLight::SetAttenuationCB,CMOWLight::GetAttenuationCB,0.0f,1.0f);

    names.clear();
    names.push_back("X");
    names.push_back("Y");
    names.push_back("Z");

    CMOWClass::RegisterAttribute("CDXLight","Range","Range",MOWAttributeType_Vector,names,CMOWLight::SetRangeCB,CMOWLight::GetRangeCB,0.0f,200.0f);
}
//---------------------------------------------

void CMOWLight::LightDirection( 
    float x, 
    float y, 
    float z 
    )
{
    m_lightDirection = XMFLOAT3(x,y,z);
}
//---------------------------------------------

const XMFLOAT3& CMOWLight::LightDirection() const
{
    return m_lightDirection;
}
//---------------------------------------------

void CMOWLight::AmbientColor( 
    byte r, 
    byte g, 
    byte b, 
    byte alpha 
    )
{
    m_ambientColor = XMFLOAT4(r/255.0f,g/255.0f,b/255.0f,alpha/255.0f);
}
//---------------------------------------------

const XMFLOAT4& CMOWLight::AmbientColor() const
{
    return m_ambientColor;
}
//------------------------------------------------------
void CMOWLight::AmbientColor(
    float red, 
    float green, 
    float blue, 
    float alpha
    )
{
    m_ambientColor = XMFLOAT4(red,green,blue,alpha);
}
//---------------------------------------------

bool CMOWLight::HasPosition() const
{
    return m_hasPosition;
}
//---------------------------------------------

void CMOWLight::HasPosition( 
    bool hasPos 
    )
{
    m_hasPosition = hasPos;
}
//---------------------------------------------

void CMOWLight::DiffuseColor( 
    byte r, 
    byte g, 
    byte b, 
    byte alpha
    )
{
    m_diffuseColor = XMFLOAT4(r/255.0f,g/255.0f,b/255.0f,alpha/255.0f);
}
//---------------------------------------------

const XMFLOAT4& CMOWLight::DiffuseColor() const
{
    return m_diffuseColor;
}
//------------------------------------------------------
void CMOWLight::DiffuseColor(
    float red, 
    float green, 
    float blue, 
    float alpha
    )
{
    m_diffuseColor = XMFLOAT4(red,green,blue,alpha);
}
//---------------------------------------------

void CMOWLight::SpecularColor( 
    byte r, 
    byte g, 
    byte b, 
    byte alpha)
{
    m_specularColor = XMFLOAT4(r/255.0f,g/255.0f,b/255.0f,alpha/255.0f);
}
//---------------------------------------------

const XMFLOAT4& CMOWLight::SpecularColor() const
{
    return m_specularColor;
}
//------------------------------------------------------
void CMOWLight::SpecularColor(
    float red, 
    float green, 
    float blue, 
    float alpha
    )
{
    m_specularColor = XMFLOAT4(red,green,blue,alpha);
}
//---------------------------------------------

void CMOWLight::UpdateViewPoint()
{
    
    if( GeometryObject() )
    {
        MutableGeometryObject()->Update();
        // Setup the vector that points upwards.
        XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f,1.0f);

        // Create the view matrix from the three updated vectors.
        XMStoreFloat4x4(&MutableViewMatrix(),XMMatrixLookAtLH(GeometryObject()->Position(), m_lookAt, up));

        XMStoreFloat4x4(&MutableProjectionMatrix(),XMMatrixOrthographicLH(20.0f, 20.0f, SHADOWMAP_NEAR, SHADOWMAP_DEPTH));
    }
    
}
//---------------------------------------------

bool CMOWLight::Initialize( 
    ID3D11Device* device 
    )
{
    bool retVal = true;
    if( MutableModel() )
    {
        retVal = MutableModel()->Initialize(device);
    }
    return retVal;
}
//---------------------------------------
ShaderLightType CMOWLight::Type() const
{
    return m_type;
}
//---------------------------------------
ShaderLight CMOWLight::AsShaderLight()
{
    ShaderLight light;

    light.m_ambient = m_ambientColor;
    light.m_diffuse = m_diffuseColor;
    light.m_specular = m_specularColor;
    light.m_lightType = m_type;

    XMVECTOR posVector = GeometryObject() ? GeometryObject()->Position() : XMVectorSet(0.0f,0.0f,0.0f, 1.0f);
    XMStoreFloat3(&light.m_position, posVector);
    light.m_attenuation = m_attenuation;

    //TODO: range etc

    return light;
    
}
//---------------------------------------
void CMOWLight::Attenuation(
    float a0,
    float a1,
    float a2
    )
{
    m_attenuation.x = a0;
    m_attenuation.y = a1;
    m_attenuation.z = a2;
}
//------------------------------------------------------
const DirectX::XMFLOAT3 CMOWLight::Attenuation() const
{
    return m_attenuation;
}
//------------------------------------------------------
const char* CMOWLight::Name() const
{
	return m_name.c_str();
}
//------------------------------------------------------
CMOWVariant* CMOWLight::DoGetAttributeValue(
    const char* attrName
    )
{
    CMOWVariant* retVariant = 0;
    std::string attr = attrName;
    
    if( attr == "AmbientColor" )
    {
        CMOWVector* color = new CMOWVector(AmbientColor().x,AmbientColor().y,AmbientColor().z,AmbientColor().w);
        retVariant = new CMOWVariant(color);
    }
    else if( attr == "DiffuseColor" )
    {
        CMOWVector* color = new CMOWVector(DiffuseColor().x,DiffuseColor().y,DiffuseColor().z,DiffuseColor().w);
        retVariant = new CMOWVariant(color);
    }
    else if( attr == "SpecularColor" )
    {
        CMOWVector* color = new CMOWVector(SpecularColor().x,SpecularColor().y,SpecularColor().z,SpecularColor().w);
        retVariant = new CMOWVariant(color);
    }

    return retVariant;
}
//------------------------------------------------------
void CMOWLight::DoSetAttributeValue(
    const char* attrName, 
    CMOWVariant* val
    )
{
    std::string name = attrName;

    if( name == "AmbientColor" )
    {
        AmbientColor(val->AsVector().X(),val->AsVector().Y(),val->AsVector().Z(),val->AsVector().W());
    }
    else if( name == "DiffuseColor" )
    {
        DiffuseColor(val->AsVector().X(),val->AsVector().Y(),val->AsVector().Z(),val->AsVector().W());
    }
    else if( name == "SpecularColor" )
    {
        SpecularColor(val->AsVector().X(),val->AsVector().Y(),val->AsVector().Z(),val->AsVector().W());
    }
}
//------------------------------------------------------
void CMOWLight::SetupModelAndPhysics(
    const CMOWPhysics& physics
    )
{
    //Do nothing, override to implement
}
//------------------------------------------------------
std::string CMOWLight::ClassName()
{
    return "CDXLight";
}
//------------------------------------------------------
CMOWVariant* CMOWLight::GetAmbientColorCB(
    const CMOWClass* aClass
    )
{
    const CMOWLight* pThis = dynamic_cast<const CMOWLight*>(aClass);
    CMOWVector* color = new CMOWVector(pThis->AmbientColor().x,pThis->AmbientColor().y,pThis->AmbientColor().z,pThis->AmbientColor().w);
    return new CMOWVariant(color);
}
//------------------------------------------------------
CMOWVariant* CMOWLight::GetDiffuseColorCB(
    const CMOWClass* aClass
    )
{
    const CMOWLight* pThis = dynamic_cast<const CMOWLight*>(aClass);
    CMOWVector* color = new CMOWVector(pThis->DiffuseColor().x,pThis->DiffuseColor().y,pThis->DiffuseColor().z,pThis->DiffuseColor().w);
    return new CMOWVariant(color);
}
//------------------------------------------------------
CMOWVariant* CMOWLight::GetSpecularColorCB(
    const CMOWClass* aClass
    )
{
    const CMOWLight* pThis = dynamic_cast<const CMOWLight*>(aClass);
    CMOWVector* color = new CMOWVector(pThis->SpecularColor().x,pThis->SpecularColor().y,pThis->SpecularColor().z,pThis->SpecularColor().w);
    return new CMOWVariant(color);
}
//------------------------------------------------------
void CMOWLight::SetAmbientColorCB(
    CMOWVariant* val,
    CMOWClass* aClass
    )
{
    CMOWLight* pThis = dynamic_cast<CMOWLight*>(aClass);
    pThis->AmbientColor(val->AsVector().X(),val->AsVector().Y(),val->AsVector().Z(),val->AsVector().W());
}
//------------------------------------------------------
void CMOWLight::SetDiffuseColorCB(
    CMOWVariant* val,
    CMOWClass* aClass
    )
{
    CMOWLight* pThis = dynamic_cast<CMOWLight*>(aClass);
    pThis->DiffuseColor(val->AsVector().X(),val->AsVector().Y(),val->AsVector().Z(),val->AsVector().W());
}
//------------------------------------------------------
void CMOWLight::SetSpecularColorCB(
    CMOWVariant* val,
    CMOWClass* aClass
    )
{
    CMOWLight* pThis = dynamic_cast<CMOWLight*>(aClass);
    pThis->SpecularColor(val->AsVector().X(),val->AsVector().Y(),val->AsVector().Z(),val->AsVector().W());
}
//------------------------------------------------------
CMOWVariant* CMOWLight::GetPositionCB(
    const CMOWClass* aClass
    )
{
    CMOWVariant* retVal = 0;
    CMOWVector* val = 0;
    const CMOWLight* pThis = dynamic_cast<const CMOWLight*>(aClass);

    if( pThis->HasPosition() && pThis->GeometryObject() )
    {
        val = new CMOWVector(XMVectorGetX(pThis->GeometryObject()->Position()),XMVectorGetY(pThis->GeometryObject()->Position()),XMVectorGetZ(pThis->GeometryObject()->Position()));
        retVal = new CMOWVariant(val);
    }

    
    return retVal;
}
//------------------------------------------------------
CMOWVariant* CMOWLight::GetAttenuationCB(
    const CMOWClass* aClass
    )
{
    CMOWVariant* retVal = 0;
    CMOWVector* val = 0;
    const CMOWLight* pThis = dynamic_cast<const CMOWLight*>(aClass);

    
    val = new CMOWVector(pThis->Attenuation().x,pThis->Attenuation().y,pThis->Attenuation().z);
    retVal = new CMOWVariant(val);

    return retVal;
}
//------------------------------------------------------
CMOWVariant* CMOWLight::GetRangeCB(
    const CMOWClass* aClass
    )
{
    CMOWVariant* retVal = 0;
    CMOWVector* val = 0;
    const CMOWLight* pThis = dynamic_cast<const CMOWLight*>(aClass);

    if( pThis->GeometryObject() )
    {
        val = new CMOWVector(pThis->GeometryObject()->Scale().x,pThis->GeometryObject()->Scale().y,pThis->GeometryObject()->Scale().z);
        retVal = new CMOWVariant(val);
    }

    

    return retVal;
}
//------------------------------------------------------
void CMOWLight::SetPositionCB(
    CMOWVariant* val,
    CMOWClass* aClass
    )
{
    CMOWLight* pThis = dynamic_cast<CMOWLight*>(aClass);

    if( pThis->HasPosition() && pThis->GeometryObject() )
    {
        pThis->MutableGeometryObject()->Position(val->AsVector().X(),val->AsVector().Y(),val->AsVector().Z());
    }
}
//------------------------------------------------------
void CMOWLight::SetAttenuationCB(
    CMOWVariant* val,
    CMOWClass* aClass
    )
{
    CMOWLight* pThis = dynamic_cast<CMOWLight*>(aClass);

    pThis->Attenuation(val->AsVector().X(),val->AsVector().Y(),val->AsVector().Z());

}
//------------------------------------------------------
void CMOWLight::SetRangeCB(
    CMOWVariant* val,
    CMOWClass* aClass
    )
{
    CMOWLight* pThis = dynamic_cast<CMOWLight*>(aClass);

    if( pThis->GeometryObject() )
    {
        pThis->MutableGeometryObject()->Scale(val->AsVector().X(),val->AsVector().Y(),val->AsVector().Z());
    }
}
//------------------------------------------------------
const char* CMOWLight::LightTypeToString(
    ShaderLightType type
    )
{
    const char* lightType = 0;
    switch( type )
    {
        case SHADER_LIGHT_TYPE_NONE:
        {
            lightType = SHADER_LIGHT_TYPE_NONE_DESC;
            break;
        }
        case SHADER_LIGHT_TYPE_SPOT:
        {
            lightType = SHADER_LIGHT_TYPE_SPOT_DESC;
            break;
        }
        case SHADER_LIGHT_TYPE_POINT:
        {
            lightType = SHADER_LIGHT_TYPE_POINT_DESC;
            break;
        }
        case SHADER_LIGHT_TYPE_DIRECTIONAL:
        {
            lightType = SHADER_LIGHT_TYPE_DIRECTIONAL_DESC;
            break;
        }
        default:
        {
            lightType = SHADER_LIGHT_TYPE_NONE_DESC;
            break;
        }
    }

    return lightType;
}
//------------------------------------------------------
ShaderLightType CMOWLight::StringToLightType(
    const char* lightType
    )
{
    ShaderLightType retVal = SHADER_LIGHT_TYPE_NONE;
    std::string lightTypeDesc = lightType;

    if( lightTypeDesc == SHADER_LIGHT_TYPE_NONE_DESC )
    {
        retVal = SHADER_LIGHT_TYPE_NONE;
    }
    else if( lightTypeDesc == SHADER_LIGHT_TYPE_POINT_DESC )
    {
        retVal = SHADER_LIGHT_TYPE_POINT;
    }
    else if( lightTypeDesc == SHADER_LIGHT_TYPE_DIRECTIONAL_DESC )
    {
        retVal = SHADER_LIGHT_TYPE_DIRECTIONAL;
    }

    return retVal;
}
//------------------------------------------------------
void CMOWLight::ToPb(
    PbMOWGraphics::PbMOWLight& toPb,
    const CMOWPhysics& physics
    ) const
{
    toPb.set_name(Name());

    switch(Type())
    {
        case SHADER_LIGHT_TYPE_NONE:{ toPb.set_type(PbMOWGraphics::PbMOWLight_Type_NONE); break; }
        case SHADER_LIGHT_TYPE_POINT:{ toPb.set_type(PbMOWGraphics::PbMOWLight_Type_POINT); break; }
        case SHADER_LIGHT_TYPE_SPOT:{ toPb.set_type(PbMOWGraphics::PbMOWLight_Type_SPOT); break; }
        case SHADER_LIGHT_TYPE_DIRECTIONAL:{ toPb.set_type(PbMOWGraphics::PbMOWLight_Type_DIRECTIONAL); break; }
        default:{toPb.set_type(PbMOWGraphics::PbMOWLight_Type_NONE); break;}
    }

    toPb.mutable_direction()->set_x(m_lightDirection.x);
    toPb.mutable_direction()->set_y(m_lightDirection.y);
    toPb.mutable_direction()->set_z(m_lightDirection.z);

    toPb.mutable_ambientcolor()->set_x(AmbientColor().x);
    toPb.mutable_ambientcolor()->set_y(AmbientColor().y);
    toPb.mutable_ambientcolor()->set_z(AmbientColor().z);
    toPb.mutable_ambientcolor()->set_w(AmbientColor().w);

    toPb.mutable_diffusecolor()->set_x(DiffuseColor().x);
    toPb.mutable_diffusecolor()->set_y(DiffuseColor().y);
    toPb.mutable_diffusecolor()->set_z(DiffuseColor().z);
    toPb.mutable_diffusecolor()->set_w(DiffuseColor().w);

    toPb.mutable_specularcolor()->set_x(SpecularColor().x);
    toPb.mutable_specularcolor()->set_y(SpecularColor().y);
    toPb.mutable_specularcolor()->set_z(SpecularColor().z);
    toPb.mutable_specularcolor()->set_w(SpecularColor().w);

    toPb.mutable_attenuation()->set_x(Attenuation().x);
    toPb.mutable_attenuation()->set_y(Attenuation().y);
    toPb.mutable_attenuation()->set_z(Attenuation().z);

    toPb.set_hasposition(HasPosition());

    if( Model() )
    {
        Model()->ToPb(*(toPb.mutable_model()),physics);
    }

}
//------------------------------------------------------
void CMOWLight::Position(
    float xPos, 
    float yPos, 
    float zPos
    )
{
    if( Model() )
    {
        MutableModel()->Position(xPos,yPos,zPos);
    }
    if( GeometryObject() )
    {
        MutableGeometryObject()->Position(xPos,yPos,zPos);
    }
}
//------------------------------------------------------
void CMOWLight::LightScale(
    float x, 
    float y, 
    float z
    )
{
    if( GeometryObject() )
    {
        MutableGeometryObject()->Scale(x,y,z);
    }
}
//------------------------------------------------------
void CMOWLight::LightScale(
    float uniformScale
    )
{
    LightScale(uniformScale,uniformScale,uniformScale);
}
//------------------------------------------------------
