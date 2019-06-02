#ifndef _CDXViewPoint_H
#define _CDXViewPoint_H

#include "MOWCommon.h"
#include "MOWShaders\MOWShader.h"

class CMOWRenderableObject;
class CMOWObject;

DECLARE_SHARED_PTR(CMOWModel)

class CMOWViewPoint
{



public:
    CMOWViewPoint(
        float fieldOfView,
        float aspectRatio,
        float nearPlane,
        float farPlane
        );

    virtual             
        ~CMOWViewPoint(
            );
    
    virtual void                    
        UpdateViewPoint(
            );
    
    const DirectX::XMFLOAT4X4&            
        GetViewMatrix(
            ) const;

    const DirectX::XMFLOAT4X4&            
        GetProjectionMatrix(
            ) const;

    DirectX::XMFLOAT4X4&
        MutableViewMatrix(
            );

    DirectX::XMFLOAT4X4&
        MutableProjectionMatrix(
            );

    void                            
        SetProjectionParameters(
            float fieldOfView,
            float aspectRatio,
            float nearPlane,
            float farPlane
            );

    void                            
        LookAt(
            float x, 
            float y, 
            float z
            );

    DirectX::XMVECTOR               
        LookAt(
            );

    virtual CMOWRenderableObject*    
        RenderableObject(
            );

    virtual CMOWModelPtrC
        Model(
            ) const;

    virtual CMOWModelPtr               
        MutableModel(
            );

protected:

    virtual const CMOWObject*              
        GeometryObject(
            ) const = 0;

    virtual CMOWObject*
        MutableGeometryObject(
            ) = 0;

    DirectX::XMFLOAT4X4         
        m_viewMatrix;

    DirectX::XMFLOAT4X4         
        m_projectionMatrix;

    DirectX::XMVECTOR           
        m_lookAt;

    float                       
        m_fieldOfView;

    float                       
        m_aspectRatio;

    float                       
        m_nearPlane;

    float                       
        m_farPlane;

};


#endif