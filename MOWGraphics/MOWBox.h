#ifndef _DXBOX_H
#define _DXBOX_H

#include "MOWModel.h"

class CMOWPhysics;
class CMOWBox : public CMOWModel
{
    typedef CMOWModel BaseClass;
public:

                   
    virtual
        ~CMOWBox(
            );

    static CMOWBox*              
        Create(
            float width,
            float height,
            float depth,
            const char* name,
            const char* materialName,
            const CMOWPhysics& physics,
            bool collidable,
            bool fixed
            );

    float                       
        Width(
            ) const;

    float                       
        Height(
            ) const;

    float                       
        Depth(
            ) const;

    

protected:
    CMOWBox(
        );

    void                        
        PopulateVerticesAndIndices(
            );

    void                        
        BoxConstruct(
            float width,
            float height,
            float depth,
            const char* name,
            const char* materialName,
            const CMOWPhysics& physics,
            bool collidable,
            bool fixed
            );

    void                        
        Width(
            float width
            );

    void                        
        Height(
            float height
            );

    void                        
        Depth(
            float depth
            );

private:

    float                       
        m_width;

    float                       
        m_height;

    float                       
        m_depth;

};

#endif