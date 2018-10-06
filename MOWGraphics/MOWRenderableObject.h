#ifndef CDXRenderableObject_H
#define CDXRenderableObject_H

#include "MOWCommon.h"
#include "MOWShaders\MOWShader.h"

class CMOWRenderableObject
{
public:
    CMOWRenderableObject(
        );

    virtual                         
        ~CMOWRenderableObject(
            );

    virtual bool                    
        Render(
            ID3D11DeviceContext* context,
            CMOWShader* shader,
            const DirectX::XMFLOAT4X4& viewMatrix,
            const DirectX::XMFLOAT4X4& projectionMatrix,
            const DirectX::XMFLOAT4X4& lightViewMatrix,
            const DirectX::XMFLOAT4X4& lightProjectionMatrix,
            const DirectX::XMVECTOR& cameraPosition,
            CMOWShader::LightBufferDefinition& lightBuff,
            int screenWidth,
            int screenHeight,
            std::vector<ID3D11ShaderResourceView*>* resources = 0,
            bool usePixelShader = true
            )const;

    virtual bool                    
        Render(
            ID3D11DeviceContext* context,
            CMOWShader* shader,
            const DirectX::XMFLOAT4X4& viewMatrix,
            const DirectX::XMFLOAT4X4& projectionMatrix,
            const DirectX::XMFLOAT4X4& lightViewMatrix,
            const DirectX::XMFLOAT4X4& lightProjectionMatrix,
            const DirectX::XMVECTOR& cameraPosition,
            ShaderLight& light,
            int screenWidth,
            int screenHeight,
            std::vector<ID3D11ShaderResourceView*>* resources,
            bool usePixelShader = true,
            bool useAdjacentIndecies = false
            )const;
};

#endif