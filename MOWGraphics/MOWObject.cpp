#include "MOWObject.h"
#include <iostream>
#include <fstream>
#include "MOWCommon/MOWSerializer.h"

using namespace DirectX;

CMOWObject::CMOWObject()
{
    m_scale = XMFLOAT3(1.0f,1.0f,1.0f);

    m_rotation = XMQuaternionIdentity();
    m_totalRotation = XMQuaternionIdentity();
    m_position = XMVectorSet(0.0f,0.0f,0.0f, 1.0f);
    m_totalPosition = XMVectorSet(0.0f,0.0f,0.0f, 1.0f);

    XMStoreFloat4x4(&m_scaleMatrix,XMMatrixIdentity());
    XMStoreFloat4x4(&m_positionMatrix,XMMatrixIdentity());
    XMStoreFloat4x4(&m_rotationMatrix,XMMatrixIdentity());
}
//------------------------------------------------------
CMOWObject::~CMOWObject()
{

}
//------------------------------------------------------
void CMOWObject::Position(
    float xPos,
    float yPos,
    float zPos
    )
{
    m_position = XMVectorSet(xPos, yPos, zPos, 1.0f);
}
//------------------------------------------------------
void CMOWObject::Rotation(
    float xRot,
    float yRot,
    float zRot
    )
{
    XMFLOAT4 temp = XMFLOAT4(xRot,yRot, zRot, 1.0f);
    m_rotation = XMLoadFloat4(&temp);
}
//------------------------------------------------------
const XMVECTOR& CMOWObject::Position() const
{
    return m_position;
}
//------------------------------------------------------
const XMVECTOR& CMOWObject::Rotation() const
{
    return m_rotation;
}
//------------------------------------------------------
void CMOWObject::Update()
{
    XMStoreFloat4x4(&MutableRotationMatrix(),XMMatrixRotationQuaternion(Rotation()));
    XMStoreFloat4x4(&MutablePositionMatrix(),XMMatrixTranslation(XMVectorGetX(Position()), XMVectorGetY(Position()), XMVectorGetZ(Position())));
    XMStoreFloat4x4(&MutableScaleMatrix(),XMMatrixScaling(Scale().x, Scale().y, Scale().z));
}
//------------------------------------------------------
const DirectX::XMFLOAT4X4& CMOWObject::RotationMatrix() const
{
    return m_rotationMatrix;
}
//------------------------------------------------------
DirectX::XMFLOAT4X4& CMOWObject::MutableRotationMatrix()
{
    return m_rotationMatrix;
}
//------------------------------------------------------
const DirectX::XMFLOAT4X4& CMOWObject::PositionMatrix() const
{
    return m_positionMatrix;
}
//------------------------------------------------------
DirectX::XMFLOAT4X4& CMOWObject::MutablePositionMatrix()
{
    return m_positionMatrix;
}
//------------------------------------------------------
XMFLOAT4X4 CMOWObject::WorldMatrix() const
{
    XMFLOAT4X4 retVal;
    XMMATRIX tempMatrix = XMMatrixMultiply(XMLoadFloat4x4(&RotationMatrix()),XMLoadFloat4x4(&ScaleMatrix()));
    tempMatrix = XMMatrixMultiply(tempMatrix,XMLoadFloat4x4(&PositionMatrix()));
    XMStoreFloat4x4(&retVal,tempMatrix);
    return retVal;

}
//---------------------------------------------
void CMOWObject::AngularVelocity( float xVel, float yVel, float zVel )
{
    //Do nothing
}
//---------------------------------------------
void CMOWObject::AngularDamping( float damping )
{
    //Do nothing
}
//---------------------------------------------
void CMOWObject::LinearVelocity( 
    float xVel, 
    float yVel, 
    float zVel 
    )
{
    //Do nothing
}
//---------------------------------------------
const DirectX::XMFLOAT4X4& CMOWObject::ScaleMatrix() const
{
    return m_scaleMatrix;
}
//------------------------------------------------------
DirectX::XMFLOAT4X4& CMOWObject::MutableScaleMatrix()
{
    return m_scaleMatrix;
}
//---------------------------------------------
void CMOWObject::Scale( 
    float xScale, 
    float yScale, 
    float zScale 
    )
{
    m_scale = XMFLOAT3(xScale,yScale,zScale);
}
//---------------------------------------------
const DirectX::XMFLOAT3& CMOWObject::Scale() const
{
    return m_scale;
}
//------------------------------------------------------
CMOWVector CMOWObject::ScaleVector() const
{
    return CMOWVector(m_scale.x, m_scale.y, m_scale.z);
}
//------------------------------------------------------
const std::string& CMOWObject::Name() const
{
    return m_name;
}
//------------------------------------------------------
void CMOWObject::Name(const char* name)
{
    m_name = name;
}
