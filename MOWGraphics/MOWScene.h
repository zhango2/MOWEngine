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
class CMOWModel;
class CMOWCamera;
class CMOWLight;
class CMOWPhysics;


struct ID3D11DeviceContext;
struct ID3D11Device;
struct DirectX::XMMATRIX;
struct DirectX::XMFLOAT3;
class CMOWBoundingBox;

class CMOWScene :
    public CMOWAlignedObject
{

    typedef std::vector<CMOWCamera*> camera_collection;
    typedef std::vector<CMOWLight*>  light_collection;
    typedef std::set<CMOWModel*> model_collection;
    
public:

    typedef void                (*RenderLightCB)(CMOWLight* light, CMOWScene* scene, void* arg);

    typedef void                (*RenderShadowCB)(CMOWLight* light,CMOWModel* model, CMOWScene* scene, void* arg);

    CMOWScene(
        ID3D11Device* device
        );

    ~CMOWScene(
        );

    void                        
        AddModel(
            CMOWModel* model
            );

    void                        
        RemoveModel(
            CMOWModel* model
            );

    void                        
        AddLight(
            CMOWLight* light,
            bool serializeable = true
            );

    void                        
        RemoveLight(
            CMOWLight* light
            );
                                    

    void                        
        AddCamera(
            CMOWCamera* camera,
            bool serializeable = true
            );

    void                        
        ActiveCamera(
            CMOWCamera* camera
            );

    CMOWCamera*                  
        ActiveCamera(
            ) const;
    

    bool                        
        Render(
            ID3D11DeviceContext* context,
            CMOWShader* shader,
            DirectX::XMFLOAT4X4& projectionMatrix,
            std::set<CMOWLight*>& affectingLights,
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
            CMOWLight* light,
            void* shadowArg
            );

    void                        
        RenderBoundingVolumes(
            ID3D11DeviceContext* context,
            DirectX::XMFLOAT4X4& projectionMatrix,
            int screenWidth,
            int screenHeight
            );

    void                        
        RenderOctTree(
            ID3D11DeviceContext* context,
            DirectX::XMFLOAT4X4& projectionMatrix,
            int screenWidth,
            int screenHeight
            );

    bool                        
        IsModelInFrustum(
            const CMOWModel* model
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


    std::set<CMOWModel*>
        FixedModels(
            );

    const std::set<CMOWModel*>&     
        DynamicModels(
            );

    const std::vector<CMOWLight*>&
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
            DirectX::XMFLOAT4X4& projectionMatrix,
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
            DirectX::XMFLOAT4X4& projectionMatrix,
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
            std::set<CMOWModel*>& models,
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

    CMOWCamera*                  
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