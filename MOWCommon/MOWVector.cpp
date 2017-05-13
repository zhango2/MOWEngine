#include "MOWVector.h"
//------------------------------------------------------
CMOWVector::CMOWVector()
{
    X(0.0f);
    Y(0.0f);
    Z(0.0f);
    W(1.0f);
}
//------------------------------------------------------
CMOWVector::CMOWVector(
    float x, 
    float y, 
    float z,
    float w
    )
{
    X(x);
    Y(y);
    Z(z);
    W(w);
}
//------------------------------------------------------
float CMOWVector::X() const
{
    return m_x;
}
//------------------------------------------------------
void CMOWVector::X(
    float x
    )
{
    m_x = x;
}
//------------------------------------------------------
float CMOWVector::Y() const
{
    return m_y;
}
//------------------------------------------------------
void CMOWVector::Y(
    float y
    )
{
    m_y = y;
}
//------------------------------------------------------
float CMOWVector::Z() const
{
    return m_z;
}
//------------------------------------------------------
void CMOWVector::Z(
    float z
    )
{
    m_z = z;
}
//------------------------------------------------------
float CMOWVector::W() const
{
    return m_w;
}
//------------------------------------------------------
void CMOWVector::W(
    float w
    )
{
    m_w = w;
}