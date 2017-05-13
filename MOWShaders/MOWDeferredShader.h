#ifndef DXDeferredShader_H
#define DXDeferredShader_H

#include "MOWGraphics/MOWCommon.h"
#include "MOWTextureShader.h"

class CMOWDeferredShader : public CMOWTextureShader
{
    typedef CMOWTextureShader BaseClass;
public:

    CMOWDeferredShader(
        );

    virtual ~CMOWDeferredShader(
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

    virtual const char*             
        ShaderTextureName(
            );

private:

    ID3D11Buffer*                           
        m_cameraPosBuffer;
    

};

#endif