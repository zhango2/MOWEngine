#ifndef DXScene_H
#define DXScene_H


#include <set>
#include "MOWCommon.h"
#include "MOWAlignedObject.h"
#include "MOWOctTreeNode.h"
#include "MOWShaders/MOWShader.h"

#include "MOWRenderer/MOWFrustum.h"
#include "MOWMessages/MOWGraphics.pb.h"


class CMOWFrustum;
class CMOWPhysics;

struct ID3D11DeviceContext;
struct ID3D11Device;
struct DirectX::XMMATRIX;
struct DirectX::XMFLOAT3;
class CMOWBoundingBox;

DECLARE_SHARED_PTR(CMOWModel)
DECLARE_SHARED_PTR(CMOWCamera)
DECLARE_SHARED_PTR(CMOWLight)

class CMOWScene :
    public CMOWAlignedObject
{

    typedef std::vector<CMOWCameraPtr> camera_collection;
    typedef std::vector<CMOWLightPtr>  light_collection;
    typedef std::set<CMOWModelPtr> model_collection;
    
public:

    typedef void                (*RenderLightCB)(CMOWLightPtr light, CMOWScene* scene, void* arg);

    typedef void                (*RenderShadowCB)(CMOWLightPtr light,CMOWModelPtr model, CMOWScene* scene, void* arg);

    CMOWScene(
        ID3D11Device* device
        );

    ~CMOWScene(
        );

    void                        
        AddModel(
            CMOWModelPtr model
            );

    void                        
        RemoveModel(
            CMOWModelPtr model
            );

    void                        
        AddLight(
            CMOWLightPtr light,
            bool serializeable = true
            );

    void                        
        RemoveLight(
            CMOWLightPtr light
            );
                                    

    void                        
        AddCamera(
            CMOWCameraPtr camera,
            bool serializeable = true
            );

    void                        
        ActiveCamera(
            CMOWCameraPtr camera
            );

    CMOWCameraPtr                  
        ActiveCamera(
            ) const;
    

    bool                        
        Render(
            ID3D11DeviceContext* context,
            CMOWShader* shader,
            std::set<CMOWLightPtr>& affectingLights,
            int screenWidth,
            int screenHeight
            );
                    
    void                        
        RenderLights(
            RenderLightCB renderCB,
            void* renderArg
            );

    void                        
        RenderShadows(
            RenderShadowCB shadowCB,
            CMOWLightPtr light,
            void* shadowArg
            );

    void                        
        RenderBoundingVolumes(
            ID3D11DeviceContext* context,
            int screenWidth,
            int screenHeight
            );

    void                        
        RenderOctTree(
            ID3D11DeviceContext* context,
            int screenWidth,
            int screenHeight
            );

    bool                        
        IsModelInFrustum(
            const CMOWModelPtr model
            ) const;
    
    void                        
        GatherStatistics( 
            bool gather = true 
            );

    int                         
        NumOfRenderedModels(
            ) const;

    long                        
        NumOfRenderedPolygons(
            ) const;

    long                        
        NumOfRenderedVertices(
            ) const;

    bool                        
        SerializeSave( 
            const char* fileName,
            const CMOWPhysics& physics
            ) const;

    static CMOWScene*            
        SerializeLoad(
            const char* fileName,
            ID3D11Device* device, 
            CMOWPhysics& physicsWorld
            );


    std::set<CMOWModelPtr>
        FixedModels(
            );

    const std::set<CMOWModelPtr>&     
        DynamicModels(
            );

    const std::vector<CMOWLightPtr>&
        Lights(
            );

    void                        
        CreateOctTreeFromFixedObjects(
            );

    void
        ToPb(
            PbMOWGraphics::PbMOWScene& toPb,
            const CMOWPhysics& physics
            )const;

    static CMOWScene*
        FromPb(
            const PbMOWGraphics::PbMOWScene& scene,
            CMOWPhysics& physics,
            ID3D11Device* device
            );
                
    

private:

    

    
    void                        
        CreateOctTree(
            CMOWBounds& bounds
            );

    
    void                        
        RenderModelsFromNodesInFrustum(
            CMOWOctTreeNode* node,
            ID3D11DeviceContext* context,
            CMOWShader* shader,
            CMOWShader::LightBufferDefinition& lightBuff,
            DirectX::XMFLOAT4X4& lightViewMatrix,
            DirectX::XMFLOAT4X4& lightProjMatrix,
            int screenWidth,
            int screenHeight
            );

    void                        
        RenderBoundingVolumes(
            const model_collection& models,
            ID3D11DeviceContext* context,
            int screenWidth,
            int screenHeight
            );

    bool                        
        Serialize(
            std::ofstream& fOut,
            const CMOWPhysics& physics
            ) const;
    

    void
        ClearModels( 
            bool deleteModels = true
            );

    void
        ClearModelCollectionAndDeleteModels(
            model_collection& models
            );

    void
        ExtractModelsFromOctTreeNode( 
            std::set<CMOWModelPtr>& models,
            CMOWOctTreeNode* node 
            );

    
    model_collection            
        m_fixedModels;

    model_collection            
        m_models;

    camera_collection           
        m_cameras;

    camera_collection           
        m_serializableCameras;

    light_collection            
        m_lights;

    light_collection            
        m_serializableLights;

    ID3D11Device*               
        m_d3dDevice;

    CMOWCameraPtr                  
        m_activeCamera;

    CMOWFrustum                  
        m_frustum;

    bool                        
        m_gatherStatistics;

    model_collection            
        m_renderedModels;

    CMOWOctTreeNode*             
        m_octTreeRoot;

};




#endif