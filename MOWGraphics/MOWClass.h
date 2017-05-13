#ifndef CDXClass_H
#define CDXClass_H

#include <string>
#include <vector>
#include "MOWVariant.h"


enum MOWAttributeType
{
    MOWAttributeType_None,
    MOWAttributeType_Integer,
    MOWAttributeType_String,
    MOWAttributeType_Float,
    MOWAttributeType_Vector
};


class CMOWClass;

typedef  void (*AttributeSetCB)(CMOWVariant* val,CMOWClass* pThis);
typedef  CMOWVariant* (*AttributeGetCB)(const CMOWClass* pThis);

class CMOWAttribute
{
public:

    CMOWAttribute(
        const char* name,
        const char* displayName,
        MOWAttributeType attrType,
        std::vector<std::string>& valueNames,
        AttributeSetCB setCB,
        AttributeGetCB getCB,
        float minValue = -1.0f,
        float maxValue = -1.0f
        );

    std::string                             
        Name(
            ) const;

    std::string                             
        DisplayName(
            ) const;

    MOWAttributeType                         
        AttributeType(
            ) const;

    std::vector<std::string>                
        ValueNames(
            ) const;

    CMOWVariant*                             
        GetValue(
            const CMOWClass* aClass
            ) const;

    void                                    
        SetValue(
            CMOWClass* aClass,
            CMOWVariant* val
            );

    bool                                    
        HasValueLimits(
            ) const;

    float                                   
        MinValue(
            )const;

    float                                   
        MaxValue(
            )const;

private:

    std::string                             
        m_name;

    std::string                             
        m_displayName;

    MOWAttributeType                         
        m_type;

    std::vector<std::string>                
        m_valueNames;

    AttributeSetCB                          
        m_setCB;

    AttributeGetCB                          
        m_getCB;

    float                                   
        m_minValue;

    float                                   
        m_maxValue;

};

class CMOWClass
{
public:
    CMOWClass(
        );

    virtual
        ~CMOWClass(
            );

    static void
        RegisterMOWClass(
            const char* name
            );

    static void                             
        RegisterAttribute(
            const char* className,
            const char* attributeName,
            const char* displayName,
            MOWAttributeType attrType,
            std::vector<std::string>& valueNames,
            AttributeSetCB setCB,
            AttributeGetCB getCB,
            float minValue = -1.0f,
            float maxValue = -1.0f
            );

    static CMOWClass*                        
        GetClass(
            const char* name
            );

    virtual std::string                     
        ClassName(
            )const;

    const std::vector<CMOWAttribute*>&       
        Attributes(
            );

    CMOWVariant*                             
        GetAttributeValue( 
            const char* attributeName 
            ) const;

    void                                    
        SetAttributeValue( 
            const char* attributeName,
            CMOWVariant* val
            );


protected:

    virtual CMOWVariant*                     
        DoGetAttributeValue(
            const char* attrName
            );

    virtual void                            
        DoSetAttributeValue(
            const char* attrName,
            CMOWVariant* val
            );

private:

    bool                                    
        HasAttribute(
            const char* attributeName
            );

    void                                    
        AddAttribute(
            CMOWAttribute* attribute
            );

    CMOWAttribute*                           
        GetAttribute(
            const char* attributeName
            ) const;

    std::vector<CMOWAttribute*>              
        m_attributes;


};

#endif