#include "MOWDeferredShader.h"
using namespace DirectX;
//---------------------------------------------
CMOWDeferredShader::CMOWDeferredShader()
{
    m_vertexShaderDescription = "DeferredVertexShader";
    m_pixelShaderDescription = "DeferredPixelShader";
    m_vertexShaderFilename = "DeferredVertexShader.cso";
    m_pixelShaderFilename = "DeferredPixelShader.cso";
    m_fxFileName = "Deferred.fxo";
    m_fxTechName = "DeferredTech";
}
//---------------------------------------------
CMOWDeferredShader::~CMOWDeferredShader()
{
}

//---------------------------------------------
void CMOWDeferredShader::CreateBuffers( ID3D11Device* device )
{
    BaseClass::CreateBuffers(device);

    D3D11_BUFFER_DESC cameraPosBufferDesc;

    cameraPosBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    cameraPosBufferDesc.ByteWidth = sizeof(CameraBufferDefinition);
    cameraPosBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cameraPosBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cameraPosBufferDesc.MiscFlags = 0;
    cameraPosBufferDesc.StructureByteStride = 0;

    device->CreateBuffer(&cameraPosBufferDesc, NULL, &m_cameraPosBuffer);
}
//---------------------------------------------
void CMOWDeferredShader::GetVSBuffers( std::vector<ID3D11Buffer*>& buffers )
{
    BaseClass::GetVSBuffers(buffers);

    buffers.push_back(m_cameraPosBuffer);
}
//---------------------------------------------
HRESULT CMOWDeferredShader::SetupBuffer( 
    ID3D11Buffer* buffer, 
    ID3D11DeviceContext* context, 
    XMMATRIX& worldMatrix, 
    XMMATRIX& viewMatrix, 
    XMMATRIX& projectionMatrix, 
    XMMATRIX& lightViewMatrix, 
    XMMATRIX& lightProjectionMatrix, 
    LightBufferDefinition& lightDef, 
    XMFLOAT3 cameraPos,
    int screenWidth,
    int screenHeight
    )
{
    HRESULT result = S_OK;
    BaseClass::SetupBuffer(buffer,
                           context,
                           worldMatrix,
                           viewMatrix,
                           projectionMatrix,
                           lightViewMatrix,
                           lightProjectionMatrix,
                           lightDef,
                           cameraPos,
                           screenWidth,
                           screenHeight
                           );

    if( buffer == m_cameraPosBuffer )
    {
        D3D11_MAPPED_SUBRESOURCE mappedResource;

        CameraBufferDefinition* cameraPosBufferDef = 0;
        result = context->Map(m_cameraPosBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        if( !FAILED(result) )
        {
            cameraPosBufferDef = reinterpret_cast<CameraBufferDefinition*>(mappedResource.pData);

            cameraPosBufferDef->cameraPosition = cameraPos;
            cameraPosBufferDef->padding = 0.0f;

            context->Unmap(m_cameraPosBuffer,0);

        }
    }

    return result;
}
//---------------------------------------
const char* CMOWDeferredShader::ShaderAlbedoTextureName()
{
    return "albedoTexture";
}




