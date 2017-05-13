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
    m_resources = 0;
    

}
//---------------------------------------------
CMOWTextureShader::~CMOWTextureShader()
{
    if( m_resources )
    {
        m_resources->Release();
        m_resources = 0;
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
void CMOWTextureShader::ApplyFXResources(std::vector<ID3D11ShaderResourceView*>* resources)
{
    if( m_fx && m_resources )
    {
        
        std::vector<ID3D11ShaderResourceView*> tempVec = *resources;
        m_resources->SetResourceArray(&resources->at(0),0,resources->size());
    }
}
//---------------------------------------
bool CMOWTextureShader::CreateFX(ID3D11Device* device)
{
    bool retVal = false;
    if( BaseClass::CreateFX(device) && m_fx )
    {
        m_resources = m_fx->GetVariableByName(ShaderTextureName())->AsShaderResource();
        retVal = m_resources != 0;
    }

    return retVal;
}
//---------------------------------------
const char* CMOWTextureShader::ShaderTextureName()
{
    return "diffuseTexture";
}
