#ifndef _DXMODEL_H
#define _DXMODEL_H

#include <map>
#include "MOWCommon.h"
#include "MOWObject.h"
#include "MOWRenderableObject.h"
#include "MOWModelPart.h"
#include "MOWShaders\MOWShader.h"
#include "MOWMessages\MOWGraphics.pb.h"

class CMOWShader;
class hkpRigidBody;
class hkpRigidBodyCinfo;
class hkxNode;
class hkpPhysicsData;
class CMOWBoundingBox;
class IMOWPhysicalEntity;
class CMOWPhysics;

typedef std::map<std::string,CMOWModelPart*> parts_map;
typedef std::vector<ID3D11ShaderResourceView*> texture_vector;

namespace PbMOWGraphics
{
    class PbMOWModel;
}

class CMOWModel : 
    public CMOWObject,
    public CMOWRenderableObject

{
    typedef CMOWRenderableObject BaseClass;
public:

    struct ModelBoundingSphere
    {

                ModelBoundingSphere()
                {
                    m_xCenter = 0.0f;
                    m_yCenter = 0.0f;
                    m_zCenter = 0.0f;
                    m_xWorldCenter = 0.0f;
                    m_yWorldCenter = 0.0f;
                    m_zWorldCenter = 0.0f;
                    m_radius =  0.0f;
                    m_isInitialized = false;
                }

        void    Initialize(
                    float xCenter,
                    float yCenter,
                    float zCenter,
                    float radius
                    )
        {
            m_xCenter = xCenter;
            m_yCenter = yCenter;
            m_zCenter = yCenter;
            m_radius = radius;
            m_isInitialized = true;
        }

        float   m_xCenter;
        float   m_yCenter;
        float   m_zCenter;
        float   m_xWorldCenter;
        float   m_yWorldCenter;
        float   m_zWorldCenter;
        float   m_radius;
        bool    m_isInitialized;

    };
        CMOWModel();
        CMOWModel(
            CMOWModel& orig
            );
    virtual
        ~CMOWModel();

    bool                                        
        Initialize(
            ID3D11Device* device
            );
    bool                                        
        Render(
            ID3D11DeviceContext* context,
            CMOWShader* shader,
            const DirectX::XMFLOAT4X4& viewMatrix,
            const DirectX::XMFLOAT4X4& projectionMatrix,
            const DirectX::XMFLOAT4X4& lightViewMatrix,
            const DirectX::XMFLOAT4X4& lightProjectionMatrix,
            const DirectX::XMFLOAT3& cameraPosition,
            CMOWShader::LightBufferDefinition& lightBuff,
            int screenWidth,
            int screenHeight,
            std::vector<ID3D11ShaderResourceView*>* resources = 0,
            bool usePixelShader = true
            )const override;

    bool                                        
        Render(
            ID3D11DeviceContext* context,
            CMOWShader* shader,
            const DirectX::XMFLOAT4X4& viewMatrix,
            const DirectX::XMFLOAT4X4& projectionMatrix,
            const DirectX::XMFLOAT4X4& lightViewMatrix,
            const DirectX::XMFLOAT4X4& lightProjectionMatrix,
            const DirectX::XMFLOAT3& cameraPosition,
            ShaderLight& light,
            int screenWidth,
            int screenHeight,
            std::vector<ID3D11ShaderResourceView*>* resources,
            bool usePixelShader = true,
            bool useAdjacentIndices = false
            )const override;

    static CMOWModel*                            
        PopulateModelFromObjFile(
            const char* fileName
            );

    virtual void                                
        Position(
            float xPos,
            float yPos,
            float zPos
            ) override;

    virtual void
        Rotation(
            float xRot,
            float yRot,
            float zRot
            ) override;

    void                                
        Rotation(
            const DirectX::XMFLOAT4& rotation
            );

    virtual void                                
        Scale( 
            float xScale, 
            float yScale, 
            float zScale 
            ) override;

    virtual void                                
        AngularVelocity( 
            float xVel, 
            float yVel, 
            float zVel 
            ) override;

    virtual void                                
        AngularDamping(
            float damping
            )override;

    virtual void                                
        LinearVelocity(
            float xVel,
            float yVel,
            float zVel
            )override;

    CMOWModelPart*                               
        CreateAndAddModelPart(
            const char* name, 
            D3D11_PRIMITIVE_TOPOLOGY topology
            );

    CMOWModelPart*                               
        ModelPart(
            const char* name
            ) const;

    bool                                        
        SerializeSave(
            const char* fileName,
            const CMOWPhysics& physics
            )const;

    static CMOWModel*                            
        SerializeLoad(
            const char* fileName,
            const CMOWPhysics& physics
            );

    void                                        
        SerializeSave(
            std::ofstream& fOut,
            const CMOWPhysics& physics
            )const;

    static CMOWModel*                            
        SerializeLoad(
            std::ifstream& fIn,
            const CMOWPhysics& physics,
            bool useAdjacencies = true
            );


    std::vector<ID3D11ShaderResourceView*>* 
        FaceResources(
            ) const;

    virtual const DirectX::XMFLOAT3&                  
        Position(
            )const override;

    virtual const DirectX::XMFLOAT4&                  
        Rotation(
            )const override;
    

    IMOWPhysicalEntity*                          
        PhysicalBody(
            ) const;

    long                                        
        VertexCount(
            )const;

    long                                        
        PolygonCount(
            )const;

    CMOWBoundingBox*                            
        BoundingBox(
            ) const;

    const ModelBoundingSphere*                  
        BoundingSphere(
            ) const;

    PbMOWGraphics::PbMOWBoundingShape           
        BoundingShape(
            ) const;

    virtual void                                
        Update(
            )override;

    

    void
        ToPb(
            PbMOWGraphics::PbMOWModel& toPb,
            const CMOWPhysics& physics
            )const;

    static CMOWModel*
        FromPb(
            const PbMOWGraphics::PbMOWModel& fromPb,
            const CMOWPhysics& physics
            );

    void
        PropertiesFromPb(
            const PbMOWGraphics::PbMOWModel& fromPb,
            const CMOWPhysics& physics
            );

protected:
                                        
    virtual void                                
        PopulateVerticesAndIndices(
            );

    virtual D3D11_PRIMITIVE_TOPOLOGY            
        Topology(
            )const;


    ID3D11Buffer*                               
        SharedVertexBufferFromPart(
            const char* partName
            );

    ID3D11Buffer*                               
        SharedIndexBufferFromPart(
            const char* partName
            );

    void                                        
        FindAdjacencies(
            CMOWModelPart* part
            );
    
    void                                        
        FindAjacentFaces(
            CMOWFace* face,
            std::set<CMOWFace*>& remainingFaces,
            CMOWModelPart* part,
            std::vector<CMOWFace*>& adjacentFaces
            );

    virtual void                                
        UpdateBoundingVolumes(
            );

    virtual void                                
        CreateBoundingVolumes(
            );

    

    void
        AddModelPart(
            CMOWModelPart* part
            );


    parts_map                                   
        m_parts;

    IMOWPhysicalEntity*                          
        m_physicalBody;

private:

    
    void                                        
        Clear(
            );

    void                                        
        CopyParts(
            parts_map& parts
            );

    ID3D11Buffer*                               
        VertexBufferFromPart(
            const char* partName
            ) const;

    ID3D11Buffer*                               
        IndexBufferFromPart(
            const char* partName
            ) const;
    
    void                                        
        CreateBoundingBox(
            float xCenter,
            float yCenter,
            float zCenter,
            float xLength,
            float yLength,
            float zLength
            );

    long                                        
        FindOppositeIndex(
            long index1, 
            long index2,
            CMOWFace* face
            ) const;

    
    mutable texture_vector                      
        m_textures;

    ModelBoundingSphere                         
        m_boundingSphere;

    PbMOWGraphics::PbMOWBoundingShape           
        m_boundingShape;

    mutable CMOWBoundingBox*                    
        m_boundingBox;

};

#endif