#ifndef MOW2DObject_H
#define MOW2DObject_H

#include "MOWCommon.h"
#include "MOWObject.h"
#include "MOWShaders\MOWShader.h"

class CMOW2DObject : public CMOWObject
{
    typedef CMOWObject BaseClass;

public:

    CMOW2DObject(
        );

    virtual             
        ~CMOW2DObject(
            );

    void                
        Initialize(
            ID3D11Device* device,
            ID3D11DeviceContext* deviceContext,
            int screenWidth,
            int screenHeight,
            int width,
            int height,
            int xPos,
            int yPos
            );

    void                
        Render(
            ID3D11DeviceContext* context,
            CMOWShader* shader,
            const DirectX::XMFLOAT4X4& worldMatrix,
            const DirectX::XMFLOAT4X4& viewMatrix,
            const DirectX::XMFLOAT4X4& projectionMatrix,
            const DirectX::XMFLOAT4X4& lightViewMatrix,
            const DirectX::XMFLOAT4X4& lightProjectionMatrix,
            const DirectX::XMFLOAT3& cameraPosition,
            const CMOWShader::LightBufferDefinition& lightBuff,
            int screenWidth,
            int screenHeight,
            std::vector<ID3D11ShaderResourceView*>* resources
            )const;

private:

    ID3D11Buffer*               
        m_vertexBuffer;

    ID3D11Buffer*               
        m_indexBuffer;

    ID3D11DepthStencilState*    
        m_depthDisabledStencilState;
    
    vertex_vector               
        m_vertices;

    index_vector                
        m_indices;
};

#endif