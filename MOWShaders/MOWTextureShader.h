#ifndef CDXTextureShader_H
#define CDXTextureShader_H

#include "MOWShader.h"

struct ID3DX11EffectShaderResourceVariable;

class CMOWTextureShader : public CMOWShader
{

    typedef CMOWShader BaseClass;
public:
    
    CMOWTextureShader(
        );

    virtual ~CMOWTextureShader(
                );


protected:

    virtual void                    
        CreateSamplerStates(
            ID3D11Device* device,
            std::vector<ID3D11SamplerState*>& samplerStates
            );

    virtual bool                    
        CreateFX(
            ID3D11Device* device
            );

    virtual void                    
        ApplyFXResources(
            std::vector<ID3D11ShaderResourceView*>* resources
            );

    virtual const char*             
        ShaderTextureName(
            );

    ID3DX11EffectShaderResourceVariable*    
        m_resources;

};
#endif