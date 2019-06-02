#include "MOWFace.h"
#include "MOWMessages/MOWGraphics.pb.h"

using namespace DirectX;
//---------------------------------------------
CMOWFace::~CMOWFace()
{

}
//---------------------------------------------
CMOWFacePtr CMOWFace::Create( 
    long index1, 
    long index2, 
    long index3
    )
{
    return CMOWFacePtr(new CMOWFace(index1,index2,index3));
}
//---------------------------------------------
CMOWFace::CMOWFace( 
    long index1, 
    long index2, 
    long index3
    )
{
    m_indices.push_back(index1);
    m_indices.push_back(index2);
    m_indices.push_back(index3);
    m_adjCount = 0;
}
//---------------------------------------------
CMOWFace::CMOWFace()
{
    m_adjCount = 0;
}
//---------------------------------------------
const std::vector<long>& CMOWFace::Indices() const
{
    return m_indices;
}
//---------------------------------------------
bool CMOWFace::Serialize( std::ofstream& fOut )
{
    size_t indexSize = m_indices.size();
    fOut.write(reinterpret_cast<const char*>(&indexSize),sizeof(size_t));
    
    for( size_t i=0; i<m_indices.size(); i++ )
    {
        fOut.write(reinterpret_cast<const char*>(&m_indices[i]),sizeof(long));
    }

    return true;
}
//---------------------------------------------
bool CMOWFace::Serialize( std::ifstream& fIn )
{
    size_t indexSize;

    fIn.read(reinterpret_cast<char*>(&indexSize),sizeof(size_t));

    for( size_t i=0; i<indexSize; i++ )
    {
        long index;

        fIn.read(reinterpret_cast<char*>(&index),sizeof(long));
        m_indices.push_back(index);
    }

    return true;
}
//---------------------------------------------
const std::vector<long>& CMOWFace::AdjecentIndices() const
{
    return m_adjIndices;
}
//---------------------------------------------
void CMOWFace::AddIndex( long index )
{
    m_indices.push_back(index);
}
//---------------------------------------------
void CMOWFace::AddAdjecentIndex( long index )
{
    m_adjIndices.push_back(index);
}
//---------------------------------------------
XMFLOAT3 CMOWFace::CalculateNormal( 
    XMFLOAT3 p1, 
    XMFLOAT3 p2 
    ) const
{
    XMFLOAT3 retVal;
    XMStoreFloat3(&retVal,XMVector3Cross(XMLoadFloat3(&p1),XMLoadFloat3(&p2)));
    return retVal;
}
//---------------------------------------------
void CMOWFace::CreateAdjecentEdgesAndNormals(const vertex_vector& vertices,XMFLOAT4X4 worldMatrix)
{
    m_adjFaces.clear();
    m_adjEdges.clear();
    XMFLOAT3 e1;
    XMFLOAT3 e2;
    XMFLOAT3 e3;
    XMFLOAT3 e4;
    XMFLOAT3 e5;
    XMFLOAT3 e6;

    e1 = CreateEdge(vertices[m_indices[1]].m_position,vertices[m_indices[0]].m_position,worldMatrix);
    m_adjEdges.push_back(e1);

    e2 = CreateEdge(vertices[m_indices[2]].m_position,vertices[m_indices[0]].m_position,worldMatrix);
    m_adjEdges.push_back(e2);
    m_normal = CalculateNormal(e1,e2);

    CMOWFace adjFace = CMOWFace(m_adjIndices[0],m_indices[0],m_indices[1]);
    e3 = CreateEdge(vertices[m_adjIndices[0]].m_position,vertices[m_indices[0]].m_position,worldMatrix);
    m_adjEdges.push_back(e3);
    adjFace.m_normal = CalculateNormal(e3,e1);
    m_adjFaces.push_back(adjFace);

    adjFace = CMOWFace(m_adjIndices[1],m_indices[1],m_indices[2]);
    e4 = CreateEdge(vertices[m_adjIndices[1]].m_position,vertices[m_indices[1]].m_position,worldMatrix);
    m_adjEdges.push_back(e4);

    e5 = CreateEdge(vertices[m_indices[2]].m_position,vertices[m_indices[1]].m_position,worldMatrix);
    m_adjEdges.push_back(e5);
    adjFace.m_normal = CalculateNormal(e4,e5);
    m_adjFaces.push_back(adjFace);

    adjFace = CMOWFace(m_adjIndices[2],m_indices[0],m_indices[2]);
    e6 = CreateEdge(vertices[m_adjIndices[2]].m_position,vertices[m_indices[0]].m_position,worldMatrix);

    m_adjEdges.push_back(e6);
    adjFace.m_normal = CalculateNormal(e2,e6);
    m_adjFaces.push_back(adjFace);
    
}
//---------------------------------------------
DirectX::XMFLOAT4 CMOWFace::Float3ToFloat4( 
    DirectX::XMFLOAT3 src 
    )const
{
    XMFLOAT4 retVal;

    retVal.x = src.x;
    retVal.y = src.y;
    retVal.z = src.z;
    retVal.w = 1.0f;

    return retVal;

}
//---------------------------------------------
DirectX::XMFLOAT3 CMOWFace::CreateEdge( 
    DirectX::XMFLOAT3 p1, 
    DirectX::XMFLOAT3 p2, 
    DirectX::XMFLOAT4X4 worldMatrix 
    )const
{
    XMFLOAT3 edge;
    XMFLOAT4 tempFloat1;
    XMFLOAT4 tempFloat2;
    XMVECTOR tempVec1;
    XMVECTOR tempVec2;

    tempFloat1 = Float3ToFloat4(p1);
    tempFloat2 = Float3ToFloat4(p2);

    tempVec1 = XMVector4Transform(XMLoadFloat4(&tempFloat1),XMLoadFloat4x4(&worldMatrix));
    tempVec2 = XMVector4Transform(XMLoadFloat4(&tempFloat2),XMLoadFloat4x4(&worldMatrix));

    XMStoreFloat3(&edge,XMVectorSubtract(tempVec2,tempVec1));

    return edge;
}
//---------------------------------------------
DirectX::XMFLOAT3 CMOWFace::Normal() const
{
    return m_normal;
}
//---------------------------------------------
const std::vector<CMOWFace>& CMOWFace::NeighborFaces() const
{
    return m_adjFaces;
}
//------------------------------------------------------
void CMOWFace::IncreaseAdjacentCount()
{
    m_adjCount++;
}
//------------------------------------------------------
int CMOWFace::AdjacentCount() const
{
    return m_adjCount;
}
//------------------------------------------------------
void CMOWFace::ToPb(
    PbMOWGraphics::PbMOWFace& toPb
    ) const
{
    for( const long& index : Indices() )
    {
        toPb.add_indices(index);
    }
}
//------------------------------------------------------
CMOWFacePtr CMOWFace::FromPb(
    const PbMOWGraphics::PbMOWFace& fromPb
    )
{
    CMOWFacePtr face = CMOWFacePtr(new CMOWFace);

    for( const int& index : fromPb.indices() )
    {
        face->AddIndex(index);
    }

    return face;
}
//---------------------------------------------
CMOWEdge::CMOWEdge( 
    long index1, 
    long index2 
    )
{
    
    m_index1 = index1 < index2 ? index1 : index2;
    m_index2 = index1 < index2 ? index2 : index1;
    
}
//---------------------------------------------
CMOWEdge::~CMOWEdge()
{

}
//---------------------------------------------
CMOWNeighbors::CMOWNeighbors()
{
    m_index1 = -1;
    m_index2 = -1;
}
//---------------------------------------------
CMOWNeighbors::~CMOWNeighbors()
{

}
//---------------------------------------------
long CMOWNeighbors::GetOther( 
    long me 
    ) const
{
    return me == m_index1 ? m_index2 : m_index1;
}
//---------------------------------------------
void CMOWNeighbors::AddNeighbor( 
    long index 
    )
{
    if( m_index1 == -1 )
    {
        m_index1 = index;
    }
    else if( m_index2 == -1 )
    {
        m_index2 = index;
    }
}
//---------------------------------------------
bool CMOWCompareEdges::operator()( 
    const CMOWEdge& edge1, 
    const CMOWEdge& edge2 
    )
{
    bool retVal = false;

    retVal = edge1.m_index1 < edge2.m_index1;

    if( !retVal && edge1.m_index1 == edge2.m_index1 )
    {
        retVal = edge1.m_index2 < edge2.m_index2;
    }
    
    return retVal;
}
//---------------------------------------------
bool CMOWCompareXMFLOAT3::operator()( 
    const XMFLOAT3& vec1, 
    const XMFLOAT3& vec2 
    )
{
    bool retVal = vec1.x < vec2.x;

    if( !retVal && vec1.x == vec2.x )
    {
        retVal = vec1.y < vec2.y;

        if( !retVal && vec1.y == vec2.y )
        {
            retVal = vec1.z < vec2.z;
        }
    }

    return retVal;
}
