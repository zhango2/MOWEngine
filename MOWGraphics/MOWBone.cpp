#include "MOWBone.h"

using namespace DirectX;

//------------------------------------------------------
CMOWBone::CMOWBone(
    const std::string& name, 
    const DirectX::XMFLOAT4X4& offsetMatrix
    )
{
    m_name = name;
    XMStoreFloat4x4(&m_offsetMatrix, XMLoadFloat4x4(&offsetMatrix));
}
//------------------------------------------------------
void CMOWBone::AddVertexWeight(
    unsigned int vertexId, 
    float weight
    )
{
    m_vertexWeights.push_back({vertexId, weight});
}
//------------------------------------------------------
const std::string& CMOWBone::Name(
    ) const
{
    return m_name;
}