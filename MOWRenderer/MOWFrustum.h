#ifndef CDXFrustum_H
#define CDXFrustum_H

#include "MOWGraphics/MOWCommon.h"

class CMOWFrustum
{
public:

    CMOWFrustum(
        );

    ~CMOWFrustum(
        );

    void            
        Update(
            float screenDepth,
            DirectX::XMFLOAT4X4& projectionMatrix, 
            DirectX::XMFLOAT4X4& viewMatrix
            );

    bool            
        IsPointInFrustum(
            float x, 
            float y, 
            float z
            ) const;


    bool            
        IsSphereInFrustum(
            float xCenter, 
            float yCenter, 
            float zCenter, 
            float radius
            ) const;

    bool            
        IsBoxInFrustum(
            float xCenter, 
            float yCenter, 
            float zCenter, 
            float xLength, 
            float yLength, 
            float zLength
            ) const;

private:

    DirectX::XMVECTOR       
        m_planes[6];

};

#endif