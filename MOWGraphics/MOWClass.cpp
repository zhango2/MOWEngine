#include "MOWClass.h"
#include <map>

static std::map<std::string,CMOWClass*>  s_classes;
//------------------------------------------------------
CMOWAttribute::CMOWAttribute(
    const char* name, 
    const char* displayName,
    MOWAttributeType attrType,
    std::vector<std::string>& valueNames,
    AttributeSetCB setCB,
    AttributeGetCB getCB,
    float minValue,
    float maxValue
    )
{
    m_name = name;
    m_type = attrType;
    m_displayName = displayName;
    m_valueNames = valueNames;
    m_setCB = setCB;
    m_getCB = getCB;
    m_minValue = minValue;
    m_maxValue = maxValue;
}
//------------------------------------------------------
std::string CMOWAttribute::Name() const
{
    return m_name;
}
//------------------------------------------------------
MOWAttributeType CMOWAttribute::AttributeType() const
{
    return m_type;
}
//------------------------------------------------------
std::string CMOWAttribute::DisplayName() const
{
    return m_displayName;
}
//------------------------------------------------------
std::vector<std::string> CMOWAttribute::ValueNames() const
{
    return m_valueNames;
}
//------------------------------------------------------
CMOWVariant* CMOWAttribute::GetValue(
    const CMOWClass* aClass
    ) const
{
    CMOWVariant* retVal = 0;
    if( m_getCB )
    {
        retVal = m_getCB(aClass);
    }

    return retVal;
}
//------------------------------------------------------
void CMOWAttribute::SetValue(
    CMOWClass* aClass,
    CMOWVariant* val
    )
{
    if( m_setCB )
    {
        m_setCB(val,aClass);
    }
}
//------------------------------------------------------
bool CMOWAttribute::HasValueLimits() const
{
    return m_minValue != m_maxValue;
}
//------------------------------------------------------
float CMOWAttribute::MinValue() const
{
    return m_minValue;
}
//------------------------------------------------------
float CMOWAttribute::MaxValue() const
{
    return m_maxValue;
}
//------------------------------------------------------
CMOWClass::CMOWClass()
{

}
//------------------------------------------------------
CMOWClass::~CMOWClass()
{

}
//------------------------------------------------------
void CMOWClass::RegisterMOWClass(const char* name)
{
    auto itClass = s_classes.find(name);

    if( itClass == s_classes.end() )
    {
        CMOWClass* aClass = new CMOWClass;

        s_classes[name] = aClass;
    }
}
//------------------------------------------------------
void CMOWClass::RegisterAttribute(
    const char* className, 
    const char* attributeName,
    const char* displayName,
    MOWAttributeType attrType,
    std::vector<std::string>& valueNames,
    AttributeSetCB setCB,
    AttributeGetCB getCB,
    float minValue,
    float maxValue
    )
{
    auto itClass = s_classes.find(className);

    if( itClass != s_classes.end() )
    {
        CMOWClass* aClass = itClass->second;
        
        CMOWAttribute* attr = new CMOWAttribute(attributeName,displayName,attrType,valueNames,setCB,getCB,minValue,maxValue);
        aClass->AddAttribute(attr);
    }
}
//------------------------------------------------------
const std::vector<CMOWAttribute*>& CMOWClass::Attributes()
{
    auto itClass = s_classes.find(ClassName());

    if( itClass != s_classes.end() )
    {
        CMOWClass* aClass = itClass->second;
        return aClass->m_attributes;
    }

    return m_attributes;
}
//------------------------------------------------------
void CMOWClass::AddAttribute(
    CMOWAttribute* attribute
    )
{
    m_attributes.push_back(attribute);
}
//------------------------------------------------------
std::string CMOWClass::ClassName() const
{
    return "CDXClass";
}
//------------------------------------------------------
CMOWClass* CMOWClass::GetClass(
    const char* name
    )
{
    CMOWClass* aClass = 0;
    auto itClass = s_classes.find(name);

    if( itClass != s_classes.end() )
    {
        aClass = itClass->second;
    }

    return aClass;
}
//------------------------------------------------------
CMOWVariant* CMOWClass::GetAttributeValue(
    const char* attributeName
    ) const
{
    CMOWVariant* retVal = 0;

    if( CMOWAttribute* attr = GetAttribute(attributeName) )
    {
        retVal = attr->GetValue(this);
    }
    return retVal;
}
//------------------------------------------------------
void CMOWClass::SetAttributeValue(
    const char* attributeName,
    CMOWVariant* val
    )
{
    if( CMOWAttribute* attr = GetAttribute(attributeName) )
    {
        attr->SetValue(this,val);
    }
}
//------------------------------------------------------
bool CMOWClass::HasAttribute(
    const char* attributeName
    )
{
    return GetAttribute(attributeName) != 0;
}
//------------------------------------------------------
CMOWVariant* CMOWClass::DoGetAttributeValue(
    const char* attrName
    )
{
    //Override to implement
    return new CMOWVariant(false);
}
//------------------------------------------------------
void CMOWClass::DoSetAttributeValue(
    const char* attrName,
    CMOWVariant* val
    )
{
    //Do nothing
}
//------------------------------------------------------
CMOWAttribute* CMOWClass::GetAttribute(
    const char* attributeName
    )const
{
    CMOWAttribute* retVal = 0;
    auto itClass = s_classes.find(ClassName());

    if( itClass != s_classes.end() )
    {
        for(CMOWAttribute* attr : itClass->second->m_attributes )
        {
            if( attr->Name() == attributeName )
            {
                retVal = attr;
                break;
            }
        }
    }
    return retVal;
}

