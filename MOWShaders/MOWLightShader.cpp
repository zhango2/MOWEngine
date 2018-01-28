#include "MOWLightShader.h"
#include "Effects11/inc/d3dx11effect.h"
using namespace DirectX;
//---------------------------------------------
CMOWLightShader::CMOWLightShader()
{
    m_vertexShaderDescription = "LightVertexShader";
    m_pixelShaderDescription = "LightPixelShader";
    m_vertexShaderFilename = "LightVertexShader.cso";
    m_pixelShaderFilename = "LightPixelShader.cso";
    m_fxFileName = "Light.fxo";
    m_fxTechName = "LightTech";
    m_fxTechNoPixelShaderName = "LightTechNoPixelShader";
    m_isInitialized = false;
}
//---------------------------------------------
CMOWLightShader::~CMOWLightShader()
{

}
//---------------------------------------------
void CMOWLightShader::CreateBuffers(
    ID3D11Device* device
    )
{
    BaseClass::CreateBuffers(device);
    D3D11_BUFFER_DESC lightBufferDesc;
    D3D11_BUFFER_DESC cameraPosBufferDesc;

    lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    lightBufferDesc.ByteWidth = sizeof(LightBufferDefinition);
    lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    lightBufferDesc.MiscFlags = 0;
    lightBufferDesc.StructureByteStride = 0;

    // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
    device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBufferPS);
    device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBufferVS);

    cameraPosBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    cameraPosBufferDesc.ByteWidth = sizeof(CameraBufferDefinition);
    cameraPosBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cameraPosBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cameraPosBufferDesc.MiscFlags = 0;
    cameraPosBufferDesc.StructureByteStride = 0;

    device->CreateBuffer(&cameraPosBufferDesc, NULL, &m_cameraPosBuffer);
}
//---------------------------------------------
void CMOWLightShader::GetVSBuffers( 
    std::vector<ID3D11Buffer*>& buffers 
    )
{
    BaseClass::GetVSBuffers(buffers);
    buffers.push_back(m_lightBufferVS);
    buffers.push_back(m_cameraPosBuffer);
}
//---------------------------------------------
void CMOWLightShader::GetPSBuffers(
    std::vector<ID3D11Buffer*>& buffers
    )
{
    BaseClass::GetPSBuffers(buffers);
    buffers.push_back(m_lightBufferPS);
}
//---------------------------------------------
HRESULT CMOWLightShader::SetupBuffer( 
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
    HRESULT result = BaseClass::SetupBuffer(buffer,
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

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    
    if( buffer == m_lightBufferPS || buffer == m_lightBufferVS )
    {
        result = SetupLightBuffer(buffer,context,lightDef);
        
    }
    else if( buffer == m_cameraPosBuffer )
    {
        CameraBufferDefinition* cameraPosBufferDef = 0;
        result = context->Map(m_cameraPosBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        if( !FAILED(result) )
        {
            cameraPosBufferDef = reinterpret_cast<CameraBufferDefinition*>(mappedResource.pData);

            cameraPosBufferDef->cameraPosition = cameraPos;
            cameraPosBufferDef->padding = 0.0f;

            context->Unmap(m_cameraPosBuffer,0);

            // Now set the camera constant buffer in the vertex shader with the updated values.
            //context->VSSetConstantBuffers(0, 1, &m_cameraPosBuffer);

        }
    }

    return result;
}
//---------------------------------------------
HRESULT CMOWLightShader::SetupLightBuffer( 
    ID3D11Buffer* buffer, 
    ID3D11DeviceContext* context,
    LightBufferDefinition& lightDef 
    )
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    LightBufferDefinition* lightBufferDef = 0;
    HRESULT result = context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if(!FAILED(result))
    {
        // Get a pointer to the data in the constant buffer.
        lightBufferDef = reinterpret_cast<LightBufferDefinition*>(mappedResource.pData);

        // Copy light info

        lightBufferDef->lightAmbientColor = lightDef.lightAmbientColor;
        lightBufferDef->lightDiffuseColor = lightDef.lightDiffuseColor;
        lightBufferDef->lightSpecularColor = lightDef.lightSpecularColor;
        lightBufferDef->lightDirection = lightDef.lightDirection;
        lightBufferDef->lightPosition = lightDef.lightPosition;
        lightBufferDef->hasPosition = lightDef.hasPosition;
        lightBufferDef->Ka = lightDef.Ka;
        lightBufferDef->Kd = lightDef.Kd;
        lightBufferDef->Ks = lightDef.Ks;
        lightBufferDef->Ke = lightDef.Ke;
        lightBufferDef->specularPower = lightDef.specularPower;


        // Unlock the constant buffer.
        context->Unmap(buffer, 0);

    }
    return result;
}
//---------------------------------------------
void CMOWLightShader::CreateSamplerStates( 
    ID3D11Device* device, 
    std::vector<ID3D11SamplerState*>& samplerStates 
    )
{
    BaseClass::CreateSamplerStates(device,samplerStates);

    ID3D11SamplerState* state = 0;
    D3D11_SAMPLER_DESC samplerDesc;

    // Create a clamp texture sampler state description.
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Create the texture sampler state.
    device->CreateSamplerState(&samplerDesc, &state);

    samplerStates.push_back(state);

}
//---------------------------------------
void CMOWLightShader::ApplyFXResources(
    std::vector<ID3D11ShaderResourceView*>* resources
    )
{
    if( resources && resources->size() >= 4 && !m_isInitialized)
    {
        m_colorResource->SetResource(resources->at(0));
        m_normalResource->SetResource(resources->at(1));
        m_metalRoughHeightResource->SetResource(resources->at(2));
        m_positionResource->SetResource(resources->at(3));
        m_isInitialized = true;
    }
}
//---------------------------------------
bool CMOWLightShader::CreateFX(ID3D11Device* device)
{
    bool retVal = false;
    if( CMOWShader::CreateFX(device) && m_fx && !m_isInitialized )
    {
        m_colorResource = m_fx->GetVariableByName("albedoTexture")->AsShaderResource();
        m_metalRoughHeightResource = m_fx->GetVariableByName("metalRoughHeightTexture")->AsShaderResource();
        m_normalResource = m_fx->GetVariableByName("normalTexture")->AsShaderResource();
        m_positionResource = m_fx->GetVariableByName("positionTexture")->AsShaderResource();

        retVal = m_colorResource && m_positionResource && m_normalResource;
        
    }

    return retVal;
}


