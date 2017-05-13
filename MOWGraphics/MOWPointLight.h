#ifndef CDXPointLight_H
#define CDXPointLight_H

#include "MOWLight.h"
#include "MOWSphere.h"
#include "MOWViewPoint.h"

class CMOWRenderableObject;

class CMOWPointLight :
    public CMOWLight,
    public CMOWSphere
    
{
public:
    CMOWPointLight(
        const char* name
        );
    virtual
        ~CMOWPointLight(
            );

    CMOWRenderableObject*                
        RenderableObject(
            );

    const CMOWModel*                     
        Model(
            ) const override;

    CMOWModel*                           
        MutableModel(
            ) override;

    static CMOWLight*                    
        Create(
            const char* name,
            const CMOWPhysics& physics
            );

    void
        SetupModelAndPhysics(
            const CMOWPhysics& physics
            ) override;

    virtual void                                
        Scale(
            float xScale,
            float yScale,
            float zScale
            ) override;

    virtual void
        Position( 
            float xPos, 
            float yPos, 
            float zPos 
            ) override;

protected:

    virtual const CMOWObject*
        GeometryObject(
            )const override;

    virtual CMOWObject*
        MutableGeometryObject(
            ) override;

    

private:

};

#endif