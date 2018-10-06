#include "MOW2DObject.h"
using namespace DirectX;
//---------------------------------------------
CMOW2DObject::CMOW2DObject()
{
    m_vertexBuffer = 0;
    m_indexBuffer = 0;
    m_depthDisabledStencilState = 0;
}
//---------------------------------------------
CMOW2DObject::~CMOW2DObject()
{
    m_vertexBuffer->Release();
    m_indexBuffer->Release();
    m_depthDisabledStencilState->Release();
}
//---------------------------------------------
void CMOW2DObject::Initialize( 
    ID3D11Device* device,
    ID3D11DeviceContext* deviceContext,
    int screenWidth, 
    int screenHeight, 
    int width, 
    int height,
    int xPos,
    int yPos
    )
{

    HRESULT result;

    // Calculate the screen coordinates of the left side of the bitmap.
    float left = (float)((screenWidth / 2) * -1) + (float)xPos;

    // Calculate the screen coordinates of the right side of the bitmap.
    float right = left + (float)width;

    // Calculate the screen coordinates of the top of the bitmap.
    float top = (float)(screenHeight / 2) - (float)yPos;

    // Calculate the screen coordinates of the bottom of the bitmap.
    float bottom = top - (float)height;

    // Load the vertex array with data.
    // First triangle.

    Vertex vert;

    for( long i=0; i<6; i++ )
    {
        m_indices.push_back(i);
    }

    vert.m_position = XMFLOAT3(left, top, 0.0f);  // Top left.
    vert.m_texture = XMFLOAT2(0.0f, 0.0f);
    m_vertices.push_back(vert);

    vert.m_position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
    vert.m_texture = XMFLOAT2(1.0f, 1.0f);
    m_vertices.push_back(vert);

    vert.m_position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
    vert.m_texture = XMFLOAT2(0.0f, 1.0f);
    m_vertices.push_back(vert);

    // Second triangle.
    vert.m_position = XMFLOAT3(left, top, 0.0f);  // Top left.
    vert.m_texture = XMFLOAT2(0.0f, 0.0f);
    m_vertices.push_back(vert);

    vert.m_position = XMFLOAT3(right, top, 0.0f);  // Top right.
    vert.m_texture = XMFLOAT2(1.0f, 0.0f);
    m_vertices.push_back(vert);

    vert.m_position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
    vert.m_texture = XMFLOAT2(1.0f, 1.0f);
    m_vertices.push_back(vert);
    

    D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;

    // Clear the second depth stencil state before setting the parameters.
    ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

    // Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
    // that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
    depthDisabledStencilDesc.DepthEnable = false;
    depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthDisabledStencilDesc.StencilEnable = true;
    depthDisabledStencilDesc.StencilReadMask = 0xFF;
    depthDisabledStencilDesc.StencilWriteMask = 0xFF;
    depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    

    // Create the state using the device.
    result = device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);


    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    
    // Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_vertices.size();
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = &m_vertices[0];
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    
    // Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(long) * m_indices.size();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the index data.
    indexData.pSysMem = &m_indices[0];
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // Create the index buffer.
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

}
//---------------------------------------------
void CMOW2DObject::Render( 
    ID3D11DeviceContext* context,
    CMOWShader* shader,
    const XMFLOAT4X4& worldMatrix,
    const XMFLOAT4X4& viewMatrix,
    const XMFLOAT4X4& projectionMatrix,
    const XMFLOAT4X4& lightViewMatrix,
    const XMFLOAT4X4& lightProjectionMatrix,
    const DirectX::XMVECTOR& cameraPosition,
    const CMOWShader::LightBufferDefinition& lightBuff,
    int screenWidth,
    int screenHeight,
    std::vector<ID3D11ShaderResourceView*>* resources
    )const
{

    unsigned int stride = sizeof(Vertex);
    unsigned int offset = 0;
    unsigned int oldRef = 0;
    ID3D11DepthStencilState* oldDepthStencilState = 0;

    context->OMGetDepthStencilState(&oldDepthStencilState,&oldRef);

    //Turn off ZBuffer
    context->OMSetDepthStencilState(m_depthDisabledStencilState,1);

    // Set the vertex buffer to active in the input assembler so it can be rendered.
    context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
    context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    shader->Update(context,
                   worldMatrix,
                   viewMatrix,
                   projectionMatrix,
                   lightViewMatrix,
                   lightProjectionMatrix,
                   lightBuff,
                   cameraPosition,
                   screenWidth,
                   screenHeight,
                   resources
                   );
    

    

    context->DrawIndexed(m_indices.size(),0,0);

    //Restore old DepthStencilState
    context->OMSetDepthStencilState(oldDepthStencilState,oldRef);
}

