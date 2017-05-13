#ifndef CDXMaterial_H
#define CDXMaterial_H

#include "MOWCommon.h"
#include "MOWClass.h"
#include <string>
#include <iostream>
#include <fstream>
#include "MOWVariant.h"


class CMOWMaterial : 
    public CMOWClass
{
public:

    CMOWMaterial();

    CMOWMaterial(
        const char* name,
        const char* texture,
        DirectX::XMFLOAT4 Ka,
        DirectX::XMFLOAT4 Kd,
        DirectX::XMFLOAT4 Ks,
        DirectX::XMFLOAT4 Ke,
        float specularPower
        );

    ~CMOWMaterial();

    const DirectX::XMFLOAT4&            
        Ka(
            ) const;

    const DirectX::XMFLOAT4&            
        Kd(
            ) const;

    const DirectX::XMFLOAT4&            
        Ks(
            ) const;

    const DirectX::XMFLOAT4&            
        Ke(
            ) const;

    void                    
        Ka(
            DirectX::XMFLOAT4 Ka
            );

    void                    
        Kd(
            DirectX::XMFLOAT4 Kd
            );

    void                    
        Ks(
            DirectX::XMFLOAT4 Ks
            );

    void                    
        Ke(
            DirectX::XMFLOAT4 Ke
            );

    const char*             
        TextureFileName(
            ) const;

    void                    
        TextureFileName(
            const char* textureFileName
            );

    const char*             
        Name(
            ) const;

    void                    
        Name(
            const char* name
            );

    float                   
        SpecularPower(
            ) const;

    void                    
        SpecularPower(
            float specularPower
            );

    bool                    
        Serialize(
            std::ofstream& fOut
            );

    bool                    
        Serialize(
            std::ifstream& fIn
            );

    ShaderMaterial          
        AsShaderMaterial(
            )const;

    static void             
        InitializeMOWClass(
            );

    virtual std::string     
        ClassName(
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

private:

    std::string             
        m_name;

    std::string             
        m_texture;

    DirectX::XMFLOAT4       
        m_Ka;

    DirectX::XMFLOAT4       
        m_Kd;

    DirectX::XMFLOAT4       
        m_Ks;

    DirectX::XMFLOAT4       
        m_Ke;

    float                   
        m_specularPower;
};

#endif