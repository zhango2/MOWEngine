#include "MOWTextureShader.h"
#include "Effects11/inc/d3dx11effect.h"
//---------------------------------------------
CMOWTextureShader::CMOWTextureShader()
{
    m_vertexShaderDescription = "TextureVertexShader";
    m_pixelShaderDescription = "TexturePixelShader";
    m_vertexShaderFilename = "TextureVertexShader.cso";
    m_pixelShaderFilename = "TexturePixelShader.cso";
    m_fxFileName = "Texture.fxo";
    m_fxTechName = "TextureTech";
    m_albedoResource = nullptr;
    m_normalResource = nullptr;
    m_metallicRoughHeightResource = nullptr;
    

}
//---------------------------------------------
CMOWTextureShader::~CMOWTextureShader()
{
    if( m_albedoResource )
    {
        m_albedoResource->Release();
        m_albedoResource = nullptr;
    }

    if( m_normalResource )
    {
        m_normalResource->Release();
        m_normalResource = nullptr;
    }

    if( m_metallicRoughHeightResource )
    {
        m_metallicRoughHeightResource->Release();
        m_metallicRoughHeightResource = nullptr;
    }
}
//---------------------------------------------
void CMOWTextureShader::CreateSamplerStates(
    ID3D11Device* device,
    std::vector<ID3D11SamplerState*>& samplerStates
    )
{

    

    BaseClass::CreateSamplerStates(device,samplerStates);

    ID3D11SamplerState* state = 0;
    D3D11_SAMPLER_DESC samplerDesc;

    // Create a texture sampler state description.
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
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
void CMOWTextureShader::ApplyFXResources(const std::vector<ID3D11ShaderResourceView*>* resources)
{
    if( m_fx )
    {
        if( m_albedoResource && resources->size() >= 1)
        {
            m_albedoResource->SetResource(resources->at(0));
        }
        if( m_normalResource && resources->size() >= 2 )
        {
            m_normalResource->SetResource(resources->at(1));
        }
        if( m_metallicRoughHeightResource && resources->size() >= 3 )
        {
            m_metallicRoughHeightResource->SetResource(resources->at(2));
        }
        
    }
    

}
//---------------------------------------
bool CMOWTextureShader::CreateFX(ID3D11Device* device)
{
    bool retVal = false;
    if( BaseClass::CreateFX(device) && m_fx )
    {
        m_albedoResource = m_fx->GetVariableByName(ShaderAlbedoTextureName())->AsShaderResource();
        m_normalResource = m_fx->GetVariableByName(ShaderNormalTextureName())->AsShaderResource();
        m_metallicRoughHeightResource = m_fx->GetVariableByName(ShaderMetallicRoughHeightTextureName())->AsShaderResource();
        retVal = m_albedoResource && m_normalResource && m_metallicRoughHeightResource;
    }

    return retVal;
}
//---------------------------------------
const char* CMOWTextureShader::ShaderAlbedoTextureName()
{
    return "albedoTexture";
}
//------------------------------------------------------
const char* CMOWTextureShader::ShaderNormalTextureName()
{
    return "normalTexture";
}
//------------------------------------------------------
const char* CMOWTextureShader::ShaderMetallicRoughHeightTextureName()
{
    return "metallicRoughHeightTexture";
}
