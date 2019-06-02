#ifndef MOWBone_H
#define MOWBone_H

#include "MOWCommon.h"

class CMOWBone
{
public:

    CMOWBone(
        const std::string& name,
        const DirectX::XMFLOAT4X4& offsetMatrix
        );

    void
        AddVertexWeight(
            unsigned int vertexId,
            float weight
            );

    const std::string&
        Name(
            ) const;

private:

    std::string
        m_name;

    DirectX::XMFLOAT4X4
        m_offsetMatrix;

    std::vector<std::pair<unsigned int, float>>
        m_vertexWeights;


};

#endif
