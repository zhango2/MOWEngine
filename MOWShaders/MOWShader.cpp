#include "MOWShader.h"
#include "Effects11/inc/d3dx11effect.h"
#include <iostream>
#include <fstream>
#include "MOWCommon/MOWStringConverter.h"

using namespace DirectX;

CMOWShader::CMOWShader()
{
    m_resourceCount = 0; 
    m_fx = 0;
    m_fxTech = 0;
    m_fxTechNoPixelShader = 0;
}

CMOWShader::~CMOWShader()
{
    if( m_fx )
    {
        m_fx->Release();
    }
    if( m_fxTech )
    {
        m_fxTech->Release();
    }
    if( m_fxTechNoPixelShader )
    {
        m_fxTechNoPixelShader->Release();
    }
}

bool CMOWShader::Initialize(
    ID3D11Device* device
    )
{
    bool retVal = false;
    
    std::vector<D3D11_INPUT_ELEMENT_DESC> descriptions;
    HRESULT result;
    unsigned int numElements;

    if( CreateFX(device) )
    {
        
        SetupElementDescriptions(descriptions);


        // Get a count of the elements in the layout.
        numElements = descriptions.size();

        // Create the vertex input layout.
        D3DX11_PASS_DESC passDesc;
        m_fxTech->GetPassByIndex(0)->GetDesc(&passDesc);
        if( !FAILED(device->CreateInputLayout(&descriptions[0], descriptions.size(), passDesc.pIAInputSignature, 
                                                  passDesc.IAInputSignatureSize, &m_layout)))
        {
            retVal = true;
        }

    }
    else
    {
        
        std::vector<byte> vertexShaderData = GetShaderBytes(VertexShaderFilename());

        if(vertexShaderData.size())
        {

            // Create the vertex shader from the buffer.
            result = device->CreateVertexShader(&vertexShaderData[0], vertexShaderData.size(), NULL, &m_vertexShader);
            if(!FAILED(result))
            {
                std::vector<byte> pixelShaderData = GetShaderBytes(PixelShaderFilename());
                // Create the pixel shader from the buffer.
                result = device->CreatePixelShader(&pixelShaderData[0], pixelShaderData.size(), NULL, &m_pixelShader);
                if(!FAILED(result))
                {

                    // Now setup the layout of the data that goes into the shader.
                    SetupElementDescriptions(descriptions);


                    // Get a count of the elements in the layout.
                    numElements = descriptions.size();

                    // Create the vertex input layout.
                    result = device->CreateInputLayout(&descriptions[0], numElements, &vertexShaderData[0], 
                        vertexShaderData.size(), &m_layout);

                    if(!FAILED(result))
                    {    
                        CreateBuffers(device);

                        if(!FAILED(result))
                        {
                            CreateSamplerStates(device,m_samplerStates);
                            retVal = true;
                        }
                    }
                }

            }

        }
    }

    
    return retVal;
}

bool CMOWShader::Update(
    ID3D11DeviceContext* context,
    const DirectX::XMFLOAT4X4& worldMatrix,
    const DirectX::XMFLOAT4X4& viewMatrix,
    const DirectX::XMFLOAT4X4& projectionMatrix,
    const DirectX::XMFLOAT4X4& lightViewMatrix,
    const DirectX::XMFLOAT4X4& lightProjectionMatrix,
    const LightBufferDefinition& lightDef,
    const XMFLOAT3& cameraPos,
    int screenWidth,
    int screenHeight,
    std::vector<ID3D11ShaderResourceView*>* resources,
    bool usePixelShader
    )
{
    bool retVal = false;
    HRESULT result;
	
	

    

    if( m_fx )
    {
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        // Set the vertex input layout.
        context->IASetInputLayout(m_layout);
        
        D3DX11_TECHNIQUE_DESC techDesc;
        m_fxTech->GetDesc( &techDesc );
        for(UINT p = 0; p < techDesc.Passes; p++)
        {
            
            m_fxVariables.world->SetMatrix(reinterpret_cast<float*>(&XMLoadFloat4x4(&worldMatrix)));
            m_fxVariables.projection->SetMatrix(reinterpret_cast<float*>(&XMLoadFloat4x4(&projectionMatrix)));
            m_fxVariables.view->SetMatrix(reinterpret_cast<float*>(&XMLoadFloat4x4(&viewMatrix)));
            m_fxVariables.screenWidth->SetInt(screenWidth);
            m_fxVariables.screenHeight->SetInt(screenHeight);
            if( m_fxVariables.material )
            {
                ShaderMaterial mat;

                mat.m_ambient = lightDef.Ka;
                mat.m_diffuse = lightDef.Kd;
                mat.m_specular = lightDef.Ks;
                mat.m_specular.w = lightDef.specularPower;

                m_fxVariables.material->SetRawValue(&mat,0,sizeof(ShaderMaterial));
            }
            ApplyFXResources(resources);
            m_fxTech->GetPassByIndex(p)->Apply(0, context);
        }
    }
    else
    {
        // Transpose the matrices to prepare them for the shader.
        XMMATRIX newWorldMatrix = XMMatrixTranspose(XMLoadFloat4x4(&worldMatrix));
        XMMATRIX newViewMatrix = XMMatrixTranspose(XMLoadFloat4x4(&viewMatrix));
        XMMATRIX newProjectionMatrix= XMMatrixTranspose(XMLoadFloat4x4(&projectionMatrix));
        XMMATRIX newLightViewMatrix = XMMatrixTranspose(XMLoadFloat4x4(&lightViewMatrix));
        XMMATRIX newLightProjectionMatrix = XMMatrixTranspose(XMLoadFloat4x4(&lightProjectionMatrix));
        LightBufferDefinition newLightDef;
        memcpy(&newLightDef, &lightDef, sizeof(LightBufferDefinition));

        std::vector<ID3D11Buffer*> vsBuffers;
        std::vector<ID3D11Buffer*> psBuffers;

        GetVSBuffers(vsBuffers);
        GetPSBuffers(psBuffers);

        for( size_t i = 0; i< vsBuffers.size(); i++ )
        {
            result = SetupBuffer(vsBuffers[i],
                context,
                newWorldMatrix,
                newViewMatrix,
                newProjectionMatrix,
                newLightViewMatrix,
                newLightProjectionMatrix,
                newLightDef,
                cameraPos,
                screenWidth,
                screenHeight
                );
        }

        for( size_t i = 0; i< psBuffers.size(); i++ )
        {
            result = SetupBuffer(psBuffers[i],
                context,
                newWorldMatrix,
                newViewMatrix,
                newProjectionMatrix,
                newLightViewMatrix,
                newLightProjectionMatrix,
                newLightDef,
                cameraPos,
                screenWidth,
                screenHeight
                );
        }

        if( vsBuffers.size() )
        {
            context->VSSetConstantBuffers(0,vsBuffers.size(),&vsBuffers[0]);
        }
        if( psBuffers.size() )
        {
            context->PSSetConstantBuffers(0,psBuffers.size(),&psBuffers[0]);
        }



        if( !FAILED( result ) )
        {
            // Set the vertex input layout.
            context->IASetInputLayout(m_layout);

            if( UseResources() )
            {
                if( resources && resources->size() )
                {
                    context->PSSetShaderResources(0, resources->size(), &((*resources)[0]));

                }
                if( m_samplerStates.size() )
                {
                    context->PSSetSamplers(0, m_samplerStates.size(), &m_samplerStates[0]);
                }

                m_resourceCount = resources ? resources->size() : 0;
            }


            // Set the vertex and pixel shaders that will be used to render.
            context->VSSetShader(m_vertexShader, NULL, 0);
            context->PSSetShader(usePixelShader ? m_pixelShader : 0, NULL, 0);

            retVal = true;
        }
    }

    
	return retVal;

}
//---------------------------------------
bool CMOWShader::Update(
    ID3D11DeviceContext* context, 
    const DirectX::XMFLOAT4X4& worldMatrix, 
    const DirectX::XMFLOAT4X4& viewMatrix, 
    const DirectX::XMFLOAT4X4& projectionMatrix, 
    const DirectX::XMFLOAT4X4& lightViewMatrix, 
    const DirectX::XMFLOAT4X4& lightProjectionMatrix, 
    const ShaderLight& light,
    const ShaderMaterial& material,
    const DirectX::XMFLOAT3 cameraPosition, 
    int screenWidth, 
    int screenHeight, 
    std::vector<ID3D11ShaderResourceView*>* resources,
    bool usePixelShader
    )
{

    if( m_fx )
    {
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        // Set the vertex input layout.
        context->IASetInputLayout(m_layout);

        D3DX11_TECHNIQUE_DESC techDesc;
        ID3DX11EffectTechnique* tech = usePixelShader ? m_fxTech : m_fxTechNoPixelShader;

        if( tech )
        {
            tech->GetDesc( &techDesc );
            for(UINT p = 0; p < techDesc.Passes; p++)
            {

                m_fxVariables.world->SetMatrix(reinterpret_cast<float*>(&XMLoadFloat4x4(&worldMatrix)));
                m_fxVariables.projection->SetMatrix(reinterpret_cast<float*>(&XMLoadFloat4x4(&projectionMatrix)));
                m_fxVariables.view->SetMatrix(reinterpret_cast<float*>(&XMLoadFloat4x4(&viewMatrix)));
                m_fxVariables.screenWidth->SetInt(screenWidth);
                m_fxVariables.screenHeight->SetInt(screenHeight);
                m_fxVariables.lightView->SetMatrix(reinterpret_cast<float*>(&XMLoadFloat4x4(&lightViewMatrix)));
                m_fxVariables.lightProjection->SetMatrix(reinterpret_cast<float*>(&XMLoadFloat4x4(&lightProjectionMatrix)));
                m_fxVariables.cameraPosition->SetRawValue(&cameraPosition,0,sizeof(cameraPosition));
                if( m_fxVariables.material )
                {
                    m_fxVariables.material->SetRawValue(&material,0,sizeof(ShaderMaterial));
                }
                if( m_fxVariables.light )
                {
                    m_fxVariables.light->SetRawValue(&light,0,sizeof(ShaderLight));
                }
                ApplyFXResources(resources);
                tech->GetPassByIndex(p)->Apply(0, context);
            }
        }

        
    }

    return true;
}

//---------------------------------------------
void CMOWShader::CreateSamplerStates(
    ID3D11Device* device,
    std::vector<ID3D11SamplerState*>& samplerStates
    )
{
    //Do nothing
}
//---------------------------------------------
void CMOWShader::SetupElementDescriptions( 
    std::vector<D3D11_INPUT_ELEMENT_DESC>& descriptions 
    )
{
    
    // This setup needs to match the VertexType structure in the ModelClass and in the shader.
    
    descriptions.push_back(CreateVertexElementDesc("POSITION",DXGI_FORMAT_R32G32B32_FLOAT,0));
    descriptions.push_back(CreateVertexElementDesc("TEXCOORD",DXGI_FORMAT_R32G32_FLOAT));
    descriptions.push_back(CreateVertexElementDesc("NORMAL",DXGI_FORMAT_R32G32B32_FLOAT));
    descriptions.push_back(CreateVertexElementDesc("TANGENT",DXGI_FORMAT_R32G32B32_FLOAT));
    descriptions.push_back(CreateVertexElementDesc("BINORMAL",DXGI_FORMAT_R32G32B32_FLOAT));
    descriptions.push_back(CreateVertexElementDesc("COLOR",DXGI_FORMAT_R32G32B32A32_FLOAT));
}
//---------------------------------------------
void CMOWShader::CreateBuffers(
    ID3D11Device* device
    )
{
    D3D11_BUFFER_DESC matrixBufferDesc;
    // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferDefinition);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
    device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
}
//---------------------------------------------
void CMOWShader::GetVSBuffers( 
    std::vector<ID3D11Buffer*>& buffers 
    )
{
    buffers.push_back(m_matrixBuffer);
}
//---------------------------------------------
void CMOWShader::GetPSBuffers( 
    std::vector<ID3D11Buffer*>& buffers 
    )
{
    buffers.push_back(m_matrixBuffer);
}
//---------------------------------------------
HRESULT CMOWShader::SetupBuffer( 
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
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferDefinition* matrixBufferDef = 0;
    if( buffer == m_matrixBuffer )
    {
        result = context->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        if(!FAILED(result))
        {
            // Get a pointer to the data in the constant buffer.
            matrixBufferDef = reinterpret_cast<MatrixBufferDefinition*>(mappedResource.pData);

            // Copy the matrices into the constant buffer.
            matrixBufferDef->world = worldMatrix;
            matrixBufferDef->view = viewMatrix;
            matrixBufferDef->projection = projectionMatrix;
            matrixBufferDef->lightProjectionMatrix = lightProjectionMatrix;
            matrixBufferDef->lightViewMatrix = lightViewMatrix;
            matrixBufferDef->screenWidth = screenWidth;
            matrixBufferDef->screenHeight = screenHeight;
            // Unlock the constant buffer.
            context->Unmap(m_matrixBuffer, 0);
        }
    }

    return result;

}
//---------------------------------------------
bool CMOWShader::UseResources() const
{
    return true;
}
//---------------------------------------------
const char* CMOWShader::VertexShaderDescription()
{
    return m_vertexShaderDescription.c_str();
}
//---------------------------------------------
const char* CMOWShader::PixelShaderDescription()
{
    return m_pixelShaderDescription.c_str();
}
//---------------------------------------------
const char* CMOWShader::VertexShaderFilename()
{
    return m_vertexShaderFilename.c_str();
}
//---------------------------------------------
const char* CMOWShader::PixelShaderFilename()
{
    return m_pixelShaderFilename.c_str();
}
//---------------------------------------------
std::vector<byte> CMOWShader::GetShaderBytes(
    const char* shaderFileName
    )
{


    std::vector<byte> buff;

    if( shaderFileName && *shaderFileName )
    {
        int count = 0;

        std::ifstream fin(shaderFileName, std::ios::binary);

        if( !fin.fail() )
        {
            fin.seekg(0, std::ios_base::end);
            UINT size = (int)fin.tellg();
            fin.seekg(0, std::ios_base::beg);

            buff.resize(size);

            fin.read(reinterpret_cast<char*>(&buff[0]), size);
            fin.close();
        }
    }

    return buff;
}
//---------------------------------------------
void CMOWShader::Clear(
    ID3D11DeviceContext* context
    )
{
    ID3D11ShaderResourceView* dummy = 0;
    
    for( int i=0; i<m_resourceCount; i++ )
    {
        context->PSSetShaderResources(i,1,&dummy);
    }
    
}
//---------------------------------------
D3D11_INPUT_ELEMENT_DESC CMOWShader::CreateVertexElementDesc(
    const char* name,
    DXGI_FORMAT format,
    UINT alignedByOffset
    )
{
    D3D11_INPUT_ELEMENT_DESC desc;
    
    desc.SemanticName = name;
    desc.SemanticIndex = 0;
    desc.Format = format;
    desc.InputSlot = 0;
    desc.AlignedByteOffset = alignedByOffset;
    desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    desc.InstanceDataStepRate = 0;

    return desc;
}
//---------------------------------------
bool CMOWShader::CreateFX(ID3D11Device* device)
{
    bool retVal = false;
    DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
    shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

    std::vector<byte> shaderBytes = GetShaderBytes(FxFilename());

    if( shaderBytes.size() )
    {
        if( !FAILED(D3DX11CreateEffectFromMemory(&shaderBytes[0],shaderBytes.size(), 0, device, &m_fx)) )
        {
            m_fxTech = m_fx->GetTechniqueByName(FxTechName());
            m_fxTechNoPixelShader = m_fx->GetTechniqueByName(FxTechNoPixelShaderName());
            if( m_fxTech || m_fxTechNoPixelShader )
            {
                m_fxVariables.world = m_fx->GetVariableByName("world")->AsMatrix();
                m_fxVariables.view = m_fx->GetVariableByName("view")->AsMatrix();
                m_fxVariables.projection = m_fx->GetVariableByName("projection")->AsMatrix();
                m_fxVariables.lightView = m_fx->GetVariableByName("lightView")->AsMatrix();
                m_fxVariables.lightProjection = m_fx->GetVariableByName("lightProjection")->AsMatrix();
                m_fxVariables.cameraPosition = m_fx->GetVariableByName("cameraPosition")->AsVector();
                m_fxVariables.screenWidth = m_fx->GetVariableByName("screenWidth")->AsScalar();
                m_fxVariables.screenHeight = m_fx->GetVariableByName("screenHeight")->AsScalar();
                m_fxVariables.material = m_fx->GetVariableByName("material");
                m_fxVariables.light = m_fx->GetVariableByName("light");
                retVal = true;
            }
        }
    }

    return retVal;

    
}
//---------------------------------------
const char* CMOWShader::FxFilename()
{
    return m_fxFileName.c_str();
}
//---------------------------------------
const char* CMOWShader::FxTechName()
{
    return m_fxTechName.c_str();
}
//---------------------------------------
void CMOWShader::ApplyFXResources(std::vector<ID3D11ShaderResourceView*>* resources)
{
    //Do nothing. Override to implement
}
//---------------------------------------
const char* CMOWShader::FxTechNoPixelShaderName()
{
    return m_fxTechNoPixelShaderName.c_str();
}

//---------------------------------------
CMOWShader::FxVariables::FxVariables()
{
    ZeroMemory(this,sizeof(FxVariables));
}
//---------------------------------------
CMOWShader::FxVariables::~FxVariables()
{
    
    if( world )
    {
        world->Release();
    }
    if( view )
    {
        view->Release();
    }
    if( projection )
    {
        projection->Release();
    }
    if( lightView )
    {
        lightView->Release();
    }
    if( lightProjection )
    {
        lightProjection->Release();
    }
    
}
