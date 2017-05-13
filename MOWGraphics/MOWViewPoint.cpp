#include "MOWViewPoint.h"
#include "MOWObject.h"
#include "MOWRenderableObject.h"
using namespace DirectX;
//--------------------------------------------
CMOWViewPoint::CMOWViewPoint()
{
    m_aspectRatio = 1.0f;
    m_farPlane = 100.0f;
    m_fieldOfView = ((float)XM_PI / 2.0f);
    m_nearPlane = 0.1f;
    LookAt(0.0f,0.0f,1.0f);
}
//---------------------------------------------
CMOWViewPoint::~CMOWViewPoint()
{

}
//---------------------------------------------
void CMOWViewPoint::UpdateViewPoint()
{
 
    if( GeometryObject() )
    {
        MutableGeometryObject()->Update();
        XMVECTOR up, lookAt;

        lookAt = m_lookAt;

        // Setup the vector that points upwards.
        up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

        // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
        lookAt = XMVector3TransformCoord(lookAt, XMLoadFloat4x4(&GeometryObject()->RotationMatrix()));
        up = XMVector3TransformCoord(up, XMLoadFloat4x4(&GeometryObject()->RotationMatrix()));

        // Translate the rotated camera position to the location of the viewer.

        lookAt = XMLoadFloat3(&GeometryObject()->Position()) + lookAt;

        // Create the view matrix from the three updated vectors.
        XMStoreFloat4x4(&MutableViewMatrix(),XMMatrixLookAtLH(XMLoadFloat3(&GeometryObject()->Position()), lookAt, up));

        // Create the projection matrix for the view point.
        XMStoreFloat4x4(&MutableProjectionMatrix(),XMMatrixPerspectiveFovLH(m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane));
    }
    
}
//---------------------------------------------
const XMFLOAT4X4& CMOWViewPoint::GetViewMatrix() const
{
    return m_viewMatrix;
}
//---------------------------------------------
const XMFLOAT4X4& CMOWViewPoint::GetProjectionMatrix() const
{
    return m_projectionMatrix;
}
//------------------------------------------------------
DirectX::XMFLOAT4X4& CMOWViewPoint::MutableViewMatrix()
{
    return m_viewMatrix;
}
//------------------------------------------------------
DirectX::XMFLOAT4X4& CMOWViewPoint::MutableProjectionMatrix()
{
    return m_projectionMatrix;
}
//---------------------------------------------
void CMOWViewPoint::SetProjectionParameters( 
    float fieldOfView, 
    float aspectRatio, 
    float nearPlane, 
    float farPlane 
    )
{
    m_fieldOfView = fieldOfView;
    m_aspectRatio = aspectRatio;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
}
//---------------------------------------------
void CMOWViewPoint::LookAt(
    float x, 
    float y, 
    float z)
{
    m_lookAt = XMVectorSet(x, y, z, 1.0f);
}
//---------------------------------------------
DirectX::XMVECTOR CMOWViewPoint::LookAt()
{
    return XMLoadFloat3(&GeometryObject()->Position()) + m_lookAt;
}

//---------------------------------------------
CMOWRenderableObject* CMOWViewPoint::RenderableObject()
{
    return 0;
}
//---------------------------------------------
const CMOWModel* CMOWViewPoint::Model() const
{
    return 0;
}
//------------------------------------------------------
CMOWModel* CMOWViewPoint::MutableModel()
{
    return 0;
}