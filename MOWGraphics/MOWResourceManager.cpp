#include "MOWResourceManager.h"
#include "MOWMaterial.h"
#include <fstream>
#include "MOWModel.h"
#include "MOWShaders\MOWShader.h"
#include "DirectXTK\Inc\WICTextureLoader.h"
#include "MOWCommon/MOWStringConverter.h"

static CMOWResourceManager* s_resourceManager = 0;

//---------------------------------------------
CMOWResourceManager::CMOWResourceManager()
{
    m_d3dDevice = 0;
    m_wireframeState = 0;
}
//---------------------------------------------
CMOWResourceManager::~CMOWResourceManager()
{
    ClearMaterials();
    ClearShaders();
    ClearTextures();
    if( m_d3dDevice )
    {
        m_d3dDevice->Release();
    }
    if( m_wireframeState )
    {
        m_wireframeState->Release();
    }
    
}
//---------------------------------------------
CMOWResourceManager* CMOWResourceManager::Instance()
{
    if( !s_resourceManager )
    {
        s_resourceManager = new CMOWResourceManager;
    }
    return s_resourceManager;
}
//---------------------------------------------
ID3D11ShaderResourceView* CMOWResourceManager::GetOrCreateTexture( 
    ID3D11Device* device, 
    const char* textureFilename 
    )
{
    ID3D11ShaderResourceView* texture = 0;

    textures_map::iterator itTexture = m_textures.find(textureFilename);
    std::wstring fileName = CMOWStringConverter::CharToWchar(textureFilename);
    if( itTexture == m_textures.end() )
    {
        if( !FAILED( DirectX::CreateWICTextureFromFileEx(device,fileName.c_str(),0,D3D11_USAGE_DEFAULT,D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,0,D3D11_RESOURCE_MISC_GENERATE_MIPS,false,NULL,&texture) ) )
        {
            m_textures[textureFilename] = texture;
        }
    }
    else
    {
        texture = itTexture->second;
    }

    return texture;
}
//---------------------------------------------
void CMOWResourceManager::AddMaterial( 
    CMOWMaterialPtr mat 
    )
{
    unsigned int index = 0;
    material_by_name_map::iterator itMat = m_materialByName.find(mat->Name());
    if( itMat == m_materialByName.end() )
    {
        m_materialByName[mat->Name()] = mat;
    }
}
//---------------------------------------------
CMOWMaterialPtr CMOWResourceManager::Material(
    const char* name
    )
{
    CMOWMaterialPtr mat = 0;
    material_by_name_map::iterator itMat = m_materialByName.find(name);
    if( itMat != m_materialByName.end() )
    {
        mat = itMat->second;
    }

    return mat;
}
//---------------------------------------------
bool CMOWResourceManager::SerializeLoad( 
    const char* fileName, 
    ID3D11Device* device 
    )
{
    bool retVal = true;

    /*std::ifstream fIn;
    fIn.open(fileName,std::ios_base::binary);

    size_t matSize;
    fIn.read(reinterpret_cast<char*>(&matSize),sizeof(size_t));

    if( fIn.good() )
    {
        for( size_t i=0; i<matSize; i++ )
        {
            CMOWMaterialPtr mat = new CMOWMaterial;
            mat->Serialize(fIn);
            AddMaterial(mat);
            GetOrCreateTexture(device,mat->TextureFileName());
        }

 
        size_t modelSize;
        fIn.read(reinterpret_cast<char*>(&modelSize),sizeof(size_t));

        if( fIn.good() )
        {
            for( size_t i=0; i<modelSize; i++ )
            {
                CMOWModelPtr model = new CMOWModel;
                model->SerializeLoad(fIn);
                AddModel(model);
            }
        }
    }

    fIn.close();*/
    
    return retVal;
}
//---------------------------------------------
bool CMOWResourceManager::SerializeSave( 
    const char* fileName 
    ) const
{
    /*std::ofstream fOut;
    fOut.open(fileName,std::ios_base::binary);

    size_t matSize = m_materialByName.size();
    fOut.write(reinterpret_cast<const char*>(&matSize),sizeof(size_t));

    if( fOut.good() )
    {
        material_by_name_map::iterator itMat = m_materialByName.begin();

        while( itMat != m_materialByName.end() )
        {
            itMat->second->Serialize(fOut);
            itMat++;
        }

        size_t modelSize = m_models.size();
        fOut.write(reinterpret_cast<const char*>(&modelSize),sizeof(size_t));

        if( fOut.good() )
        {
            model_by_name_map::iterator itModel = m_models.begin();

            while( itModel != m_models.end() )
            {
                itModel->second->SerializeSave(fOut);
                itModel++;
            }
        }
        
    }
    
    fOut.flush();
    fOut.close();*/

    return true;
}
//---------------------------------------------
material_vector CMOWResourceManager::Materials()
{
    material_vector materials;

    material_by_name_map::iterator itMaterial = m_materialByName.begin();

    while( itMaterial != m_materialByName.end() )
    {
        materials.push_back(itMaterial->second);
        itMaterial++;
    }

    return materials;
}
//---------------------------------------------
void CMOWResourceManager::AddModel( 
    CMOWModelPtr model 
    )
{
    m_models[model->Name()] = model;
}
//---------------------------------------------
CMOWModelPtr CMOWResourceManager::GetModel( 
    const char* modelName 
    )
{
    CMOWModelPtr model = 0;
    model_by_name_map::iterator itModel = m_models.find(modelName);

    if( itModel != m_models.end() )
    {
        model = itModel->second;
    }

    return model;

}
//---------------------------------------------
CMOWModelPtr CMOWResourceManager::GetNewModelInstance( 
    const char* modelName 
    )
{
    CMOWModelPtr newModel = 0;
    CMOWModelPtr orig = GetModel(modelName);

    if( orig )
    {
        newModel = CMOWModelPtr(new CMOWModel(*orig));
    }
    return newModel;
}
//---------------------------------------------
model_vector CMOWResourceManager::Models()
{
    model_vector models;
    model_by_name_map::iterator itModel = m_models.begin();

    while( itModel != m_models.end() )
    {

        models.push_back(itModel->second);
        itModel++;
    }

    return models;
}
//---------------------------------------
void CMOWResourceManager::AddShader(
    CMOWShader* shader
    )
{
    if( m_shaders.find(shader->PixelShaderDescription()) == m_shaders.end() )
    {
        m_shaders[shader->PixelShaderDescription()] = shader;
    }
    
}
//---------------------------------------
CMOWShader* CMOWResourceManager::GetShader(
    const char* shaderDesc
    )
{
    CMOWShader* shader = 0;
    auto itShader = m_shaders.find(shaderDesc);

    if( itShader != m_shaders.end() )
    {
        shader = itShader->second;
    }

    return shader;

}
//---------------------------------------
void CMOWResourceManager::ClearShaders()
{
    auto itShader = m_shaders.begin();

    while( itShader != m_shaders.end() )
    {
        CMOWShader* shader = itShader->second;
        delete shader;
        itShader = m_shaders.erase(itShader);
    }
}
//---------------------------------------
void CMOWResourceManager::ClearMaterials()
{
    m_materialByName.clear();
}
//---------------------------------------
void CMOWResourceManager::ClearTextures()
{
    auto itTexture = m_textures.begin();

    while( itTexture != m_textures.end() )
    {
        ID3D11ShaderResourceView* texture = itTexture->second;
        texture->Release();
        itTexture = m_textures.erase(itTexture);
    }
}
//---------------------------------------
ID3D11RasterizerState* CMOWResourceManager::GetWireframeState()
{
    if( !m_wireframeState && m_d3dDevice )
    {
        // Setup the raster description which will determine how and what polygons will be drawn.
        D3D11_RASTERIZER_DESC rasterDesc;
        rasterDesc.AntialiasedLineEnable = false;
        rasterDesc.CullMode = D3D11_CULL_NONE;
        rasterDesc.DepthBias = 0;
        rasterDesc.DepthBiasClamp = 0.0f;
        rasterDesc.DepthClipEnable = true;
        rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
        rasterDesc.FrontCounterClockwise = false;
        rasterDesc.MultisampleEnable = false;
        rasterDesc.ScissorEnable = false;
        rasterDesc.SlopeScaledDepthBias = 0.0f;

        // Create the rasterizer state from the description we just filled out.

        m_d3dDevice->CreateRasterizerState(&rasterDesc, &m_wireframeState);
        
    }
    return m_wireframeState;
}
//---------------------------------------
void CMOWResourceManager::SetDevice(
    ID3D11Device* device
    )
{
    if( m_d3dDevice != device )
    {
        if( m_d3dDevice )
        {
            m_d3dDevice->Release();
        }
        m_d3dDevice = device;
        m_d3dDevice->AddRef();
    }
    
}
//---------------------------------------
ID3D11Device* CMOWResourceManager::GetDevice()
{
    return m_d3dDevice;
}

//---------------------------------------


