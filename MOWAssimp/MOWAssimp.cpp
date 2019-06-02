#include "MOWAssimp.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/material.h"

#include <d3dcommon.h>
#include <sstream>

#include "MOWGraphics/MOWModel.h"
#include "MOWGraphics/MOWModelPart.h"
#include "MOWGraphics/MOWFace.h"
#include "MOWGraphics/MOWCommon.h"
#include "MOWGraphics/MOWMaterial.h"
#include "MOWGraphics/MOWResourceManager.h"
#include "MOWGraphics/MOWScene.h"
#include "MOWGraphics/MOWBone.h"
#include "MOWPhysics/MOWPhysics.h"


using namespace DirectX;

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
            CMOWModelPtr model = CMOWModelPtr(new CMOWModel);
            for(unsigned int i = 0; i < aiScene->mNumMeshes; i++)
            {
                aiMesh* mesh = aiScene->mMeshes[i];

                if( mesh )
                {
                    if( i != 0 )
                    {
                        model = loadAllMeshesIntoASingleModel ? model : CMOWModelPtr(new CMOWModel);
                    }
                    
                    LoadModel(mesh, modelName, physics, textureNamesByMaterialName, materialNameByIndex, model);

                    if( model )
                    {
                        if( !CMOWResourceManager::Instance()->GetModel(model->Name().c_str()) )
                        {
                            CMOWResourceManager::Instance()->AddModel(model);
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
    CMOWModelPtr target
    )
{
    target->Name(modelName);
    CMOWModelPartPtr modelPart = target->CreateAndAddModelPart(mesh->mName.C_Str(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    CreateAndAddMaterials(*mesh,textureNamesByMaterialName,materialNameByIndex,*modelPart);
    CreateAndAddFaces(*mesh, *modelPart);
    CreateAndAddBones(*mesh, *modelPart);

    //target->CreatePhysicalBody(physics, true, true);
}
//------------------------------------------------------
void CreateAndAddMaterials(
    const aiMesh& mesh, 
    const std::map< std::string, std::vector<std::string> >& textureNamesByMaterialName, 
    const std::map< unsigned int, std::string >& materialNameByIndex, 
    CMOWModelPart& modelPart
    )
{
    auto itMaterialName = materialNameByIndex.find(mesh.mMaterialIndex);

    if( itMaterialName != materialNameByIndex.end() )
    {
                        
        CMOWMaterialPtr material = CMOWResourceManager::Instance()->Material(itMaterialName->second.c_str()) ? CMOWResourceManager::Instance()->Material(itMaterialName->second.c_str()) : CMOWMaterialPtr(new CMOWMaterial());
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
        modelPart.MaterialName(itMaterialName->second.c_str());
    }
}


//------------------------------------------------------
void CreateAndAddFaces(
    const aiMesh& mesh, 
    CMOWModelPart& modelPart
    )
{
    if(mesh.HasFaces())
    {
        for(unsigned int n = 0; n < mesh.mNumFaces; n++)
        {
            aiFace* face = &mesh.mFaces[n];

            if(face && 3 == face->mNumIndices)
            {
                unsigned int index1 = face->mIndices[0];
                unsigned int index2 = face->mIndices[1];
                unsigned int index3 = face->mIndices[2];

                CMOWFacePtr mowFace = CMOWFace::Create(index1, index2, index3);
                modelPart.AddFace(mowFace);
            }
        }

        aiVector3D* textCoord = nullptr;
        if(mesh.HasTextureCoords(0))
        {
            textCoord = *(&mesh.mTextureCoords[0]);
        }
        for(unsigned int n = 0; n < mesh.mNumVertices; n++)
        {
            aiVector3D* vertex = &mesh.mVertices[n];
            aiVector3D* normal = &mesh.mNormals[n];
            aiVector3D* tangent = &mesh.mTangents[n];
            aiVector3D* biTangent = &mesh.mBitangents[n];


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

            modelPart.AddVertex(mowVertex);
        }
                        
    }
}
//------------------------------------------------------
void CreateAndAddBones(
    const aiMesh& mesh, 
    CMOWModelPart& modelPart
    )
{
    if( mesh.HasBones() )
    {
        for(unsigned int i=0;i<mesh.mNumBones; i++ )
        {
            aiBone* bone = mesh.mBones[i];
            
            XMFLOAT4X4 offsetMatrix(
                bone->mOffsetMatrix.a1,
                bone->mOffsetMatrix.a2,
                bone->mOffsetMatrix.a3,
                bone->mOffsetMatrix.a4,
                bone->mOffsetMatrix.b1,
                bone->mOffsetMatrix.b2,
                bone->mOffsetMatrix.b3,
                bone->mOffsetMatrix.b4,
                bone->mOffsetMatrix.c1,
                bone->mOffsetMatrix.c2,
                bone->mOffsetMatrix.c3,
                bone->mOffsetMatrix.c4,
                bone->mOffsetMatrix.d1,
                bone->mOffsetMatrix.d2,
                bone->mOffsetMatrix.d3,
                bone->mOffsetMatrix.d4
                )
            ;

            CMOWBonePtr mowBone = CMOWBonePtr(new CMOWBone(bone->mName.C_Str(), offsetMatrix));


            for( unsigned int j=0; j<bone->mNumWeights; j++ )
            {
                aiVertexWeight weight = bone->mWeights[j];
                mowBone->AddVertexWeight(weight.mVertexId, weight.mWeight);
            }
            modelPart.AddBone(mowBone);

        }
    }
}
}


