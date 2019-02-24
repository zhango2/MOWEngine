#ifndef _CDXCamera_H
#define _CDXCamera_H

#include "MOWViewPoint.h"
#include "MOWSphere.h"
#include "MOWObject.h"
#include "MOWCommon/MOWInput.h"

class CMOWCamera : 
    public CMOWViewPoint,
    public CMOWSphere,
    public CMOWInput::IEvents
{
    using BaseClass = CMOWViewPoint;
public:
    CMOWCamera(
        float fieldOfView,
        float aspectRatio,
        float nearPlane,
        float farPlane
        );

    virtual 
        ~CMOWCamera(
            );
    
    static CMOWCamera*       
        Create(
            const CMOWPhysics& physics,
            float fieldOfView,
            float aspectRatio,
            float nearPlane,
            float farPlane
            );


    void                    
        Update(
            );

    CMOWRenderableObject*    
        RenderableObject(
            );

    CMOWModel*               
        Model(
            );

    //CMOWInput::IEvents overrides

    void                    
        MouseUpdated(
            int x,
            int y
            );

    void                    
        KeyboardUpdated(
            const std::vector<char>& pressedKeys
            );

    

    static CMOWCamera*
        FromPb( 
            const PbMOWGraphics::PbMOWModel& fromPb, 
            const CMOWPhysics& physics 
            );

protected:

    const CMOWObject*             
        GeometryObject(
            ) const override;

    CMOWObject*
        MutableGeometryObject(
            ) override;

    void
        Init(
            const CMOWPhysics& physics
            );



private:


    int                     
        m_oldMouseX;

    int                     
        m_oldMouseY;

    int                     
        m_mouseXDelta;

    int                     
        m_mouseYDelta;

    float                   
        m_xVelPart;

    float                   
        m_yVelPart;

    float                   
        m_zVelPart;

    float                   
        m_velFactor;
};


#endif