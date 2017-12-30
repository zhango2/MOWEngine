#ifndef DXBoundingBox_H
#define DXBoundingBox_H

#include "MOWBox.h"

class CMOWModelPart;

class CMOWBoundingBox : public CMOWBox
{
public:

    ~CMOWBoundingBox(
        );

    static CMOWBoundingBox*          
        Create( 
            float width, 
            float height, 
            float depth 
            );

    const CMOWModelPart*             
        ModelPart(
            );

    bool                            
        IsInside(
            const CMOWBoundingBox& box
            ) const;

    void                            
        Render( 
            ID3D11DeviceContext* context, 
            const DirectX::XMFLOAT4X4& viewMatrix, 
            const DirectX::XMFLOAT4X4& projectionMatrix,
            const DirectX::XMFLOAT3& cameraPosition,
            int screenWidth, 
            int screenHeight
            ) const;

    virtual bool                    
        Initialize(
            ID3D11Device* device
            );

    virtual void                                
        Scale( 
            float xScale, 
            float yScale, 
            float zScale 
            ) override;

    static CMOWBoundingBox*
        FromPb( 
            const PbMOWGraphics::PbMOWBoundingBox& fromPb 
            );

    void
        ToPb(
            PbMOWGraphics::PbMOWBoundingBox& toPb 
            )const;


protected:

    virtual void                    
        UpdateBoundingVolumes(
            );

private:

    CMOWBoundingBox(
        );

};

#endif