#include "MOWAssimp.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/material.h"
#include "MOWGraphics/MOWModel.h"
#include "MOWGraphics/MOWModelPart.h"
#include "MOWGraphics/MOWFace.h"
#include "MOWGraphics/MOWCommon.h"
#include <d3dcommon.h>
#include <sstream>
#include "MOWGraphics/MOWMaterial.h"
#include "MOWGraphics/MOWResourceManager.h"
#include "MOWGraphics/MOWScene.h"
#include "MOWPhysics/MOWPhysics.h"

namespace MOWAssimp
{
//------------------------------------------------------
void LoadModelsIntoResourceManager(
    const char* fileName,
    const char* modelName,
    const std::string& texturePath,
    CMOWPhysics& physics,
    bool loadAllMeshesIntoASingleModel
    )
{
    Assimp::Importer importer;
    std::map< std::string, std::vector<std::string> > textureNamesByMaterialName;
    std::map< unsigned int, std::string > materialNameByIndex;

    const aiScene* aiScene = importer.ReadFile(
                                        fileName,
                                        aiProcess_CalcTangentSpace |
                                        aiProcess_Triangulate |
                                        aiProcess_JoinIdenticalVertices |
                                        aiProcess_SortByPType |
                                        aiProcess_ConvertToLeftHanded
        );


    if( aiScene )
    {
        LoadMaterialsAndTextures(
            aiScene, 
            fileName,
            texturePath,
            textureNamesByMaterialName,
            materialNameByIndex
        );
        
        if( aiScene->HasMeshes() )
        {
            CMOWModel* model = new CMOWModel;
            for(unsigned int i = 0; i < aiScene->mNumMeshes; i++)
            {
                aiMesh* mesh = aiScene->mMeshes[i];

                if( mesh )
                {
                    if( i != 0 )
                    {
                        model = loadAllMeshesIntoASingleModel ? model : new CMOWModel;
                    }
                    
                    LoadModel(mesh, modelName, physics, textureNamesByMaterialName, materialNameByIndex, model);

                    if( model )
                    {
                        if( !CMOWResourceManager::Instance()->GetModel(model->Name().c_str()) )
                        {
                            CMOWResourceManager::Instance()->AddModel(model);
                        }
                        else if( !loadAllMeshesIntoASingleModel )
                        {
                            delete model;
                        }
                    }
                }
            }
        }
    }
}
//------------------------------------------------------
void LoadMaterialsAndTextures(
    const aiScene* aiScene, 
    const char* fileName,
    const std::string& texturePath,
    std::map< std::string, std::vector<std::string> >& textureNamesByMaterialName, 
    std::map< unsigned int, std::string >& materialNameByIndex
    )
{
    if( aiScene->HasMaterials() )
    {
        for( unsigned int i=0; i<aiScene->mNumMaterials; i++ )
        {
            aiMaterial* mat = aiScene->mMaterials[i];

            if( mat )
            {
                aiString matName;
                mat->Get(AI_MATKEY_NAME,matName);

                materialNameByIndex[i] = matName.C_Str();

                std::stringstream ss;
                std::string objName = fileName;

                texturePath.length() !=0 ? ss << texturePath : ss << "";

                size_t index = objName.find_last_of('.');
                ss << objName.substr(0,index) << "_" << matName.C_Str();

                std::string textureName = ss.str() + "_BaseColor.png";
                textureNamesByMaterialName[matName.C_Str()].push_back(textureName);

                textureName = ss.str() + "_Normal.png";
                textureNamesByMaterialName[matName.C_Str()].push_back(textureName);

                textureName = ss.str() + "_MetRoughHeight.png";
                textureNamesByMaterialName[matName.C_Str()].push_back(textureName);
            }

        }
    }
}
//------------------------------------------------------
void LoadModel(
    const aiMesh* mesh, 
    const char* modelName,
    CMOWPhysics& physics,
    const std::map< std::string, std::vector<std::string> >& textureNamesByMaterialName, 
    const std::map< unsigned int, std::string >& materialNameByIndex,
    CMOWModel* target
    )
{
    target->Name(modelName);
    CMOWModelPart* modelPart = target->CreateAndAddModelPart(mesh->mName.C_Str(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    auto itMaterialName = materialNameByIndex.find(mesh->mMaterialIndex);

    if( itMaterialName != materialNameByIndex.end() )
    {
                        
        CMOWMaterial* material = CMOWResourceManager::Instance()->Material(itMaterialName->second.c_str()) ? CMOWResourceManager::Instance()->Material(itMaterialName->second.c_str()) : new CMOWMaterial();
        material->Name(itMaterialName->second.c_str());
        CMOWResourceManager::Instance()->AddMaterial(material);

        auto itTextureFileName = textureNamesByMaterialName.find(itMaterialName->second);

        if( itTextureFileName != textureNamesByMaterialName.end() )
        {
            for( int n=CMOWMaterial::TT_BASE_COLOR; n<CMOWMaterial::TT_LAST; n++ )
            {
                material->TextureFileName(itTextureFileName->second[n-1].c_str(), CMOWMaterial::TEXTURE_TYPE(n));
            }
        }
        modelPart->MaterialName(itMaterialName->second.c_str());
    }
                    
    if(mesh->HasFaces())
    {
        for(unsigned int n = 0; n < mesh->mNumFaces; n++)
        {
            aiFace* face = &mesh->mFaces[n];

            if(face && 3 == face->mNumIndices)
            {
                unsigned int index1 = face->mIndices[0];
                unsigned int index2 = face->mIndices[1];
                unsigned int index3 = face->mIndices[2];

                CMOWFace* mowFace = CMOWFace::Create(index1, index2, index3);
                modelPart->AddFace(mowFace);
            }
        }

        aiVector3D* textCoord = nullptr;
        if(mesh->HasTextureCoords(0))
        {
            textCoord = *(&mesh->mTextureCoords[0]);
        }
        for(unsigned int n = 0; n < mesh->mNumVertices; n++)
        {
            aiVector3D* vertex = &mesh->mVertices[n];
            aiVector3D* normal = &mesh->mNormals[n];
            aiVector3D* tangent = &mesh->mTangents[n];
            aiVector3D* biTangent = &mesh->mBitangents[n];


            Vertex mowVertex;
            mowVertex.m_position.x = vertex->x;
            mowVertex.m_position.y = vertex->y;
            mowVertex.m_position.z = vertex->z;

            mowVertex.m_normal.x = normal->x;
            mowVertex.m_normal.y = normal->y;
            mowVertex.m_normal.z = normal->z;

            mowVertex.m_tangent.x = tangent->x;
            mowVertex.m_tangent.y = tangent->y;
            mowVertex.m_tangent.z = tangent->z;

            mowVertex.m_biTangent.x = biTangent->x;
            mowVertex.m_biTangent.y = biTangent->y;
            mowVertex.m_biTangent.z = biTangent->z;

            if(textCoord)
            {
                aiVector3D* actualTextCoord = &textCoord[n];
                mowVertex.m_texture.x = actualTextCoord->x;
                mowVertex.m_texture.y = actualTextCoord->y;
            }

            modelPart->AddVertex(mowVertex);
        }
                        
    }
    //target->CreatePhysicalBody(physics, true, true);
}
}


