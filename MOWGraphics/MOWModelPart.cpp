#include "MOWModelPart.h"
#include "MOWFace.h"
#include "MOWResourceManager.h"
#include <set>
#include "MOWMaterial.h"
#include "MOWCommon/MOWSerializer.h"
#include "MOWMessages/MOWCommon.pb.h"
#include "MOWMessages/MOWGraphics.pb.h"



//---------------------------------------------
CMOWModelPart::CMOWModelPart(D3D11_PRIMITIVE_TOPOLOGY topology)
{
    m_topology = topology;
    m_vertexBuffer = 0;
    m_indexBuffer = 0;
    m_adjacentIndexBuffer = 0;
}
//---------------------------------------------
CMOWModelPart::CMOWModelPart( CMOWModelPart& orig )
{
    m_vertexBuffer = orig.m_vertexBuffer;
    m_indexBuffer = orig.m_indexBuffer;
    m_vertices = orig.m_vertices;
    m_indices = orig.m_indices;
    m_topology = orig.m_topology;
    m_resources = orig.m_resources;
    m_materialName = orig.m_materialName;
    m_adjacentIndexBuffer  = orig.m_adjacentIndexBuffer;
    m_faces = orig.m_faces;
}
//---------------------------------------------
CMOWModelPart::~CMOWModelPart()
{
    Clear();
}
//---------------------------------------------
const vertex_vector& CMOWModelPart::Vertices() const
{
    return m_vertices;
}
//---------------------------------------------
const index_vector& CMOWModelPart::Indices() const
{
    return m_indices;
}
//---------------------------------------------
bool CMOWModelPart::CreateResources( ID3D11Device* device )
{

    
    bool retVal = true;
    D3D11_BUFFER_DESC vertexBufferDesc;
    D3D11_BUFFER_DESC indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData;
    D3D11_SUBRESOURCE_DATA indexData;

    memset(&vertexBufferDesc,0,sizeof(D3D11_BUFFER_DESC));
    memset(&indexBufferDesc,0,sizeof(D3D11_BUFFER_DESC));
    memset(&vertexData,0,sizeof(D3D11_SUBRESOURCE_DATA));
    memset(&indexData,0,sizeof(D3D11_SUBRESOURCE_DATA));

    if( Vertices().size() && Indices().size() )
    {
        std::set<ID3D11ShaderResourceView*> resources;
        for( size_t i = 0; i<Faces().size(); i++ )
        {
            CMOWFace* face = Faces()[i];
            CMOWMaterial* material = CMOWResourceManager::Instance()->Material(MaterialName());

            if( material )
            {
                ID3D11ShaderResourceView* texture = CMOWResourceManager::Instance()->GetOrCreateTexture(device,material->TextureFileName());
                std::set<ID3D11ShaderResourceView*>::iterator itResource = resources.find(texture);
                if( itResource == resources.end() )
                {
                    m_resources.push_back(texture);
                    resources.insert(texture);
                }
            }
            
        }

        // Set up the description of the static vertex buffer.
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = sizeof(Vertex) * Vertices().size();
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;
        vertexBufferDesc.StructureByteStride = 0;

        // Give the subresource structure a pointer to the vertex data.
        vertexData.pSysMem = &Vertices()[0];
        vertexData.SysMemPitch = 0;
        vertexData.SysMemSlicePitch = 0;

        // Now create the vertex buffer.
        if( !FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)) )
        {
            // Set up the description of the static index buffer.
            indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
            indexBufferDesc.ByteWidth = sizeof(long) * Indices().size();
            indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            indexBufferDesc.CPUAccessFlags = 0;
            indexBufferDesc.MiscFlags = 0;
            indexBufferDesc.StructureByteStride = 0;

            // Give the subresource structure a pointer to the index data.
            indexData.pSysMem = &Indices()[0];
            
            // Create the index buffer.
            retVal = !FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer));

            if( IndicesWithAdjacencies().size() )
            {
                indexBufferDesc.ByteWidth = sizeof(long) * IndicesWithAdjacencies().size();
                indexData.pSysMem = &IndicesWithAdjacencies()[0];
                retVal = !FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_adjacentIndexBuffer));
            }

        }
    }

    return retVal;
}
//---------------------------------------------
void CMOWModelPart::Clear()
{
    if( m_indexBuffer )
    {
        m_indexBuffer->Release();
    }
    if( m_vertexBuffer )
    {
        m_vertexBuffer->Release();
    }
    
    m_vertices.clear();
    m_indices.clear();

    face_vector::iterator itFace = m_faces.begin();

    while( itFace != m_faces.end() )
    {
        CMOWFace* face = *itFace;
        delete face;
        itFace = m_faces.erase(itFace);
    }
}
//---------------------------------------------
bool CMOWModelPart::Render( 
    ID3D11DeviceContext* context,
    bool useAdjacentIndices
    )
{
    bool retVal = true;
    unsigned int stride = sizeof(Vertex);
    unsigned int offset = 0;

    // Set the vertex buffer to active in the input assembler so it can be rendered.
    context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
    
    context->IASetIndexBuffer(m_adjacentIndexBuffer ? m_adjacentIndexBuffer : m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    // Set the type of primitive that should be rendered from this vertex buffer
    context->IASetPrimitiveTopology(m_adjacentIndexBuffer ? D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ : Topology() );
    context->DrawIndexed(m_adjacentIndexBuffer ? IndicesWithAdjacencies().size() : Indices().size(),0,0);

    return retVal;
}
//---------------------------------------------
D3D11_PRIMITIVE_TOPOLOGY CMOWModelPart::Topology() const
{
    return m_topology;
}
//---------------------------------------------
void CMOWModelPart::AddVertexAndIndex( const Vertex& vert,long index )
{
    AddVertex(vert);
}
//---------------------------------------------
void CMOWModelPart::AddVertex( const Vertex& vert )
{
    Vertex newVert = vert;
    m_vertices.push_back(newVert);
}
//---------------------------------------------
void CMOWModelPart::AddFace( CMOWFace* face )
{
    m_faces.push_back(face);
    
    for( size_t i=0; i<face->Indices().size(); i++ )
    {
        m_indices.push_back(face->Indices()[i]);
    }
    
}
//---------------------------------------------
const face_vector& CMOWModelPart::Faces() const
{
    return m_faces;
}
//---------------------------------------------
const std::vector<ID3D11ShaderResourceView*>& CMOWModelPart::GetFaceResources() const
{
    return m_resources;   
}
//---------------------------------------------
bool CMOWModelPart::Serialize( std::ofstream& fOut )
{
    
    /*std::map<std::string,int>::iterator itTexture;

    int index = 0;

    CMOWSerializer::WriteSizeT(fOut,m_faces.size());

    for( size_t i=0; i<m_faces.size(); i++ )
    {
        m_faces[i]->Serialize(fOut);
    }
    
    CMOWSerializer::WriteSizeT(fOut,m_vertices.size());
    
    for( size_t i=0; i<m_vertices.size(); i++ )
    {
        CMOWSerializer::WriteBlob(fOut,&m_vertices[i],sizeof(Vertex));
    }

    CMOWSerializer::WriteSizeT(fOut,m_indicesWithAdjacencies.size());

    for( size_t i=0; i<m_indicesWithAdjacencies.size(); i++ )
    {
        CMOWSerializer::WriteBlob(fOut,&m_indicesWithAdjacencies[i],sizeof(long));
    }

    CMOWSerializer::WriteString(fOut,m_materialName);

    CMOWMaterial* mat = CMOWResourceManager::Instance()->Material(m_materialName.c_str());
    if( mat )
    {
        mat->Serialize(fOut);
    }*/

    return true;
}
//---------------------------------------------
bool CMOWModelPart::Serialize( std::ifstream& fIn )
{
    /*std::map<int,std::string> textureIndex;
    std::map<int,std::string>::iterator itTexture;
    int index = 0;

    size_t faceSize = CMOWSerializer::ReadSizeT(fIn);

    for( size_t i=0; i<faceSize; i++ )
    {
        CMOWFace* face = new CMOWFace;
        face->Serialize(fIn);
        AddFace(face);
    }

    size_t vertexSize = CMOWSerializer::ReadSizeT(fIn);

    for( size_t i=0; i<vertexSize; i++ )
    {
        Vertex vert;
        CMOWSerializer::ReadBlob(fIn,&vert,sizeof(Vertex));
        AddVertex(vert);
    }

    size_t indices = CMOWSerializer::ReadSizeT(fIn);

    for( size_t i=0; i<indices; i++ )
    {
        long index = 0;
        CMOWSerializer::ReadBlob(fIn,&index,sizeof(long));
        m_indicesWithAdjacencies.push_back(index);
    }

    m_materialName = CMOWSerializer::ReadString(fIn);

    if( m_materialName.size() )
    {
        CMOWMaterial* material = new CMOWMaterial();
        material->Serialize(fIn);
        CMOWResourceManager::Instance()->AddMaterial(material);
    }*/
    return true;
}
//---------------------------------------------
const char* CMOWModelPart::MaterialName() const
{
    return m_materialName.c_str();
}
//---------------------------------------------
void CMOWModelPart::MaterialName( const char* materialName )
{
    m_materialName = materialName;
}
//---------------------------------------------
ID3D11Buffer* CMOWModelPart::GetVertexBuffer()
{
    if( m_vertexBuffer )
    {
        m_vertexBuffer->AddRef();
    }

    return m_vertexBuffer;
}
//---------------------------------------------
ID3D11Buffer* CMOWModelPart::GetIndexBuffer()
{
    if( m_indexBuffer )
    {
        m_indexBuffer->AddRef();
    }

    return m_indexBuffer;
}
//------------------------------------------------------
const char* CMOWModelPart::Name() const
{
    return m_name.c_str();
}
//------------------------------------------------------
void CMOWModelPart::Name(const char* name)
{
    m_name = name;
}
//------------------------------------------------------
void CMOWModelPart::ToPb(
    PbMOWGraphics::PbMOWModelPart& toPb
    ) const
{
    toPb.set_name(Name());

    for( const Vertex& vert : Vertices() )
    {
        PbMOWGraphics::PbMOWVertex* pbVert = toPb.add_vertices();
        pbVert->mutable_position()->set_x(vert.m_position.x);
        pbVert->mutable_position()->set_y(vert.m_position.y);
        pbVert->mutable_position()->set_z(vert.m_position.z);

        pbVert->mutable_texture()->set_x(vert.m_texture.x);
        pbVert->mutable_texture()->set_y(vert.m_texture.y);
        pbVert->mutable_texture()->set_z(1.0);

        pbVert->mutable_normal()->set_x(vert.m_normal.x);
        pbVert->mutable_normal()->set_y(vert.m_normal.y);
        pbVert->mutable_normal()->set_z(vert.m_normal.z);

        /*pbVert->mutable_color()->set_x(vert.m_color.x);
        pbVert->mutable_color()->set_y(vert.m_color.y);
        pbVert->mutable_color()->set_z(vert.m_color.z);
        pbVert->mutable_color()->set_w(vert.m_color.w);*/
    }

    for( const CMOWFace* face : Faces() )
    {
        PbMOWGraphics::PbMOWFace* pbFace = toPb.add_faces();
        face->ToPb( *pbFace );
    }
    toPb.set_materialname(m_materialName);
}
//------------------------------------------------------
CMOWModelPart* CMOWModelPart::FromPb(
    const PbMOWGraphics::PbMOWModelPart& fromPb
    )
{
    CMOWModelPart* part = new CMOWModelPart(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    part->Name(fromPb.name().c_str());

    for( const PbMOWGraphics::PbMOWVertex& vert : fromPb.vertices() )
    {
        Vertex vertex;

        vertex.m_position.x = vert.position().x();
        vertex.m_position.y = vert.position().y();
        vertex.m_position.z = vert.position().z();

        vertex.m_texture.x = vert.texture().x();
        vertex.m_texture.y = vert.texture().y();
        
        vertex.m_normal.x = vert.normal().x();
        vertex.m_normal.y = vert.normal().y();
        vertex.m_normal.z = vert.normal().z();

        part->AddVertex(vertex);
    }
    for( const PbMOWGraphics::PbMOWFace& face : fromPb.faces() )
    {
        part->AddFace(CMOWFace::FromPb(face));
    }

    part->MaterialName(fromPb.materialname().c_str());

    return part;
}
//------------------------------------------------------
const index_vector& CMOWModelPart::IndicesWithAdjacencies() const
{
    return m_indicesWithAdjacencies;
}
//------------------------------------------------------
index_vector& CMOWModelPart::MutableIndicesWithAdjacencies()
{
    return m_indicesWithAdjacencies;
}