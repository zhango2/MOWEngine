#include "MOWVariant.h"
#include "MOWCommon/MOWVector.h"

//------------------------------------------------------
CMOWVariant::CMOWVariant(float val)
{
    m_val.m_float = val;
    m_isFloat = true;
    m_isBool = false;
    m_isInt = false;
    m_isString = false;
    m_isVector = false;
}
//------------------------------------------------------
CMOWVariant::CMOWVariant(int val)
{
    m_val.m_int = val;
    m_isInt = true;
    m_isBool = false;
    m_isFloat = false;
    m_isString = false;
    m_isVector = false;
}
//------------------------------------------------------
CMOWVariant::CMOWVariant(bool val)
{
    m_val.m_bool = val;
    m_isBool = true;
    m_isFloat = false;
    m_isInt = false;
    m_isString = false;
    m_isVector = false;
}
//------------------------------------------------------
CMOWVariant::CMOWVariant(std::string* val)
{
    m_val.m_string = val;
    m_isString = true;
    m_isBool = false;
    m_isInt = false;
    m_isVector = false;
}
//------------------------------------------------------
CMOWVariant::CMOWVariant(CMOWVector* vec)
{
    m_val.m_vector = vec;
    m_isVector = true;
    m_isBool = false;
    m_isInt = false;
    m_isString = false;

}
//------------------------------------------------------
CMOWVariant::~CMOWVariant()
{
    if( IsString() )
    {
        delete m_val.m_string;
    }
    else if( IsVector() )
    {
        delete m_val.m_vector;
    }
}
//------------------------------------------------------
bool CMOWVariant::IsFloat() const
{
    return m_isFloat;
}
//------------------------------------------------------
bool CMOWVariant::IsInt() const
{
    return m_isBool;
}
//------------------------------------------------------
bool CMOWVariant::IsBool() const
{
    return m_isBool;
}
//------------------------------------------------------
bool CMOWVariant::IsString() const
{
    return m_isString;
}
//------------------------------------------------------
float CMOWVariant::AsFloat() const
{
    return IsFloat() ? m_val.m_float : 0.0f;
}
//------------------------------------------------------
int CMOWVariant::AsInt() const
{
    return IsInt() ? m_val.m_int : 0;
}
//------------------------------------------------------
bool CMOWVariant::AsBool() const
{
    return IsBool() ? m_val.m_bool : false;
}
//------------------------------------------------------
std::string CMOWVariant::AsString() const
{
    return IsString() ? m_val.m_string->c_str() : "";
}
//------------------------------------------------------
bool CMOWVariant::IsVector() const
{
    return m_isVector;
}
//------------------------------------------------------
CMOWVector CMOWVariant::AsVector() const
{
    return IsVector() ? *m_val.m_vector : CMOWVector();
}
//------------------------------------------------------
//CMOWVariantHandler
//------------------------------------------------------
CMOWVariantHandler::CMOWVariantHandler(CMOWVariant* variant)
{
    m_variant = variant;
}
//------------------------------------------------------
CMOWVariantHandler::~CMOWVariantHandler()
{
    delete m_variant;
}
//------------------------------------------------------
CMOWVariant* CMOWVariantHandler::Variant()
{
    return m_variant;
}
