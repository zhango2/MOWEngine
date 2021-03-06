#include "MOWViewPoint.h"
#include "MOWObject.h"
#include "MOWRenderableObject.h"
using namespace DirectX;
//--------------------------------------------
CMOWViewPoint::CMOWViewPoint(
    float fieldOfView,
    float aspectRatio,
    float nearPlane,
    float farPlane
    )
{
    m_aspectRatio = aspectRatio;
    m_farPlane = farPlane;
    m_nearPlane = nearPlane;
    m_fieldOfView = fieldOfView;

    LookAt(0.0f,0.0f,1.0f);

    // Create the projection matrix for the view point.
    XMStoreFloat4x4(&MutableProjectionMatrix(),XMMatrixPerspectiveFovLH(m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane));
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

        // Setup the vector that points upwards.
        up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
        lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);

        // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
        lookAt = XMVector3TransformCoord(lookAt, XMLoadFloat4x4(&GeometryObject()->RotationMatrix()));
        up = XMVector3TransformCoord(up, XMLoadFloat4x4(&GeometryObject()->RotationMatrix()));

        // Translate the rotated camera position to the location of the viewer.

        lookAt = GeometryObject()->Position() + lookAt;

        // Create the view matrix from the three updated vectors.
        XMStoreFloat4x4(&MutableViewMatrix(),XMMatrixLookAtLH(GeometryObject()->Position(), lookAt, up));
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
    return GeometryObject()->Position() + m_lookAt;
}

//---------------------------------------------
CMOWRenderableObject* CMOWViewPoint::RenderableObject()
{
    return 0;
}
//---------------------------------------------
CMOWModelPtrC CMOWViewPoint::Model() const
{
    return 0;
}
//------------------------------------------------------
CMOWModelPtr CMOWViewPoint::MutableModel()
{
    return 0;
}
