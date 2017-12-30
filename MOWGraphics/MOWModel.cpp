#include "MOWModel.h"
#include "MOWObjParser\ObjParser.h"
#include <string>
#include <math.h>
#include "MOWFace.h"
#include <list>
#include <iostream>
#include <fstream>
#include "MOWMaterial.h"
#include "MOWResourceManager.h"
#include "MOWBoundingBox.h"
#include "MOWCommon/MOWSerializer.h"
#include "MOWCommon/MOWVector.h"
#include "MOWPhysics/IMOWPhysicalEntity.h"
#include "MOWPhysics/MOWPhysics.h"

using namespace DirectX;

//---------------------------------------------
CMOWModel::CMOWModel(
    )
{
    m_physicalBody = 0;
    m_boundingBox = 0;
    m_boundingShape = PbMOWGraphics::PMBS_NONE;
    
}
//---------------------------------------------
CMOWModel::CMOWModel( CMOWModel& orig )
{
    m_physicalBody = nullptr;
    m_boundingBox = nullptr;
    m_boundingShape = PbMOWGraphics::PMBS_NONE;
    m_textures = orig.m_textures;
    
    CopyParts(orig.m_parts);
}
//---------------------------------------------
CMOWModel::~CMOWModel()
{
    Clear();
}
//---------------------------------------------
bool CMOWModel::Initialize(ID3D11Device* device)
{
    bool retVal = false;
    HRESULT result = S_OK;

    D3D11_BUFFER_DESC vertexBufferDesc;
    D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
    D3D11_SUBRESOURCE_DATA indexData;

    memset(&vertexBufferDesc,0,sizeof(D3D11_BUFFER_DESC));
    memset(&indexBufferDesc,0,sizeof(D3D11_BUFFER_DESC));
    memset(&vertexData,0,sizeof(D3D11_SUBRESOURCE_DATA));
    memset(&indexData,0,sizeof(D3D11_SUBRESOURCE_DATA));

	PopulateVerticesAndIndices();

    parts_map::iterator itParts = m_parts.begin();

    while( itParts != m_parts.end() )
    {
        itParts->second->CreateResources(device);
        itParts++;
    }
    FaceResources();

    Update();

    CreateBoundingVolumes();

    if( m_boundingBox )
    {
        m_boundingBox->Initialize(device);
    }

    return retVal;
}
//---------------------------------------------
void CMOWModel::PopulateVerticesAndIndices()
{
    //Do nothing
}
//---------------------------------------------
bool CMOWModel::Render(
    ID3D11DeviceContext* context,
    CMOWShader* shader,
    const XMFLOAT4X4& viewMatrix,
    const XMFLOAT4X4& projectionMatrix,
    const XMFLOAT4X4& lightViewMatrix,
    const XMFLOAT4X4& lightProjectionMatrix,
    const XMFLOAT3& cameraPosition,
    CMOWShader::LightBufferDefinition& lightBuff,
    int screenWidth,
    int screenHeight,
    std::vector<ID3D11ShaderResourceView*>* resources,
    bool usePixelShader
    )const
{
    bool retVal = true;

    for( auto itParts : m_parts )
    {
        CMOWMaterial* mat = CMOWResourceManager::Instance()->Material(itParts.second->MaterialName());

        if( mat )
        {
            lightBuff.Ka = mat->Ka();
            lightBuff.Kd = mat->Kd();
            lightBuff.Ks = mat->Ks();
            lightBuff.Ke = mat->Ke();
            lightBuff.specularPower = mat->SpecularPower();
        }
        
        shader->Update(context,
                       WorldMatrix(),
                       viewMatrix,
                       projectionMatrix,
                       lightViewMatrix,
                       lightProjectionMatrix,
                       lightBuff,
                       cameraPosition,
                       screenWidth,
                       screenHeight,
                       resources ? resources : FaceResources(),
                       usePixelShader
                       );

        itParts.second->Render(context);
    }

    return retVal;
}
//---------------------------------------
bool CMOWModel::Render(
    ID3D11DeviceContext* context, 
    CMOWShader* shader, 
    const DirectX::XMFLOAT4X4& viewMatrix, 
    const DirectX::XMFLOAT4X4& projectionMatrix, 
    const DirectX::XMFLOAT4X4& lightViewMatrix, 
    const DirectX::XMFLOAT4X4& lightProjectionMatrix, 
    const DirectX::XMFLOAT3& cameraPosition, 
    ShaderLight& light, 
    int screenWidth, 
    int screenHeight, 
    std::vector<ID3D11ShaderResourceView*>* resources,
    bool usePixelShader,
    bool useAdjacentIndices
    )const
{
    bool retVal = true;
    for( auto itParts : m_parts )
    {
        CMOWMaterial* mat = CMOWResourceManager::Instance()->Material(itParts.second->MaterialName());
        ShaderMaterial shaderMat;

        if( mat )
        {
            shaderMat = mat->AsShaderMaterial();
        }

        shader->Update(context,
                       WorldMatrix(),
                       viewMatrix,
                       projectionMatrix,
                       lightViewMatrix,
                       lightProjectionMatrix,
                       light,
                       shaderMat,
                       cameraPosition,
                       screenWidth,
                       screenHeight,
                       resources ? resources : FaceResources(),
                       usePixelShader
                       );

        itParts.second->Render(context,useAdjacentIndices);
    }

    return retVal;
}

//---------------------------------------------
CMOWModel* CMOWModel::PopulateModelFromObjFile(
    const char* fileName
    )
{
    
    bool retVal = false;
    CMOWModel* model = 0;
    
    std::map<std::string,CObjParser::ObjMaterial> materials;
    std::vector<CObjParser::ObjObject> objects = CObjParser::Parse(fileName,materials);


    if(objects.size())
    {
        model = new CMOWModel;
        auto itMat = materials.begin();
        while( itMat != materials.end() )
        {
            
            CObjParser::ObjMaterial objMat = (*itMat).second;
            CMOWMaterial* mat = new CMOWMaterial(
                objMat.m_name.c_str(),
                objMat.m_aTexture.c_str(),
                XMFLOAT4(objMat.m_aR, objMat.m_aG, objMat.m_aB, 1.0f),
                XMFLOAT4(objMat.m_dR, objMat.m_dG, objMat.m_dB, 1.0f),
                XMFLOAT4(objMat.m_sR, objMat.m_sG, objMat.m_sB, 1.0f),
                XMFLOAT4(objMat.m_eR, objMat.m_eG, objMat.m_eB, 1.0f),
                objMat.m_specularPower > 0.0f ? 1000.0f / objMat.m_specularPower : 0.0f);

            CMOWResourceManager::Instance()->AddMaterial(mat);
            itMat++;
        }
        long vertIndexMax = 0;
        long textIndexMax = 0;
        long normalIndexMax = 0;
        for(const CObjParser::ObjObject& obj : objects)
        {
            long vertSubtract = vertIndexMax;
            long normalSubtract = normalIndexMax;
            long textSubtract = textIndexMax;

            CMOWModelPart* part = model->CreateAndAddModelPart(obj.m_name.c_str(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            part->MaterialName(obj.m_materialName.c_str());


            //Copy the information to the model

            std::vector<Vertex> resultVertex;
            
            for(const CObjParser::ObjFace& face : obj.m_faces)
            {
                for(size_t i = 0; i < face.m_vertices.size(); i+=3)
                {
                    std::vector<long> indexVector;
                    
                    for(int n = 2; n >= 0; n--)
                    {
                        long vertIndex = face.m_vertices[i + n] - 1;
                        long textureIndex = face.m_textures[i + n] - 1;
                        long normalIndex = face.m_normals[i + n] - 1;

                        vertIndexMax = vertIndex+1 > vertIndexMax ? vertIndex+1 : vertIndexMax;
                        normalIndexMax = normalIndex+1 > normalIndexMax ? normalIndex+1 : normalIndexMax;
                        textIndexMax = textureIndex+1 > textIndexMax ? textureIndex+1 : textIndexMax;

                        vertIndex -= vertSubtract;
                        normalIndex -= normalSubtract;
                        textureIndex -= textSubtract;

                        const CObjParser::ObjVertex& vertex = obj.m_vertices[vertIndex];
                        const CObjParser::ObjTexture& texture = obj.m_textureCoordinates[textureIndex];
                        const CObjParser::ObjNormal& normal = obj.m_normals[normalIndex];
                        Vertex vert;

                        vert.m_position = XMFLOAT3(vertex.m_x / 100.0f, vertex.m_z / 100.0f, vertex.m_y / 100.0f);
                        vert.m_normal = XMFLOAT3(normal.m_nX, normal.m_nZ, normal.m_nY);
                        vert.m_texture = XMFLOAT2(texture.m_u, 1.0f - texture.m_v);

                        resultVertex.push_back(vert);
                    }
                }
            }
            int index = 0;

            for(const CObjParser::ObjFace& face : obj.m_faces)
            {
                for(size_t n = 0; n < 3; n++)
                {
                    part->AddVertex(resultVertex[index + n]);
                }
                part->AddFace(CMOWFace::Create(index, index + 1, index + 2));
                index += 3;
            }


        }

    }

    return model;
}
//---------------------------------------------
void CMOWModel::Position( 
    float xPos, 
    float yPos, 
    float zPos 
    )
{
    if( PhysicalBody() )
    {
        PhysicalBody()->Position(CMOWVector(xPos,yPos,zPos));
    }
    else
    {
        m_position = XMFLOAT3(xPos,yPos,zPos);
    }
    
}
//---------------------------------------------
void CMOWModel::Rotation( 
    float xRot, 
    float yRot, 
    float zRot 
    )
{
    if( PhysicalBody() )
    {
        PhysicalBody()->Rotation(CMOWVector(xRot,yRot,zRot));
    }
    else
    {
        m_rotation = XMFLOAT4((float)XMConvertToRadians(xRot),(float)XMConvertToRadians(yRot),(float)XMConvertToRadians(zRot),1.0f);
    }
    if( m_boundingBox )
    {
        m_boundingBox->Rotation(m_rotation);
    }
    
}
//---------------------------------------------
void CMOWModel::Rotation( 
    const DirectX::XMFLOAT4& rotation 
    )
{
    m_rotation = rotation;
}
//---------------------------------------------
const XMFLOAT3& CMOWModel::Position() const
{

    if( PhysicalBody() )
    {
        CMOWVector pos = PhysicalBody()->Position();
        m_position = XMFLOAT3(pos.X(),pos.Y(),pos.Z());
    }
    if( m_boundingBox )
    {
        m_boundingBox->Position(m_position.x,m_position.y,m_position.z);
    }
    return m_position;
}
//---------------------------------------------
const XMFLOAT4& CMOWModel::Rotation() const
{
    
    if( PhysicalBody() )
    {
        CMOWVector rot = PhysicalBody()->Rotation();
        m_rotation = XMFLOAT4(rot.X(),rot.Y(),rot.Z(),rot.W());
        
    }
    if( m_boundingBox )
    {
        m_boundingBox->Rotation(m_rotation);
    }

    
    return m_rotation;
}
//---------------------------------------------
void CMOWModel::AngularVelocity( 
    float xVel, 
    float yVel, 
    float zVel 
    )
{
    if( PhysicalBody() )
    {
        PhysicalBody()->AngularVelocity(CMOWVector(xVel,yVel,zVel));
    }
    
}
//---------------------------------------------
CMOWModelPart* CMOWModel::CreateAndAddModelPart( 
    const char* name,
    D3D11_PRIMITIVE_TOPOLOGY topology
    )
{
    CMOWModelPart* part = new CMOWModelPart(topology);
    part->Name(name);
    AddModelPart(part);
    return part;
}
//---------------------------------------------
CMOWModelPart* CMOWModel::ModelPart( const char* name ) const
{
    CMOWModelPart* part = 0;
    auto itFind = m_parts.find(name);

    if( itFind != m_parts.end() )
    {
        part = itFind->second;
    }

    return part;
}
//---------------------------------------------
void CMOWModel::Clear()
{
    parts_map::iterator itParts = m_parts.begin();

    while( itParts != m_parts.end() )
    {
        CMOWModelPart* part = itParts->second;
        delete part;
        itParts = m_parts.erase(itParts);
    }
    if( m_physicalBody )
    {
        m_physicalBody->Clear();
    }
    
    if( m_boundingBox )
    {
        delete m_boundingBox;
    }
}
//---------------------------------------------
std::vector<ID3D11ShaderResourceView*>* CMOWModel::FaceResources() const
{
    if( !m_textures.size() )
    {
        for( auto itParts :  m_parts )
        {
            const std::vector<ID3D11ShaderResourceView*>& partTextures = itParts.second->GetFaceResources();

            for( ID3D11ShaderResourceView* texture : partTextures )
            {
                m_textures.push_back(texture);
            }
            
        }
    }

    return &m_textures;
    
}
//---------------------------------------------
D3D11_PRIMITIVE_TOPOLOGY CMOWModel::Topology() const
{
    return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}
//---------------------------------------------
bool CMOWModel::SerializeSave(
    const char* fileName,
    const CMOWPhysics& physics
    )const
{
    std::ofstream fOut;
    fOut.open(fileName,std::ios_base::binary);
    SerializeSave(fOut,physics);
    fOut.flush();
    fOut.close();

    return true;
}
//---------------------------------------------
void CMOWModel::SerializeSave( 
    std::ofstream& fOut,
    const CMOWPhysics& physics
    )const
{


    PbMOWGraphics::PbMOWModel model;
    ToPb(model,physics);

    model.SerializeToOstream(&fOut);
}
//---------------------------------------------
CMOWModel* CMOWModel::SerializeLoad( 
    std::ifstream& fIn,
    const CMOWPhysics& physics,
    bool useAdjacencies
    )
{

    PbMOWGraphics::PbMOWModel pbModel;

    pbModel.ParseFromIstream(&fIn);

    return CMOWModel::FromPb(pbModel,physics);

    //CDXObject::SerializeLoad(fIn);
    
    /*size_t partSize = CDXSerializer::ReadSizeT(fIn);

    for( size_t i=0; i<partSize; i++ )
    {
        std::string partName = CDXSerializer::ReadString(fIn);
        CDXModelPart* part = new CDXModelPart(Topology());

        part->Serialize(fIn);

        if( part->IndicesWithAdjacencies().size() == 0 && useAdjacencies )
        {
            //No adjacencies loaded. Create them
            FindAdjacencies(part);
        }
        //part->IndicesWithAdjacencies().clear();
        m_parts[partName] = part;
    }

    int bufSize = CDXSerializer::ReadSizeT(fIn);

    if( bufSize > 0 )
    {
        hkArray<char> buff(bufSize);
        
        CDXSerializer::ReadBlob(fIn,(void*)(&buff[0]),bufSize);
        
        m_physicalBody = hkSerializeUtil::loadObject<hkpRigidBody>(buff.begin(),buff.getSize());
        m_physicalBody->setLinearVelocity(hkVector4(0.0f,0.0f,0.0f,0.0f));
    }*/
}
//------------------------------------------------------
CMOWModel* CMOWModel::SerializeLoad(
    const char* fileName,
    const CMOWPhysics& physics
    )
{
    CMOWModel* model = 0;
    std::ifstream fIn;
    fIn.open(fileName,std::ios_base::binary);
    model = SerializeLoad(fIn,physics),
    fIn.close();

    return model;
}
//---------------------------------------------
IMOWPhysicalEntity* CMOWModel::PhysicalBody() const
{
    return m_physicalBody;
}
//---------------------------------------------
long CMOWModel::VertexCount() const
{
    long count = 0;
    
    for( auto itPart : m_parts )
    {
        count += itPart.second->Vertices().size();
    }

    return count;
}
//---------------------------------------------
long CMOWModel::PolygonCount() const
{
    long count = 0;
    
    for( auto itPart : m_parts )
    {
        count += itPart.second->Faces().size();
    }

    return count;
}
//---------------------------------------------
void CMOWModel::UpdateBoundingVolumes()
{

    XMVECTOR res;
    XMVECTOR temp = XMVectorSet(m_boundingSphere.m_xCenter, m_boundingSphere.m_yCenter, m_boundingSphere.m_zCenter,1.0f);

    res = XMVector3Transform(temp,XMLoadFloat4x4(&WorldMatrix()));

    m_boundingSphere.m_xWorldCenter = XMVectorGetX(res);
    m_boundingSphere.m_yWorldCenter = XMVectorGetY(res);
    m_boundingSphere.m_zWorldCenter = XMVectorGetZ(res);
}
//---------------------------------------------
void CMOWModel::Update()
{
    CMOWObject::Update();

    if( m_boundingBox )
    {
        m_boundingBox->Update();
    }
    
    UpdateBoundingVolumes();
    
}
//---------------------------------------------
CMOWBoundingBox* CMOWModel::BoundingBox() const
{
    return m_boundingBox;
}
//---------------------------------------------
const CMOWModel::ModelBoundingSphere* CMOWModel::BoundingSphere() const
{
    return &m_boundingSphere;
}
//---------------------------------------------
void CMOWModel::AngularDamping( float damping )
{
    if( PhysicalBody() )
    {
        PhysicalBody()->AngularDamping(damping);
    }
}
//---------------------------------------------
void CMOWModel::CopyParts( parts_map& parts )
{
    parts_map::iterator itPart = parts.begin();

    while( itPart != parts.end() )
    {
        CMOWModelPart* newPart = new CMOWModelPart(*itPart->second);
        m_parts[itPart->first] = newPart;

        itPart++;
    }
}
//---------------------------------------------
void CMOWModel::LinearVelocity( float xVel, float yVel, float zVel )
{
    if( PhysicalBody() )
    {
        PhysicalBody()->LinearVelocity(CMOWVector(xVel,yVel,zVel));
    }
}
//---------------------------------------------
void CMOWModel::Scale( 
    float xScale, 
    float yScale, 
    float zScale 
    )
{
    CMOWObject::Scale(xScale,yScale,zScale);
    if( m_boundingBox )
    {
        m_boundingBox->Scale(xScale,yScale,zScale);
    }
    m_boundingSphere.m_isInitialized = false;
    if( PhysicalBody() )
    {
        //TODO: How to scale the physics object?
        //Probably not doable in runtime
    }
}
//---------------------------------------------
ID3D11Buffer* CMOWModel::VertexBufferFromPart( const char* partName ) const
{
    ID3D11Buffer* retVal = 0;
    CMOWModelPart* part = ModelPart(partName);

    if( part )
    {
        retVal = part->GetVertexBuffer();
    }

    return retVal;
}
//---------------------------------------------
ID3D11Buffer* CMOWModel::IndexBufferFromPart( const char* partName ) const
{
    ID3D11Buffer* retVal = 0;
    CMOWModelPart* part = ModelPart(partName);

    if( part )
    {
        retVal = part->GetIndexBuffer();
    }

    return retVal;
}
//---------------------------------------------
ID3D11Buffer* CMOWModel::SharedVertexBufferFromPart( const char* partName )
{
    //Check if this model has already been created.
    //If so we can share its buffers

    ID3D11Buffer* retVal = 0;
    CMOWModel* model = CMOWResourceManager::Instance()->GetModel(Name().c_str());

    if( model )
    {
        retVal = model->VertexBufferFromPart(partName);
    }

    return retVal;
}
//---------------------------------------------
ID3D11Buffer* CMOWModel::SharedIndexBufferFromPart( const char* partName )
{
    //Check if this model has already been created.
    //If so we can share its buffers

    ID3D11Buffer* retVal = 0;
    CMOWModel* model = CMOWResourceManager::Instance()->GetModel(Name().c_str());

    if( model )
    {
        retVal = model->VertexBufferFromPart(partName);
    }

    return retVal;
}
//---------------------------------------------
PbMOWGraphics::PbMOWBoundingShape CMOWModel::BoundingShape() const
{
    return m_boundingShape;
}
//---------------------------------------------
static long GetOppositeIndex(CMOWFace& face, CMOWEdge& e)
{
    long index = 0;
    for (long i = 0 ; i < 3 ; i++) 
    {
        index = face.Indices()[i];
        if( index != e.m_index1 && index != e.m_index2 )
        {
            return index;
        }
    }
    assert(0);
    return index;
} 


//---------------------------------------------
void CMOWModel::FindAdjacencies(CMOWModelPart* part)
{
    std::map<XMFLOAT3,long, CMOWCompareXMFLOAT3> posMap;
    face_vector faces = part->Faces();
    std::vector<CMOWFace*> adjecentFaces;
    std::map<CMOWEdge,CMOWNeighbors,CMOWCompareEdges> neighborsByEdge;

    std::set<CMOWFace*> remainingFaces;

    for( size_t i = 0; i<faces.size(); i++ )
    {
        adjecentFaces.clear();
        remainingFaces.insert(faces[i]);
    }
    for(CMOWFace* face : faces )
    {
        
        FindAjacentFaces(face,remainingFaces,part,adjecentFaces);
        /*if( face->AdjacentCount() == 3 )
        {
            remainingFaces.erase(face);
        }*/
    }



    /*for( size_t i = 0; i<faces.size(); i++ )
    {
        CDXFace uniqueFace;
        CDXFace* face = faces[i];

        for( size_t j=0; j<face->Indices().size(); j++ )
        {
            long index = face->Indices()[j];
            Vertex vert = part->Vertices()[index];
            if( posMap.find(vert.m_position) == posMap.end() )
            {
                posMap[vert.m_position] = index;
            }
            else
            {
                index = posMap[vert.m_position];
            }
            uniqueFace.AddIndex(index);
        }
        uniqueFaces.push_back(uniqueFace);

        CDXEdge edge1(uniqueFace.Indices()[0],uniqueFace.Indices()[1]);
        CDXEdge edge2(uniqueFace.Indices()[1],uniqueFace.Indices()[2]);
        CDXEdge edge3(uniqueFace.Indices()[2],uniqueFace.Indices()[0]);

        neighborsByEdge[edge1].AddNeighbor(i);
        neighborsByEdge[edge2].AddNeighbor(i);
        neighborsByEdge[edge3].AddNeighbor(i);
    }

    for( size_t i = 0; i<faces.size(); i++ )
    {
        CDXFace* origFace = faces[i];
        CDXFace face = uniqueFaces[i];

        for( size_t j=0; j<3; j++ )
        {
            CDXEdge edge(face.Indices()[j],face.Indices()[(j+1) % 3]);
            CDXNeighbors neighbors = neighborsByEdge[edge];
            long otherTri = neighbors.GetOther(i);

            CDXFace otherFace = uniqueFaces[otherTri];
            long oppositeIndex = GetOppositeIndex(otherFace,edge);
            origFace->AddAdjecentIndex(oppositeIndex);
        }

        origFace->CreateAdjecentEdgesAndNormals(part->Vertices(),GetWorldMatrix());
    }*/

    
}
//------------------------------------------------------
void CMOWModel::FindAjacentFaces(
    CMOWFace* face,
    std::set<CMOWFace*>& remainingFaces,
    CMOWModelPart* part,
    std::vector<CMOWFace*>& adjacentFaces
    )
{
    const face_vector& faces = part->Faces();
    std::vector<long>& finalIndexBuff = part->MutableIndicesWithAdjacencies();
    std::vector<long> foundIndecies;
    int foundCount = 0;
    std::set<CMOWFace*>::iterator itFace = remainingFaces.begin();
            
    long indexToInsert = 0;
    
    for( size_t i=0; i<3; i++ )
    {
        Vertex origVert1;
        Vertex origVert2;

        if( i==2 )
        {
            origVert1 = part->Vertices()[face->Indices()[i]];
            origVert2 = part->Vertices()[face->Indices()[0]];
        }
        else
        {
            origVert1 = part->Vertices()[face->Indices()[i]];
            origVert2 = part->Vertices()[face->Indices()[i+1]];
        }

         
        itFace = remainingFaces.begin();
        for( size_t faceIndex = 0; faceIndex<faces.size(); faceIndex++ )
        {
            CMOWFace* faceToCheck = faces[faceIndex];
            if( faceToCheck != face )
            {
                
                for( size_t j=0; j<3; j++ )
                {
                    Vertex vert1;
                    Vertex vert2;

                    if( j == 2 )
                    {
                        vert1 = part->Vertices()[faceToCheck->Indices()[j]];
                        vert2 = part->Vertices()[faceToCheck->Indices()[0]];
                    }
                    else
                    {
                       vert1 = part->Vertices()[faceToCheck->Indices()[j]];
                       vert2 = part->Vertices()[faceToCheck->Indices()[j+1]];
                    }

                    if( (IsPositionEqual(origVert1,vert1) &&
                        IsPositionEqual(origVert2,vert2)) ||
                        (IsPositionEqual(origVert1,vert2) &&
                        IsPositionEqual(origVert2,vert1))
                        )
                    {        
                        indexToInsert = FindOppositeIndex(j == 2 ? 0 : j,j == 2 ? j : j+1,faceToCheck);
                            
                        finalIndexBuff.push_back(face->Indices()[foundCount]);
                        finalIndexBuff.push_back(indexToInsert);

                        faceToCheck->IncreaseAdjacentCount();
                        face->IncreaseAdjacentCount();
                        foundCount++;
                        break;
                        
                    }
                }
            }
            if( foundCount == 3 )
            {
                //we're done.No need to continue looking
                break;
            }
            itFace++;    
        }
    }
}
//---------------------------------------------
void CMOWModel::CreateBoundingBox(
    float xCenter,
    float yCenter,
    float zCenter,
    float xLength,
    float yLength,
    float zLength
    )
{
    if( !m_boundingBox )
    {
        m_boundingBox = CMOWBoundingBox::Create(xLength,yLength,zLength);
    }
}
//---------------------------------------
void CMOWModel::CreateBoundingVolumes()
{
    if( !m_boundingSphere.m_isInitialized )
    {
        float maxX = -999999.0f;
        float maxY = -999999.0f;
        float maxZ = -999999.0f;
        float minX = 999999.0f;
        float minY = 999999.0f;
        float minZ = 999999.0f;


        parts_map::iterator itPart = m_parts.begin();
        while( itPart != m_parts.end() )
        {
            for( size_t i = 0; i< itPart->second->Vertices().size(); i++ )
            {

                if( itPart->second->Vertices()[i].m_position.x * m_scale.x > maxX )
                {
                    maxX = itPart->second->Vertices()[i].m_position.x * m_scale.x;
                }
                else if (itPart->second->Vertices()[i].m_position.x * m_scale.x < minX)
                {
                    minX = itPart->second->Vertices()[i].m_position.x * m_scale.x;
                }
                if (itPart->second->Vertices()[i].m_position.y * m_scale.y > maxY)
                {
                    maxY = itPart->second->Vertices()[i].m_position.y * m_scale.y;
                }
                else if (itPart->second->Vertices()[i].m_position.y * m_scale.y < minY)
                {
                    minY = itPart->second->Vertices()[i].m_position.y * m_scale.y;
                }
                if (itPart->second->Vertices()[i].m_position.z * m_scale.z > maxZ)
                {
                    maxZ = itPart->second->Vertices()[i].m_position.z * m_scale.z;
                }
                else if (itPart->second->Vertices()[i].m_position.z * m_scale.z < minZ)
                {
                    minZ = itPart->second->Vertices()[i].m_position.z * m_scale.z;
                }

            }
            itPart++;
        }
        
        float xLength = fabs(maxX - minX);
        float yLength = fabs(maxY - minY);
        float zLength = fabs(maxZ - minZ);
        float xCenter = (maxX + minX) / 2.0f;
        float yCenter = (maxY + minY) / 2.0f;
        float zCenter = (maxZ + minZ) / 2.0f;

        XMFLOAT3 pos = Position();

        XMVECTOR res;
        XMVECTOR temp = XMVectorSet(xCenter,yCenter,zCenter,1.0f);
        res = XMVector3Transform(temp,XMLoadFloat4x4(&WorldMatrix()));

        CreateBoundingBox(pos.x,pos.y,pos.z,xLength,yLength,zLength);

        float boxVolume = xLength*yLength*zLength;
        float diagonal = sqrt( (xLength*xLength) + (yLength*yLength) + (zLength*zLength) );

        /*float maxLength = xLength;

        if( maxLength < yLength )
        {
            maxLength = yLength;
        }
        if( maxLength < zLength )
        {
            maxLength = zLength;
        }*/

        float radius = diagonal/2.0f;
        float sphereVolume = (4.0f*XM_PI*(radius*radius*radius) ) / 3.0f;
        

        m_boundingShape = sphereVolume < boxVolume ? PbMOWGraphics::PbMOWBoundingShape::PMBS_SPHERE : PbMOWGraphics::PbMOWBoundingShape::PMBS_BOX;
        
        m_boundingSphere.Initialize(xCenter,
                                    yCenter,
                                    zCenter,
                                    radius
                                    );

    }
}
//------------------------------------------------------
void CMOWModel::ToPb(
    PbMOWGraphics::PbMOWModel& toPb,
    const CMOWPhysics& physics
    ) const
{
    toPb.set_name(Name());

    for( auto itPart : m_parts )
    {
        PbMOWGraphics::PbMOWModelPart* part = toPb.add_parts();
        itPart.second->ToPb(*part);
    }

    toPb.set_boundingshape(m_boundingShape);

    if( BoundingBox() )
    {
        BoundingBox()->ToPb(*(toPb.mutable_boundingbox()));
    }
    
    if( m_boundingShape != PbMOWGraphics::PMBS_NONE )
    {
        toPb.mutable_boundingsphere()->mutable_center()->set_x(m_boundingSphere.m_xCenter);
        toPb.mutable_boundingsphere()->mutable_center()->set_y(m_boundingSphere.m_yCenter);
        toPb.mutable_boundingsphere()->mutable_center()->set_z(m_boundingSphere.m_zCenter);
        toPb.mutable_boundingsphere()->set_radius(m_boundingSphere.m_radius);
    }
    if( PhysicalBody() )
    {
        std::string physicalBody = physics.SaveEntity(PhysicalBody());
        toPb.set_physicalentity(physicalBody);
    }
    toPb.mutable_scale()->set_x(CMOWObject::Scale().x);
    toPb.mutable_scale()->set_y(CMOWObject::Scale().y);
    toPb.mutable_scale()->set_z(CMOWObject::Scale().z);

    
}
//------------------------------------------------------
CMOWModel* CMOWModel::FromPb(
    const PbMOWGraphics::PbMOWModel& fromPb,
    const CMOWPhysics& physics
    )
{
    CMOWModel* model = new CMOWModel;
    model->PropertiesFromPb(fromPb,physics);
    return model;
}
//------------------------------------------------------
void CMOWModel::PropertiesFromPb(
    const PbMOWGraphics::PbMOWModel& fromPb, 
    const CMOWPhysics& physics
    )
{
    Name(fromPb.name().c_str());
    for(const PbMOWGraphics::PbMOWModelPart& part : fromPb.parts())
    {
        CMOWModelPart* modelPart = CMOWModelPart::FromPb(part);
        AddModelPart(modelPart);
    }
    m_boundingShape = fromPb.boundingshape();

    if(fromPb.has_boundingbox())
    {
        m_boundingBox = CMOWBoundingBox::FromPb(fromPb.boundingbox());
    }

    if(fromPb.has_boundingsphere())
    {
        m_boundingSphere.m_isInitialized = false;
        m_boundingSphere.m_radius = fromPb.boundingsphere().radius();
        m_boundingSphere.m_xCenter = fromPb.boundingsphere().center().x();
        m_boundingSphere.m_yCenter = fromPb.boundingsphere().center().y();
        m_boundingSphere.m_zCenter = fromPb.boundingsphere().center().z();
    }
    if(fromPb.has_physicalentity())
    {
        m_physicalBody = physics.LoadEntity(fromPb.physicalentity());
    }
    if( fromPb.has_scale() )
    {
        Scale(fromPb.scale().x(),fromPb.scale().y(),fromPb.scale().z());
    }
}
//------------------------------------------------------
const parts_map& CMOWModel::Parts() const
{
    return m_parts;
}
//------------------------------------------------------
void CMOWModel::AddModelPart(CMOWModelPart* part)
{
    m_parts[part->Name()] = part;
}
//------------------------------------------------------
void CMOWModel::CreatePhysicalBody(
    CMOWPhysics& physics,
    bool fixed,
    bool collidable
    )
{
    m_physicalBody = physics.CreatePhysicalEntityFromModel(this,fixed,collidable);
}
//------------------------------------------------------
long CMOWModel::FindOppositeIndex(
    long index1, 
    long index2, 
    CMOWFace* face
    ) const
{
    long oppIndex = -1;

    if( index1 == 0 && index2 == 1 ||
        index1 == 1 && index2 == 0 )
    {
        oppIndex = 2;
    }
    else if( index1 == 0 && index2 == 2 ||
             index1 == 2 && index2 == 0 )
    {
        oppIndex = 1;
    }
    else if( index1 == 1 && index2 == 2 ||
             index1 == 2 && index2 == 1 )
    {
        oppIndex = 0;
    }

    return face->Indices()[oppIndex];

}
//---------------------------------------------




