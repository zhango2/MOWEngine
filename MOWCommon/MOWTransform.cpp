#include "MOWTransform.h"
//------------------------------------------------------
CMOWTransform::CMOWTransform(
    const CMOWVector& position, 
    const CMOWVector& rotationQuaternion
    )
{
    m_position = position;
    m_rotationQuaternion = rotationQuaternion;
}
//------------------------------------------------------
CMOWTransform::CMOWTransform()
{
}
//------------------------------------------------------
const CMOWVector& CMOWTransform::Position() const
{
    return m_position;
}
//------------------------------------------------------
const CMOWVector& CMOWTransform::RotationQuaternion() const
{
    return m_rotationQuaternion;
}