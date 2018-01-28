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

namespace MOWAssimp
{
//------------------------------------------------------
CMOWModel* Load(
    const char* fileName
    )
{
    CMOWModel* model = nullptr;
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
                                        fileName,
                                        aiProcess_CalcTangentSpace |
                                        aiProcess_Triangulate |
                                        aiProcess_JoinIdenticalVertices |
                                        aiProcess_SortByPType |
                                        aiProcess_ConvertToLeftHanded
        );


    if( scene )
    {
        if( scene->HasMeshes() )
        {
            model = new CMOWModel;
            for(unsigned int i = 0; i < scene->mNumMeshes; i++)
            {
                aiMesh* mesh = scene->mMeshes[i];

                if( mesh )
                {
                    model->Name(mesh->mName.C_Str());
                    CMOWModelPart* modelPart = model->CreateAndAddModelPart(mesh->mName.C_Str(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

                    //Look for a material file. Blender won't export materials
                    CMOWMaterial* material = nullptr;
                    for( int i=CMOWMaterial::TT_BASE_COLOR; i<CMOWMaterial::TT_LAST; i++ )
                    {
                        AppendTextureFileNameToMaterial(CMOWMaterial::TEXTURE_TYPE(i),fileName, material);
                        CMOWResourceManager::Instance()->AddMaterial(material);
                    }

                    /*if(!fin.fail())
                    {
                        ss << fin.rdbuf();
                        std::string currentLine;
                        while(!ss.eof())
                        {
                            char buff[1024];
                            ss.getline(buff, 1024);
                            currentLine = buff;

                            if( currentLine.length() )
                            {
                                size_t index = currentLine.find_first_of(":");
                                std::string textureName = currentLine.substr(0, index);
                                std::string textureFileName = currentLine.substr(index + 1);

                                CMOWMaterial* material = CMOWResourceManager::Instance()->Material(mesh->mName.C_Str()) ? CMOWResourceManager::Instance()->Material(mesh->mName.C_Str()) : new CMOWMaterial();
                                CMOWMaterial::TEXTURE_TYPE textureType = CMOWMaterial::TextureTypeFromName(textureName.c_str());
                                material->TextureFileName(textureFileName.c_str(), textureType);
                                material->Name(mesh->mName.C_Str());
                                CMOWResourceManager::Instance()->AddMaterial(material);
                            }
                        }
                        fin.close();
                    }*/
                    
                    if( material )
                    {

                        //Fake some settings

                        material->Ka({1.0f,1.0f,1.0f,1.0f});
                        material->Kd({1.0f, 1.0f, 1.0f, 1.0f});
                        material->Ks({0.2f, 0.2f, 0.2f, 1.0f});
                        material->Ke({0.0f, 0.0f, 0.0f, 1.0f});
                        material->SpecularPower(10.f);

                        modelPart->MaterialName(material->Name());
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
                }
            }
        }
        CMOWResourceManager::Instance()->AddModel(model);
    }

    

    return model;
}
//------------------------------------------------------
void AppendTextureFileNameToMaterial(
    CMOWMaterial::TEXTURE_TYPE textureType, 
    const std::string& objectName,
    CMOWMaterial*& material
    )
{
    
    std::string objName = objectName;
    std::string textureTypeName = "BaseColor";
    std::stringstream ss;
    std::string materialFileName;

    textureTypeName = CMOWMaterial::TextureTypeNameFromType(textureType);

    size_t index = objName.find_last_of('.');
    objName = objName.substr(0,index);

    ss << objName << "_" << "Material_" << textureTypeName;
    ss << ".png";
    materialFileName = ss.str();

    if( !material )
    {
        material = CMOWResourceManager::Instance()->Material(objectName.c_str()) ? CMOWResourceManager::Instance()->Material(objectName.c_str()) : new CMOWMaterial();
    }
    material->TextureFileName(materialFileName.c_str(), textureType);
    material->Name(objectName.c_str());

}
}


