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
    XMFLOAT4X4& projectionMatrix,
    XMFLOAT4X4& viewMatrix
    )
{
    XMMATRIX matrix = XMMatrixMultiply(XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix));
    
    XMFLOAT4 value0;
    XMFLOAT4 value1;
    XMFLOAT4 value2;
    XMFLOAT4 value3;

    XMStoreFloat4(&value0, matrix.r[0]);
    XMStoreFloat4(&value1, matrix.r[1]);
    XMStoreFloat4(&value2, matrix.r[2]);
    XMStoreFloat4(&value3, matrix.r[3]);

    // Left plane

    m_planes[0] = XMVectorSet(
        value0.w + value0.x,
        value1.w + value1.x,
        value2.w + value2.x,
        value3.w + value3.x
    );

    //Right plane

    m_planes[1] = XMVectorSet(
        value0.w - value0.x,
        value1.w - value1.x,
        value2.w - value2.x,
        value3.w - value3.x
    );

    //Top plane
    m_planes[2] = XMVectorSet(
        value0.w - value0.y,
        value1.w - value1.y,
        value2.w - value2.y,
        value3.w - value3.y
    );

    //Bottom plane
    m_planes[3] = XMVectorSet(
        value0.w + value0.y,
        value1.w + value1.y,
        value2.w + value2.y,
        value3.w + value3.y
    );

    //Near plane
    m_planes[4] = XMVectorSet(
       value0.w + value0.z,
       value1.w + value1.z,
       value2.w + value2.z,
       value3.w + value3.z
    );

    //Far plane
    m_planes[5] = XMVectorSet(
        value0.w - value0.z,
        value1.w - value1.z,
        value2.w - value2.z,
        value3.w - value3.z
    );

    for( int i=0; i<6; i++ )
    {
        m_planes[i] = XMPlaneNormalize(m_planes[i]);
    }


    /*m_planes[0].a = matrix(1,4) + matrix(1,1);
    m_planes[0].b = matrix(2,4) + matrix(2,1);
    m_planes[0].c = matrix(3,4) + matrix(3,1);
    m_planes[0].d = matrix(4,4) + matrix(4,1);*/
    
    //Right plane

    /*m_planes[1].a = viewProjection._14 - viewProjection._11;
    m_planes[1].b = viewProjection._24 - viewProjection._21;
    m_planes[1].c = viewProjection._34 - viewProjection._31;
    m_planes[1].d = viewProjection._44 - viewProjection._41;
    
    //Top plane

    m_planes[2].a = viewProjection._14 - viewProjection._12;
    m_planes[2].b = viewProjection._24 - viewProjection._22;
    m_planes[2].c = viewProjection._34 - viewProjection._32;
    m_planes[2].d = viewProjection._44 - viewProjection._42;
    
    //Bottom plane

    m_planes[3].a = viewProjection._14 + viewProjection._12;
    m_planes[3].b = viewProjection._24 + viewProjection._22;
    m_planes[3].c = viewProjection._34 + viewProjection._32;
    m_planes[3].d = viewProjection._44 + viewProjection._42;
    
    //Near plane

    m_planes[4].a = viewProjection._13;
    m_planes[4].b = viewProjection._23;
    m_planes[4].c = viewProjection._33;
    m_planes[4].d = viewProjection._43;
    
    //Far plane

    m_planes[5].a = viewProjection._14 - viewProjection._13;
    m_planes[5].b = viewProjection._24 - viewProjection._23;
    m_planes[5].c = viewProjection._34 - viewProjection._33;
    m_planes[5].d = viewProjection._44 - viewProjection._43;*/

    /*float zMinimum, r;
    XMMATRIX matrix;
    XMFLOAT4X4 tempProj = projectionMatrix;
    
    // Calculate the minimum Z distance in the frustum.
    
    zMinimum = -tempProj._43/ tempProj._33;
    r = screenDepth / (screenDepth - zMinimum);
    tempProj._33 = r;
    tempProj._43 = -r * zMinimum;

    // Create the frustum matrix from the view matrix and updated projection matrix.
    matrix = XMMatrixMultiply(XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&tempProj));*/

    // Calculate near plane of frustum.
    // and far plane of frustum.
    
    /*float sign = 1.0f;
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
    }*/
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

    xLength = xLength / 2.0f;
    yLength = yLength / 2.0f;
    zLength = zLength / 2.0f;

    XMVECTOR nearBottomLeft =   XMVectorSet((xCenter - xLength), (yCenter - yLength), (zCenter - zLength),1.0f);
    XMVECTOR nearBottomRight =  XMVectorSet((xCenter + xLength), (yCenter - yLength), (zCenter - zLength),1.0f);
    XMVECTOR farBottomLeft =    XMVectorSet((xCenter - xLength), (yCenter - yLength), (zCenter + zLength),1.0f);
    XMVECTOR farBottomRight =   XMVectorSet((xCenter + xLength), (yCenter - yLength), (zCenter + zLength),1.0f);

    XMVECTOR nearTopLeft =   XMVectorSet((xCenter - xLength), (yCenter + yLength), (zCenter - zLength),1.0f);
    XMVECTOR nearTopRight =  XMVectorSet((xCenter + xLength), (yCenter + yLength), (zCenter - zLength),1.0f);
    XMVECTOR farTopLeft =    XMVectorSet((xCenter - xLength), (yCenter + yLength), (zCenter + zLength),1.0f);
    XMVECTOR farTopRight =   XMVectorSet((xCenter + xLength), (yCenter + yLength), (zCenter + zLength),1.0f);


    // Check if any one point of the cube is in the view frustum.
    for(int i=0; i<6; i++) 
    {
        

        XMVECTOR test0 = XMPlaneDotCoord(m_planes[i], nearBottomLeft);
        XMVECTOR test1 = XMPlaneDotCoord(m_planes[i], nearBottomRight);
        XMVECTOR test2 = XMPlaneDotCoord(m_planes[i], farBottomLeft);
        XMVECTOR test3 = XMPlaneDotCoord(m_planes[i], farBottomRight);
        XMVECTOR test4 = XMPlaneDotCoord(m_planes[i], nearTopLeft);
        XMVECTOR test5 = XMPlaneDotCoord(m_planes[i], nearTopRight);
        XMVECTOR test6 = XMPlaneDotCoord(m_planes[i], farTopLeft);
        XMVECTOR test7 = XMPlaneDotCoord(m_planes[i], farTopRight);

        if( XMVectorGetX(test0) > 0.0f )
        {
            continue;
        }
        if( XMVectorGetX(test1) > 0.0f )
        {
            continue;
        }
        if( XMVectorGetX(test2) > 0.0f )
        {
            continue;
        }
        if( XMVectorGetX(test3) > 0.0f )
        {
            continue;
        }
        
        if( XMVectorGetX(test4) > 0.0f )
        {
            continue;
        }
        if( XMVectorGetX(test5) > 0.0f )
        {
            continue;
        }
        if( XMVectorGetX(test6) > 0.0f )
        {
            continue;
        }
        if( XMVectorGetX(test7) > 0.0f )
        {
            continue;
        }

        return false;
        


        /*if(XMVectorGetX(test0) >= 0.0f ||
           XMVectorGetX(test1) >= 0.0f ||
           XMVectorGetX(test2) >= 0.0f ||
           XMVectorGetX(test3) >= 0.0f ||
           XMVectorGetX(test4) >= 0.0f ||
           XMVectorGetX(test5) >= 0.0f ||
           XMVectorGetX(test6) >= 0.0f ||
           XMVectorGetX(test7) >= 0.0f)
        {
            continue;
        }
        retVal = false;
        break;*/
    }

    return retVal;

    
}

