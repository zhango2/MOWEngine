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

DECLARE_SHARED_PTR(CMOWModelPart)
DECLARE_SHARED_PTR(CMOWModel)

typedef std::map<std::string,CMOWModelPartPtr> parts_map;
typedef std::vector<ID3D11ShaderResourceView*> texture_vector;

namespace PbMOWGraphics
{
    class PbMOWModel;
}

class CMOWModel : 
    public CMOWObject,
    public CMOWRenderableObject,
    public std::enable_shared_from_this<CMOWModel>

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
            const DirectX::XMVECTOR& cameraPosition,
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
            const DirectX::XMVECTOR& cameraPosition,
            ShaderLight& light,
            int screenWidth,
            int screenHeight,
            std::vector<ID3D11ShaderResourceView*>* resources,
            bool usePixelShader = true,
            bool useAdjacentIndices = false
            )const override;

    static CMOWModelPtr                            
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
            const DirectX::XMVECTOR& rotation
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

    CMOWModelPartPtr                               
        CreateAndAddModelPart(
            const char* name, 
            D3D11_PRIMITIVE_TOPOLOGY topology
            );

    CMOWModelPartPtr                               
        ModelPart(
            const char* name
            ) const;

    bool                                        
        SerializeSave(
            const char* fileName,
            const CMOWPhysics& physics
            )const;

    static CMOWModelPtr                            
        SerializeLoad(
            const char* fileName,
            const CMOWPhysics& physics
            );

    void                                        
        SerializeSave(
            std::ofstream& fOut,
            const CMOWPhysics& physics
            )const;

    static CMOWModelPtr                            
        SerializeLoad(
            std::ifstream& fIn,
            const CMOWPhysics& physics,
            bool useAdjacencies = true
            );


    std::vector<ID3D11ShaderResourceView*>* 
        FaceResources(
            ) const;

    virtual const DirectX::XMVECTOR&                  
        Position(
            )const override;

    virtual const DirectX::XMVECTOR&                  
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

    static CMOWModelPtr
        FromPb(
            const PbMOWGraphics::PbMOWModel& fromPb,
            const CMOWPhysics& physics
            );

    void
        PropertiesFromPb(
            const PbMOWGraphics::PbMOWModel& fromPb,
            const CMOWPhysics& physics
            );

    const parts_map&
        Parts(
            )const;

    void
        CreatePhysicalBody(
            CMOWPhysics& physics,
            bool fixed,
            bool collidable
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
            CMOWModelPartPtr part
            );
    
    void                                        
        FindAjacentFaces(
            CMOWFacePtr face,
            std::set<CMOWFacePtr>& remainingFaces,
            CMOWModelPartPtr part,
            std::vector<CMOWFacePtr>& adjacentFaces
            );

    virtual void                                
        UpdateBoundingVolumes(
            );

    virtual void                                
        CreateBoundingVolumes(
            );

    

    

    void
        AddModelPart(
            CMOWModelPartPtr part
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
            CMOWFacePtr face
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