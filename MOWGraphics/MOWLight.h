#ifndef CDXLight_H
#define CDXLight_H

#include "MOWCommon.h"
#include "MOWViewPoint.h"
#include "MOWClass.h"
#include "MOWObject.h"
#include "MOWMessages/MOWGraphics.pb.h"


class CMOWVariant;
class CMOWPhysics;

class CMOWLight : 
    public CMOWViewPoint,
    public CMOWClass
{
public:

    using BaseClass = CMOWViewPoint;

    CMOWLight(
        const char* name
        );

    virtual             
        ~CMOWLight(
            );

    void                
        LightDirection(
            float x,
            float y,
            float z
            );

    const DirectX::XMFLOAT3&  
        LightDirection(
            ) const;


    void                
        AmbientColor(
            float red,
            float green,
            float blue,
            float alpha = 1.0f
            );

    void                
        DiffuseColor(
            float red,
            float green,
            float blue,
            float alpha = 1.0f
            );

    void                
        SpecularColor(
            float red,
            float green,
            float blue,
            float alpha = 1.0f
            );


    void                
        AmbientColor(
            byte r,
            byte g,
            byte b,
            byte alpha = 255
            );

    const DirectX::XMFLOAT4&        
        AmbientColor(
            )const;

    void                
        DiffuseColor(
            byte r,
            byte g,
            byte b,
            byte alpha = 255
            );

    const DirectX::XMFLOAT4&        
        DiffuseColor(
            )const;

    void                
        SpecularColor(
            byte r,
            byte g,
            byte b,
            byte alpha = 255
            );

    const DirectX::XMFLOAT4&        
        SpecularColor(
            )const;

    
    void                
        Attenuation(
            float a0,
            float a1,
            float a2
            );

    const DirectX::XMFLOAT3   
        Attenuation(
            ) const;

    bool                
        HasPosition(
            ) const;

    void                
        HasPosition( 
            bool hasPos 
            );

    virtual void        
        UpdateViewPoint(
            );

    virtual bool        
        Initialize(
            ID3D11Device* device
            );

    ShaderLightType     
        Type(
            ) const;

    ShaderLight         
        AsShaderLight(
            );

    const char*         
        Name(
            ) const;

    static void         
        InitializeMOWClass(
            );

    std::string         
        ClassName(
            );

    static const char*      
        LightTypeToString(
            ShaderLightType type
            );

    static ShaderLightType  
        StringToLightType(
            const char* lightType
            );

    void
        ToPb(
            PbMOWGraphics::PbMOWLight& toPb,
            const CMOWPhysics& physics
            )const;

    virtual void
        Position(
            float xPos,
            float yPos,
            float zPos
            );

    virtual void
        LightScale(
            float x,
            float y,
            float z
            );

    virtual void
        LightScale(
            float uniformScale
            );

protected:

    virtual CMOWVariant*                     
        DoGetAttributeValue(
            const char* attrName
            );

    virtual void                            
        DoSetAttributeValue(
            const char* attrName,
            CMOWVariant* val
            );

    virtual void
        SetupModelAndPhysics(
            const CMOWPhysics& physics
            );

    static CMOWVariant*                      
        GetAmbientColorCB(
            const CMOWClass* aClass
            );

    static CMOWVariant*                      
        GetDiffuseColorCB(
            const CMOWClass* aClass
            );

    static CMOWVariant*                      
        GetSpecularColorCB(
            const CMOWClass* aClass
            );

    static CMOWVariant*                      
        GetPositionCB(
            const CMOWClass* aClass
            );

    static CMOWVariant*                      
        GetAttenuationCB(
            const CMOWClass* aClass
            );

    static CMOWVariant*                      
        GetRangeCB(
            const CMOWClass* aClass
            );

    static void                             
        SetAmbientColorCB(
            CMOWVariant* val,
            CMOWClass* aClass
            );

    static void                             
        SetDiffuseColorCB(
            CMOWVariant* val,
            CMOWClass* aClass
            );

    static void                             
        SetSpecularColorCB(
            CMOWVariant* val,
            CMOWClass* aClass
            );

    static void                             
        SetPositionCB(
            CMOWVariant* val,
            CMOWClass* aClass
            );

    static void                             
        SetAttenuationCB(
            CMOWVariant* val,
            CMOWClass* aClass
            );

    static void                             
        SetRangeCB(
            CMOWVariant* val,
            CMOWClass* aClass
            );

    ShaderLightType             
        m_type;

private:

    DirectX::XMFLOAT3           
        m_lightDirection;

    DirectX::XMFLOAT4           
        m_ambientColor;

    DirectX::XMFLOAT4           
        m_diffuseColor;

    DirectX::XMFLOAT4           
        m_specularColor;

    bool                        
        m_hasPosition;

    DirectX::XMFLOAT3           
        m_attenuation;

    std::string                 
        m_name;
};


#endif