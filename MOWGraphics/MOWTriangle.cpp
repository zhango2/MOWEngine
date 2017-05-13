#include "MOWTriangle.h"

CMOWTriangle::CMOWTriangle():
CMOWModel()
{

}

void CMOWTriangle::PopulateVerticesAndIndices()
{
/*    Vertices().clear();
    Indices().clear();

    Vertex vertex;

    vertex.m_position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
    vertex.m_color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    
    Vertices().push_back(vertex);

    vertex.m_position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
    vertex.m_color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
    
    Vertices().push_back(vertex);

    vertex.m_position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
    vertex.m_color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    
    Vertices().push_back(vertex);

    // Load the index array with data.
	Indices().push_back(0);  // Bottom left.
	Indices().push_back(1);  // Top middle.
	Indices().push_back(2);  // Bottom right.*/

}

D3D11_PRIMITIVE_TOPOLOGY CMOWTriangle::Topology() const
{
    return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}