#ifndef DXLightShader_H
#define DXLightShader_H

#include "MOWGraphics/MOWCommon.h"
#include "MOWTextureShader.h"



class CMOWLightShader : public CMOWTextureShader
{
    typedef CMOWTextureShader BaseClass;
public:
    
    CMOWLightShader(
        );

    virtual ~CMOWLightShader(
                );


protected:
    
    virtual void                    
        CreateBuffers(
            ID3D11Device* device
            );

    virtual void                    
        GetVSBuffers(
            std::vector<ID3D11Buffer*>& buffers
            );

    virtual void                    
        GetPSBuffers(
            std::vector<ID3D11Buffer*>& buffers
            );

    virtual HRESULT                 
        SetupBuffer( 
            ID3D11Buffer* buffer, 
            ID3D11DeviceContext* context, 
            DirectX::XMMATRIX& worldMatrix, 
            DirectX::XMMATRIX& viewMatrix, 
            DirectX::XMMATRIX& projectionMatrix,
            DirectX::XMMATRIX& lightViewMatrix,
            DirectX::XMMATRIX& lightProjectionMatrix,
            LightBufferDefinition& lightDef,
            DirectX::XMFLOAT3 cameraPos,
            int screenWidth,
            int screenHeight
            );

    virtual void                    
        CreateSamplerStates(
            ID3D11Device* device,
            std::vector<ID3D11SamplerState*>& samplerStates
            );

    virtual void                    
        ApplyFXResources(
            std::vector<ID3D11ShaderResourceView*>* resources
            );

    virtual bool                    
        CreateFX(
            ID3D11Device* device
            );



private:

    HRESULT                         
        SetupLightBuffer(
            ID3D11Buffer* buffer,
            ID3D11DeviceContext* context,
            LightBufferDefinition& lightDef
            );

    ID3D11Buffer*                           
        m_lightBufferPS;

    ID3D11Buffer*                           
        m_lightBufferVS;

    ID3D11Buffer*                           
        m_cameraPosBuffer;

    ID3DX11EffectShaderResourceVariable*    
        m_colorResource;

    ID3DX11EffectShaderResourceVariable*    
        m_ambientResource;

    ID3DX11EffectShaderResourceVariable*    
        m_diffuseResource;

    ID3DX11EffectShaderResourceVariable*    
        m_specularResource;

    ID3DX11EffectShaderResourceVariable*    
        m_normalResource;

    ID3DX11EffectShaderResourceVariable*    
        m_positionResource;

    bool                                    
        m_isInitialized;

};

#endif