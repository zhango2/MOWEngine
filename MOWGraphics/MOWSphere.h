#ifndef CDXSphere_H
#define CDXSphere_H

#include "MOWModel.h"

class CMOWPhysics;

class CMOWSphere : 
    public CMOWModel
{
    typedef CMOWModel            BaseClass;

public:
    virtual                     
        ~CMOWSphere(
            );

    static CMOWSphere*           
        Create(
            float radius,
            const char* name,
            const char* materialName,
            const CMOWPhysics& physics,
            bool fixed,
            int stacks = 100
            );



protected:

    CMOWSphere(
        );

    void                        
        SphereConstruct(
            float radius,
            const char* name,
            const char* materialName,
            const CMOWPhysics& physics,
            bool fixed,
            int stacks = 100,
            float mass = 1.0f,
            bool collidable = true
            );

    void                        
        PopulateVerticesAndIndices(
            );

private:
    
    float                       
        m_radius;

    int                         
        m_stacks;

};

#endif