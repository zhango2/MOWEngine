#include "MOWFrustum.h"
using namespace DirectX;
//---------------------------------------------
CMOWFrustum::CMOWFrustum()
{

}
//---------------------------------------------
CMOWFrustum::~CMOWFrustum()
{

}
//---------------------------------------------
void CMOWFrustum::Update( 
    float screenDepth, 
    XMFLOAT4X4& projectionMatrix,
    XMFLOAT4X4& viewMatrix
    )
{
    float zMinimum, r;
    XMMATRIX matrix;
    XMFLOAT4X4 tempProj = projectionMatrix;
    
    // Calculate the minimum Z distance in the frustum.
    
    zMinimum = -tempProj._43/ tempProj._33;
    r = screenDepth / (screenDepth - zMinimum);
    tempProj._33 = r;
    tempProj._43 = -r * zMinimum;

    // Create the frustum matrix from the view matrix and updated projection matrix.
    matrix = XMMatrixMultiply(XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&tempProj));

    // Calculate near plane of frustum.
    // and far plane of frustum.
    
    float sign = 1.0f;
    for (int i = 0; i < 2; i++)
    {
        sign = i == 1 ? -1.0f : 1.0f;
        m_planes[i] = XMVectorSet(XMVectorGetW(matrix.r[0]) + XMVectorGetZ(matrix.r[0])*sign,
                                  XMVectorGetW(matrix.r[1]) + XMVectorGetZ(matrix.r[1])*sign,
                                  XMVectorGetW(matrix.r[2]) + XMVectorGetZ(matrix.r[2])*sign,
                                  XMVectorGetW(matrix.r[3]) + XMVectorGetZ(matrix.r[3])*sign);

        m_planes[i] = XMPlaneNormalize(m_planes[i]);
    }

    // Calculate left and right plane of frustum.
    
    for (int i = 2; i < 4; i++)
    {
        sign = i == 3 ? -1.0f : 1.0f;
        m_planes[i] = XMVectorSet(XMVectorGetW(matrix.r[0]) + XMVectorGetX(matrix.r[0])*sign,
                                  XMVectorGetW(matrix.r[1]) + XMVectorGetX(matrix.r[1])*sign,
                                  XMVectorGetW(matrix.r[2]) + XMVectorGetX(matrix.r[2])*sign,
                                  XMVectorGetW(matrix.r[3]) + XMVectorGetX(matrix.r[3])*sign);

        m_planes[i] = XMPlaneNormalize(m_planes[i]);
    }

    // Calculate top and bottom plane of frustum.

    for (int i = 4; i < 6; i++)
    {
        sign = i == 5 ? -1.0f : 1.0f;
        m_planes[i] = XMVectorSet(XMVectorGetW(matrix.r[0]) + XMVectorGetY(matrix.r[0])*sign,
                                  XMVectorGetW(matrix.r[1]) + XMVectorGetY(matrix.r[1])*sign,
                                  XMVectorGetW(matrix.r[2]) + XMVectorGetY(matrix.r[2])*sign,
                                  XMVectorGetW(matrix.r[3]) + XMVectorGetY(matrix.r[3])*sign);

        m_planes[i] = XMPlaneNormalize(m_planes[i]);
    }
}
//---------------------------------------------
bool CMOWFrustum::IsPointInFrustum( 
    float x, 
    float y, 
    float z 
    )const
{
    bool retVal = true;
    // Check if the point is inside all six planes of the view frustum.
    for( int i=0; i<6; i++ ) 
    {
        if(XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet(x, y, z,1.0f))) < 0.0f)
        {
            retVal = false;
            break;
        }
    }

    return retVal;
}
//---------------------------------------------
bool CMOWFrustum::IsSphereInFrustum( float xCenter, float yCenter, float zCenter, float radius ) const
{
    bool retVal = true;
    // Check if the radius of the sphere is inside the view frustum.
    for(int i=0; i<6; i++) 
    {
        XMVECTOR vec = XMVectorSet(xCenter, yCenter, zCenter, 1.0f);
        if (XMVectorGetX(XMPlaneDotCoord(m_planes[i],vec )) < -radius)
        {
            retVal = false;
            break;
        }
    }

    return retVal;
}
//---------------------------------------------
bool CMOWFrustum::IsBoxInFrustum( 
    float xCenter, 
    float yCenter, 
    float zCenter, 
    float xLength, 
    float yLength, 
    float zLength 
    )const
{

    bool retVal = true;
    // Check if any one point of the cube is in the view frustum.
    for(int i=0; i<6; i++) 
    {
        XMVECTOR test = XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xLength), (yCenter - yLength), (zCenter - zLength),1.0f));

        if(XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xLength), (yCenter - yLength), (zCenter - zLength),1.0f))) >= 0.0f ||
           XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xLength), (yCenter - yLength), (zCenter - zLength),1.0f))) >= 0.0f ||
           XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xLength), (yCenter + yLength), (zCenter - zLength),1.0f))) >= 0.0f ||
           XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xLength), (yCenter + yLength), (zCenter - zLength),1.0f))) >= 0.0f ||
           XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xLength), (yCenter - yLength), (zCenter + zLength),1.0f))) >= 0.0f ||
           XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xLength), (yCenter - yLength), (zCenter + zLength),1.0f))) >= 0.0f ||
           XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xLength), (yCenter + yLength), (zCenter + zLength),1.0f))) >= 0.0f ||
           XMVectorGetX(XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xLength), (yCenter + yLength), (zCenter + zLength),1.0f))) >= 0.0f)
        {
            continue;
        }
        retVal = false;
        break;
    }

    return retVal;

    
}

