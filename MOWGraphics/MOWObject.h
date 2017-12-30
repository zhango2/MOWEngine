#ifndef _DXOBJECT_H
#define _DXOBJECT_H

#include "MOWCommon.h"
#include "MOWAlignedObject.h"
#include "MOWCommon/MOWVector.h"

class CMOWObject : 
    public CMOWAlignedObject
{

public:

                    CMOWObject();
    virtual         ~CMOWObject();
    
    virtual void            
        Position(
            float xPos,
            float yPos,
            float zPos
            );

    virtual void            
        Rotation(
            float xRot,
            float yRot,
            float zRot
            );

    virtual void            
        Scale(
            float xScale,
            float yScale,
            float zScale
            );

    virtual void            
        AngularVelocity(
            float xVel,
            float yVel,
            float zVel
            );

    virtual void            
        AngularDamping(
            float damping
            );

    virtual void            
        LinearVelocity(
            float xVel,
            float yVel,
            float zVel
            );

    virtual void            
        Update(
            );            

    virtual const DirectX::XMFLOAT3&      
        Position(
            ) const;

    virtual const DirectX::XMFLOAT4&      
        Rotation(
            ) const;
    
    virtual const DirectX::XMFLOAT3& 
        Scale(
            ) const;

    virtual CMOWVector
        ScaleVector(
            ) const;

    virtual const std::string&             
        Name(
            ) const;

    virtual void                    
        Name(
            const char* name
            );
    
    const DirectX::XMFLOAT4X4& 
        RotationMatrix(
            ) const;

    DirectX::XMFLOAT4X4& 
        MutableRotationMatrix(
            );
    
    const DirectX::XMFLOAT4X4&
        PositionMatrix( 
            ) const;

    DirectX::XMFLOAT4X4&
        MutablePositionMatrix(
            );

    const DirectX::XMFLOAT4X4& 
        ScaleMatrix(
            ) const;

    DirectX::XMFLOAT4X4&
        MutableScaleMatrix(
            );
    
    DirectX::XMFLOAT4X4                 
        WorldMatrix(
            )const;




protected:

    
    mutable DirectX::XMFLOAT3           
        m_position;

    mutable DirectX::XMFLOAT3           
        m_scale;

    mutable DirectX::XMFLOAT4           
        m_rotation;

    DirectX::XMFLOAT4X4         
        m_rotationMatrix;

    DirectX::XMFLOAT4X4         
        m_positionMatrix;

    DirectX::XMFLOAT4X4         
        m_scaleMatrix;

    std::string                 
        m_name;
};

#endif