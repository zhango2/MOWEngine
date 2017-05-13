#ifndef CDXVariant_H
#define CDXVariant_H

#include <string>

class CMOWVector;

class CMOWVariant
{
    union
    {
        float               m_float;
        int                 m_int;
        bool                m_bool;
        CMOWVector*           m_color;
        CMOWVector*          m_vector;
        std::string*        m_string;
    }m_val;

public:
    CMOWVariant(
        float val
        );

    CMOWVariant(
        int val
        );

    CMOWVariant(
        bool val
        );

    CMOWVariant(
        CMOWVector* val
        );

    CMOWVariant(
        std::string* val
        );
                

    ~CMOWVariant(
        );

    bool        
        IsFloat(
            )const;

    bool        
        IsInt(
            )const;

    bool        
        IsBool(
            )const;

    bool        
        IsString(
            )const;

    bool        
        IsVector(
            )const;

    float       
        AsFloat(
            )const;

    int         
        AsInt(
            )const;

    bool        
        AsBool(
            )const;

    CMOWVector    
        AsVector(
            )const;

    std::string 
        AsString(
            )const;

private:

    bool        
        m_isFloat;

    bool        
        m_isInt;

    bool        
        m_isBool;

    bool        
        m_isString;

    bool        
        m_isVector;
    
};

class CMOWVariantHandler
{
public:

    CMOWVariantHandler(
        CMOWVariant* variant
        );

    ~CMOWVariantHandler(
        );

    CMOWVariant* 
        Variant(
            );

private:

    CMOWVariant* 
        m_variant;
};

#endif