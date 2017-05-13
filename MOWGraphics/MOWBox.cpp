#include "MOWBox.h"
#include "MOWFace.h"
#include "MOWPhysics/MOWPhysics.h"

using namespace DirectX;
//---------------------------------------------
CMOWBox::CMOWBox():
CMOWModel()
{
}
//---------------------------------------------
CMOWBox::~CMOWBox()
{

}
//---------------------------------------------
void CMOWBox::PopulateVerticesAndIndices()
{

    CMOWModelPart* part = ModelPart("Box");
    
    if( part && !part->Vertices().size() )
    {
        int count = 0;
        Vertex vertex;

        float halfWidth = m_width / 2.0f;
        float halfHeight = m_height / 2.0f;
        float halfDepth = m_depth / 2.0f;

        //Front
        
        vertex.m_position = XMFLOAT3(halfWidth*-1.0f, halfHeight*-1.0f, halfDepth*-1.0f);  // Bottom left
        vertex.m_texture = XMFLOAT2(0.0f, 1.0f);
        vertex.m_normal = XMFLOAT3(0.0f,0.0f,-1.0f);
        part->AddVertex(vertex);

        vertex.m_position = XMFLOAT3(halfWidth*-1.0f, halfHeight, halfDepth*-1.0f);  // Top left
        vertex.m_texture = XMFLOAT2(0.0f, 0.0f);
        vertex.m_normal = XMFLOAT3(0.0f,0.0f,-1.0f);
        part->AddVertex(vertex);
        
        vertex.m_position = XMFLOAT3(halfWidth, halfHeight, halfDepth*-1.0f);  // Top right
        vertex.m_texture = XMFLOAT2(1.0f, 0.0f);
        vertex.m_normal = XMFLOAT3(0.0f,0.0f,-1.0f);
        part->AddVertex(vertex);

        vertex.m_position = XMFLOAT3(halfWidth, halfHeight*-1.0f, halfDepth*-1.0f);  // Bottom right
        vertex.m_texture = XMFLOAT2(1.0f, 1.0f);
        vertex.m_normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
        part->AddVertex(vertex);

        part->AddFace(CMOWFace::Create(count+0,count+1,count+2));
        part->AddFace(CMOWFace::Create(count+0,count+2,count+3));

        count+=4;


        //-------------------------------------------------------------------------------------------------
        //Right

        vertex.m_position = XMFLOAT3(halfWidth, halfHeight*-1.0f, halfDepth*-1.0f);  // Bottom left
        vertex.m_texture = XMFLOAT2(0.0f, 1.0f);
        vertex.m_normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
        part->AddVertex(vertex);

        vertex.m_position = XMFLOAT3(halfWidth, halfHeight, halfDepth*-1.0f);  // Top left
        vertex.m_texture = XMFLOAT2(0.0f, 0.0f);
        vertex.m_normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
        part->AddVertex(vertex);

        vertex.m_position = XMFLOAT3(halfWidth, halfHeight, halfDepth);  // Top right
        vertex.m_texture = XMFLOAT2(1.0f, 0.0f);
        vertex.m_normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
        part->AddVertex(vertex);

        vertex.m_position = XMFLOAT3(halfWidth, halfHeight*-1.0f, halfDepth);  // Bottom right
        vertex.m_texture = XMFLOAT2(1.0f, 1.0f);
        vertex.m_normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
        part->AddVertex(vertex);

        part->AddFace(CMOWFace::Create(count+0,count+1,count+2));
        part->AddFace(CMOWFace::Create(count+0,count+2,count+3));

        count+=4;

        //-------------------------------------------------------------------------------------------------
        //Back

        vertex.m_position = XMFLOAT3(halfWidth, halfHeight*-1.0f, halfDepth);  // Bottom left
        vertex.m_texture = XMFLOAT2(0.0f, 1.0f);
        vertex.m_normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
        part->AddVertex(vertex);

        vertex.m_position = XMFLOAT3(halfWidth, halfHeight, halfDepth);  // Top left
        vertex.m_texture = XMFLOAT2(0.0f, 0.0f);
        vertex.m_normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
        part->AddVertex(vertex);

        vertex.m_position = XMFLOAT3(halfWidth*-1.0f, halfHeight, halfDepth);  // Top right
        vertex.m_texture = XMFLOAT2(1.0f, 0.0f);
        vertex.m_normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
        part->AddVertex(vertex);

        vertex.m_position = XMFLOAT3(halfWidth*-1.0f, halfHeight*-1.0f, halfDepth);  // Bottom right
        vertex.m_texture = XMFLOAT2(1.0f, 1.0f);
        vertex.m_normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
        part->AddVertex(vertex);

        part->AddFace(CMOWFace::Create(count+0,count+1,count+2));
        part->AddFace(CMOWFace::Create(count+0,count+2,count+3));

        count+=4;

        //-------------------------------------------------------------------------------------------------
        //Left

        vertex.m_position = XMFLOAT3(halfWidth*-1.0f, halfHeight*-1.0f, halfDepth);  // Bottom left
        vertex.m_texture = XMFLOAT2(0.0f, 1.0f);
        vertex.m_normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
        part->AddVertex(vertex);

        vertex.m_position = XMFLOAT3(halfWidth*-1.0f, halfHeight, halfDepth);  // Top left
        vertex.m_texture = XMFLOAT2(0.0f, 0.0f);
        vertex.m_normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
        part->AddVertex(vertex);

        vertex.m_position = XMFLOAT3(halfWidth*-1.0f, halfHeight, halfDepth*-1.0f);  // Top right
        vertex.m_texture = XMFLOAT2(1.0f, 0.0f);
        vertex.m_normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
        part->AddVertex(vertex);

        vertex.m_position = XMFLOAT3(halfWidth*-1.0f, halfHeight*-1.0f, halfDepth*-1.0f);  // Bottom right
        vertex.m_texture = XMFLOAT2(1.0f, 1.0f);
        vertex.m_normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
        part->AddVertex(vertex);

        part->AddFace(CMOWFace::Create(count+0,count+1,count+2));
        part->AddFace(CMOWFace::Create(count+0,count+2,count+3));

        count+=4;

        //-------------------------------------------------------------------------------------------------
        //Top
        
        vertex.m_position = XMFLOAT3(halfWidth, halfHeight, halfDepth);  // Bottom left
        vertex.m_texture = XMFLOAT2(0.0f, 1.0f);
        vertex.m_normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
        part->AddVertex(vertex);
        
        vertex.m_position = XMFLOAT3(halfWidth, halfHeight, halfDepth*-1.0f);  // Top left
        vertex.m_texture = XMFLOAT2(0.0f, 0.0f);
        vertex.m_normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
        part->AddVertex(vertex);

        vertex.m_position = XMFLOAT3(halfWidth*-1.0f, halfHeight, halfDepth*-1.0f);  // Top right
        vertex.m_texture = XMFLOAT2(1.0f, 0.0f);
        vertex.m_normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
        part->AddVertex(vertex);

        vertex.m_position = XMFLOAT3(halfWidth*-1.0f, halfHeight, halfDepth);  // Bottom right
        vertex.m_texture = XMFLOAT2(1.0f, 1.0f);
        vertex.m_normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
        part->AddVertex(vertex);

        part->AddFace(CMOWFace::Create(count+0,count+1,count+2));
        part->AddFace(CMOWFace::Create(count+0,count+2,count+3));

        count+=4;

        //-------------------------------------------------------------------------------------------------
        //Bottom
        
        vertex.m_position = XMFLOAT3(halfWidth*-1.0f, halfHeight*-1.0f, halfDepth);  // Bottom left
        vertex.m_texture = XMFLOAT2(0.0f, 1.0f);
        vertex.m_normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
        part->AddVertex(vertex);

        vertex.m_position = XMFLOAT3(halfWidth*-1.0f, halfHeight*-1.0f, halfDepth*-1.0f);  // Top left
        vertex.m_texture = XMFLOAT2(0.0f, 0.0f);
        vertex.m_normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
        part->AddVertex(vertex);

        vertex.m_position = XMFLOAT3(halfWidth, halfHeight*-1.0f, halfDepth*-1.0f);  // Top right
        vertex.m_texture = XMFLOAT2(1.0f, 0.0f);
        vertex.m_normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
        part->AddVertex(vertex);

        vertex.m_position = XMFLOAT3(halfWidth, halfHeight*-1.0f, halfDepth);  // Bottom right
        vertex.m_texture = XMFLOAT2(1.0f, 1.0f);
        vertex.m_normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
        part->AddVertex(vertex);


        part->AddFace(CMOWFace::Create(count+0,count+1,count+2));
        part->AddFace(CMOWFace::Create(count+0,count+2,count+3));

        FindAdjacencies(part);
        
    }
}
//---------------------------------------------
CMOWBox* CMOWBox::Create(
    float width,
    float height,
    float depth,
    const char* name,
    const char* materialName,
    const CMOWPhysics& physics,
    bool collidable,
    bool fixed
    )
{
    CMOWBox* box = new CMOWBox();
    box->BoxConstruct(width,height,depth,name,materialName,physics,collidable,fixed);
    return box;
    
}
//---------------------------------------------
float CMOWBox::Width() const
{
    return m_width;
}
//---------------------------------------------
void CMOWBox::Width( 
    float width 
    )
{
    m_width = width;
}

//---------------------------------------------
float CMOWBox::Height() const
{
    return m_height;
}
//---------------------------------------------
void CMOWBox::Height( 
    float height 
    )
{
    m_height = height;
}

//---------------------------------------------
float CMOWBox::Depth() const
{
    return m_depth;
}
//---------------------------------------------
void CMOWBox::Depth( 
    float depth 
    )
{
    m_depth = depth;
}
//---------------------------------------------
void CMOWBox::BoxConstruct( 
    float width, 
    float height, 
    float depth, 
    const char* name, 
    const char* materialName,
    const CMOWPhysics& physics,
    bool collidable,
    bool fixed
    )
{
    CMOWModelPart* part = CreateAndAddModelPart("Box",D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Width(width);
    Height(height);
    Depth(depth);
    Name(name ? name : "Box");

    part->MaterialName(materialName ? materialName : "");

    m_physicalBody = physics.CreatePhysicalBox(collidable,fixed, Width(),Height(),Depth(),100.0f);
    
    PopulateVerticesAndIndices();
    CreateBoundingVolumes();
}
