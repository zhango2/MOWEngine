#ifndef CDXMaterial_H
#define CDXMaterial_H

#include "MOWCommon.h"
#include "MOWClass.h"
#include <string>
#include <iostream>
#include <fstream>
#include "MOWVariant.h"
#include <map>


class CMOWMaterial : 
    public CMOWClass
{
public:

    enum TEXTURE_TYPE
    {
        TT_NONE,
        TT_BASE_COLOR,
        TT_NORMAL,
        TT_METALLIC_ROUGHNESS_HEIGHT,
        TT_LAST
    };

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
            TEXTURE_TYPE textureType = TT_BASE_COLOR
            ) const;

    void                    
        TextureFileName(
            const char* textureFileName,
            TEXTURE_TYPE textureType = TT_BASE_COLOR
            );

    static const std::string&
        TextureTypeNameFromType(
            TEXTURE_TYPE textureType
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

    static TEXTURE_TYPE
        TextureTypeFromName(
            const char* textureName
            );

    const std::map<TEXTURE_TYPE,std::string>&
        Textures(
            ) const;


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

    const std::string&
        PrivateTextureFileName(
            TEXTURE_TYPE textureType = TT_BASE_COLOR
            )const;

private:

    std::map<TEXTURE_TYPE,std::string>
        m_textureFilenameByTextureType;

    std::string
        m_emptyTexture;

    std::string             
        m_name;

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