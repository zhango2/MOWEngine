#include "MOWAssimp.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "MOWGraphics/MOWModel.h"
#include "MOWGraphics/MOWModelPart.h"
#include "MOWGraphics/MOWFace.h"
#include "MOWGraphics/MOWCommon.h"
#include <d3dcommon.h>

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
                                        aiProcess_SortByPType
        );

    if( scene && scene->HasMeshes() )
    {
        model = new CMOWModel;
        for( unsigned int i=0; i<scene->mNumMeshes; i++ )
        {
            aiMesh* mesh = scene->mMeshes[i];
            CMOWModelPart* modelPart = model->CreateAndAddModelPart(mesh->mName.C_Str(),D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            if( mesh && mesh->HasFaces() )
            {
                for( unsigned int n=0; n<mesh->mNumFaces; n++ )
                {
                    aiFace* face = &mesh->mFaces[n];

                    if( face && 3 == face->mNumIndices )
                    {
                        unsigned int index1 = face->mIndices[0];
                        unsigned int index2 = face->mIndices[1];
                        unsigned int index3 = face->mIndices[2];
                        
                        CMOWFace* mowFace = CMOWFace::Create(index1,index2,index3);
                        modelPart->AddFace(mowFace);
                    }
                }

                aiVector3D* textCoord = nullptr;
                if( mesh->HasTextureCoords(0) )
                {
                    textCoord = *(&mesh->mTextureCoords[0]);
                }
                for( unsigned int n=0; n<mesh->mNumVertices; n++ )
                {
                    aiVector3D* vertex = &mesh->mVertices[n];
                    aiVector3D* normal = &mesh->mNormals[n];
                    

                    Vertex mowVertex;
                    mowVertex.m_position.x = vertex->x;
                    mowVertex.m_position.y = vertex->y;
                    mowVertex.m_position.z = vertex->z;

                    mowVertex.m_normal.x = normal->x;
                    mowVertex.m_normal.y = normal->y;
                    mowVertex.m_normal.z = normal->z;

                    if( textCoord )
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

    return model;
}
}


