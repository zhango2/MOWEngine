#include "MOWSphere.h"
#include "MOWFace.h"
#include "MOWPhysics/MOWPhysics.h"

using namespace DirectX;
//---------------------------------------------
CMOWSphere::~CMOWSphere()
{

}
//---------------------------------------------
CMOWSphere* CMOWSphere::Create(
    float radius,
    const char* name,
    const char* materialName,
    const CMOWPhysics& physics,
    bool fixed,
    int stacks
    )
{
    CMOWSphere* sphere = new CMOWSphere();
    sphere->SphereConstruct(radius,name,materialName,physics,fixed,stacks);
    return sphere;
}
//---------------------------------------------
CMOWSphere::CMOWSphere()
{
}
//---------------------------------------------
void CMOWSphere::PopulateVerticesAndIndices()
{

    int sliceCount = m_stacks;
    int stackCount = m_stacks;

    CMOWModelPartPtr part = ModelPart("Sphere");


    if( part && part->Vertices().size() == 0 )
    {
        
        //
        // Compute the vertices stating at the top pole and moving down the stacks.
        //

        // Poles: note that there will be texture coordinate distortion as there is
        // not a unique point on the texture map to assign to the pole when mapping
        // a rectangular texture onto a sphere.
        Vertex topVertex;
        Vertex bottomVertex;

        topVertex.m_position = XMFLOAT3(0.0f,m_radius,0.0f);
        bottomVertex.m_position = XMFLOAT3(0.0f,-m_radius,0.0f);

        XMStoreFloat3(&topVertex.m_normal,XMVector3Normalize(XMLoadFloat3(&topVertex.m_position)));
        XMStoreFloat3(&bottomVertex.m_normal,XMVector3Normalize(XMLoadFloat3(&bottomVertex.m_position)));

        part->AddVertex(topVertex);

        float phiStep = (float)XM_PI / stackCount;
        float thetaStep = 2.0f * (float)XM_PI / sliceCount;

        // Compute vertices for each stack ring (do not count the poles as rings).
        for (int i = 1; i <= stackCount - 1; ++i)
        {
            float phi = i * phiStep;

            // Vertices of ring.
            for (int j = 0; j <= sliceCount; ++j)
            {
                float theta = j * thetaStep;

                // spherical to cartesian
                float x = m_radius * (float)sin((double)phi) * (float)cos((double)theta);
                float y = m_radius * (float)cos((double)phi);
                float z = m_radius * (float)sin((double)phi) * (float)sin((double)theta);

                Vertex vert;

                vert.m_position = XMFLOAT3(x, y, z);

                XMStoreFloat3(&vert.m_normal,XMVector3Normalize(XMLoadFloat3(&vert.m_position)));
                
                vert.m_texture = XMFLOAT2(((float)atan2((double)vert.m_normal.x, (double)vert.m_normal.z) / (2.0f * (float)XM_PI) + 0.5f),
                                         (((float)asin((double)vert.m_normal.y)) / (float)XM_PI + 0.5f));
                
                
                part->AddVertex(vert);
                
            }
        }

        part->AddVertex(bottomVertex);

        //
        // Compute indices for top stack.  The top stack was written first to the vertex buffer
        // and connects the top pole to the first ring.
        //

        for (int i = 1; i <= sliceCount; ++i)
        {
            part->AddFace(CMOWFace::Create(0,i+1,i));
        }

        //
        // Compute indices for inner stacks (not connected to poles).
        //

        // Offset the indices to the index of the first vertex in the first ring.
        // This is just skipping the top pole vertex.
        int baseIndex = 1;
        int ringVertexCount = sliceCount + 1;
        for (int i = 0; i < stackCount - 2; ++i)
        {
            for (int j = 0; j < sliceCount; ++j)
            {
                part->AddFace(CMOWFace::Create((baseIndex + i * ringVertexCount + j),
                                              (baseIndex + i * ringVertexCount + j + 1),
                                              (baseIndex + (i + 1) * ringVertexCount + j)));

               
                part->AddFace(CMOWFace::Create((baseIndex + (i + 1) * ringVertexCount + j),
                                              (baseIndex + i * ringVertexCount + j + 1),
                                              (baseIndex + (i + 1) * ringVertexCount + j + 1)));
            }
        }

        //
        // Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
        // and connects the bottom pole to the bottom ring.
        //

        // South pole vertex was added last.
        int southPoleIndex = (int)part->Vertices().size()- 1;

        // Offset the indices to the index of the first vertex in the last ring.
        baseIndex = southPoleIndex - ringVertexCount;

        for (int i = 0; i < sliceCount; ++i)
        {
            part->AddFace(CMOWFace::Create(southPoleIndex,baseIndex + i,baseIndex + i + 1));
        }
        //FindAdjacencies(part);
    }
}
//---------------------------------------------
void CMOWSphere::SphereConstruct( 
    float radius, 
    const char* name, 
    const char* materialName, 
    const CMOWPhysics& physics,
    bool fixed,
    int stacks,
    float mass,
    bool collidable
    )
{
    m_radius = radius;
    m_stacks = stacks;
    Name(name ? name : "Sphere");

    CMOWModelPartPtr part = CreateAndAddModelPart("Sphere",D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    part->MaterialName(materialName ? materialName : "");
    
    m_physicalBody = physics.CreatePhysicalSphere(collidable, fixed, radius, mass);
}


