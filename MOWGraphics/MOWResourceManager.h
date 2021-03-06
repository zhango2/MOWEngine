#ifndef CDXResourceManager_H
#define CDXResourceManager_H

#include <string>
#include <map>
#include <vector>
#include "MOWCommon.h"

class CMOWShader;

DECLARE_SHARED_PTR(CMOWMaterial)
DECLARE_SHARED_PTR(CMOWModel)

typedef std::map<std::string,ID3D11ShaderResourceView*> textures_map;
typedef std::vector<CMOWMaterialPtr> material_vector;
typedef std::map<std::string,CMOWMaterialPtr> material_by_name_map;
typedef std::map<std::string,unsigned int> material_index_by_name_map;
typedef std::map<std::string,CMOWModelPtr> model_by_name_map;
typedef std::vector<CMOWModelPtr> model_vector;
typedef std::map<std::string,CMOWShader*> shader_by_name_map;

class CMOWResourceManager
{
public:

    ~CMOWResourceManager(
        );

    static CMOWResourceManager*  
        Instance(
            );

    ID3D11ShaderResourceView*   
        GetOrCreateTexture(
            ID3D11Device* device,
            const char* textureFilename
            );

    void                        
        AddMaterial(
            CMOWMaterialPtr mat
            );

    CMOWMaterialPtr                
        Material(
            const char* name
            );

    bool                       
        SerializeLoad(
            const char* fileName,
            ID3D11Device* device
            );

    bool                        
        SerializeSave(
            const char* fileName
            ) const;

    material_vector             
        Materials(
            );

    void                        
        AddModel(
            CMOWModelPtr model
            );

    CMOWModelPtr                   
        GetModel(
            const char* modelName
            );

    CMOWModelPtr                   
        GetNewModelInstance(
            const char* modelName
            );

    model_vector                
        Models(
            );

    void                        
        AddShader(
            CMOWShader* shader
            );

    CMOWShader*                  
        GetShader(
            const char* shaderDesc
            );

    ID3D11RasterizerState*      
        GetWireframeState(
            );

    void                        
        SetDevice(
            ID3D11Device* device
            );

    ID3D11Device*               
        GetDevice(
            );


private:

    CMOWResourceManager(
        );

    void                        
        ClearShaders(
            );

    void                        
        ClearMaterials(
            );

    void                        
        ClearTextures(
            );
    

    textures_map                
        m_textures;

    material_by_name_map        
        m_materialByName;

    model_by_name_map           
        m_models;

    shader_by_name_map          
        m_shaders;

    ID3D11RasterizerState*      
        m_wireframeState;

    ID3D11Device*               
        m_d3dDevice;
    
                                
};


#endif