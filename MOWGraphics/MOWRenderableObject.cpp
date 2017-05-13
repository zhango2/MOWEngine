#include "MOWRenderableObject.h"
//------------------------------------------------------
CMOWRenderableObject::CMOWRenderableObject()
{

}
//------------------------------------------------------
CMOWRenderableObject::~CMOWRenderableObject()
{

}
//------------------------------------------------------
bool CMOWRenderableObject::Render(
    ID3D11DeviceContext* context, 
    CMOWShader* shader, 
    const DirectX::XMFLOAT4X4& viewMatrix, 
    const DirectX::XMFLOAT4X4& projectionMatrix, 
    const DirectX::XMFLOAT4X4& lightViewMatrix, 
    const DirectX::XMFLOAT4X4& lightProjectionMatrix, 
    const DirectX::XMFLOAT3& cameraPosition, 
    CMOWShader::LightBufferDefinition& lightBuff, 
    int screenWidth, 
    int screenHeight, 
    std::vector<ID3D11ShaderResourceView*>* resources, 
    bool usePixelShader
    )const
{
    return true;
}
//------------------------------------------------------
bool CMOWRenderableObject::Render(
    ID3D11DeviceContext* context, 
    CMOWShader* shader, 
    const DirectX::XMFLOAT4X4& viewMatrix, 
    const DirectX::XMFLOAT4X4& projectionMatrix, 
    const DirectX::XMFLOAT4X4& lightViewMatrix, 
    const DirectX::XMFLOAT4X4& lightProjectionMatrix, 
    const DirectX::XMFLOAT3& cameraPosition, 
    ShaderLight& light, 
    int screenWidth, 
    int screenHeight, 
    std::vector<ID3D11ShaderResourceView*>* resources, 
    bool usePixelShader,
    bool useAdjacentIndecies
    )const
{
    return true;
}