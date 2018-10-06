#include "MOWScene.h"
#include "MOWModel.h"
#include "MOWCamera.h"
#include "MOWLight.h"
#include "MOWRenderer/MOWFrustum.h"
#include "D3D11.h"
#include "MOWPointLight.h"
#include "MOWPhysics/MOWPhysics.h"
#include "MOWResourceManager.h"
#include "MOWMaterial.h"
#include "MOWBoundingBox.h"
#include "MOWFace.h"
#include "MOWCommon/MOWSerializer.h"
#include <iostream>
#include <fstream>
#include "MOWLightCreator.h"
#include "MOWPhysics/IMOWPhysicalEntity.h"

using namespace DirectX;

static int s_iterations = 0;

#define LIGHTS_ID "Lights"
//---------------------------------------------
CMOWScene::CMOWScene(
    ID3D11Device* device
    )
{
    m_d3dDevice = device;
    m_activeCamera = 0;
    m_gatherStatistics = false;
    m_octTreeRoot = 0;
}
//---------------------------------------------
CMOWScene::~CMOWScene()
{

}
//---------------------------------------------
void CMOWScene::AddModel( 
    CMOWModel* model 
    )
{
    model->Initialize(m_d3dDevice);
    if( model->PhysicalBody() )
    {
        if( model->PhysicalBody()->MotionType() == IMOWPhysicalEntity::PEMT_FIXED )
        {
            m_fixedModels.insert(model);
        }
        else
        {
            m_models.insert(model);
        }
    }
    else
    {
        m_models.insert(model);
    }
    
}
//---------------------------------------------
bool CMOWScene::Render( 
    ID3D11DeviceContext* context, 
    CMOWShader* shader, 
    XMFLOAT4X4& projectionMatrix,
    std::set<CMOWLight*>& affectingLights,
    int screenWidth,
    int screenHeight
    )
{

    if( m_gatherStatistics )
    {
        m_renderedModels.clear();
    }

    ActiveCamera()->UpdateViewPoint();
    m_frustum.Update(SCREEN_DEPTH,ActiveCamera()->MutableProjectionMatrix(),ActiveCamera()->MutableViewMatrix());

    CMOWShader::LightBufferDefinition lightBuff;
    XMFLOAT4X4 lightViewMatrix;
    XMFLOAT4X4 lightProjMatrix;

    //Render the fixed models contained in the oct tree
    RenderModelsFromNodesInFrustum(m_octTreeRoot,
                                   context,
                                   shader,
                                   projectionMatrix,
                                   lightBuff,
                                   lightViewMatrix,
                                   lightProjMatrix,
                                   screenWidth,
                                   screenHeight
                                   );


    /*model->Render(context,
        shader,
        ActiveCamera()->GetViewMatrix(),
        projectionMatrix,
        lightViewMatrix,
        lightProjMatrix,
        ActiveCamera()->GetPosition(),
        ShaderLight(),
        screenWidth,
        screenHeight,
        0
    );*/


    model_collection::iterator itModel = m_models.begin();
    
    while( itModel != m_models.end() )
    {
        CMOWModel* model = (*itModel);
        model->Update();
        
        if( IsModelInFrustum(model) )
        {

            model->Render(context,
                            shader,
                            ActiveCamera()->GetViewMatrix(),
                            projectionMatrix,
                            lightViewMatrix,
                            lightProjMatrix,
                            ActiveCamera()->Position(),
                            ShaderLight(),
                            screenWidth,
                            screenHeight,
                            0
                            );

            if( m_gatherStatistics )
            {
                m_renderedModels.insert(model);
            }
        }
            
        itModel++;
        
    }
    
    return true;
}
//---------------------------------------------
void CMOWScene::AddLight( 
    CMOWLight* light, 
    bool serializable
    )
{
    light->Initialize(m_d3dDevice);
    m_lights.push_back(light);
    if( serializable )
    {
        m_serializableLights.push_back(light);
    }
}
//------------------------------------------------------
void CMOWScene::RemoveLight(
    CMOWLight* light
    )
{
    light_collection::iterator itLight = m_lights.begin();

    while( itLight != m_lights.end() )
    {
        if( *itLight == light )
        {
            m_lights.erase(itLight);
            break;
        }
    }

}
//---------------------------------------------
void CMOWScene::AddCamera( 
    CMOWCamera* camera,
    bool serializeable
    )
{
    m_cameras.push_back(camera);
    if( serializeable )
    {
        m_serializableCameras.push_back(camera);
    }
    if( !m_activeCamera )
    {
        m_activeCamera = camera;
    }
}
//---------------------------------------------
void CMOWScene::ActiveCamera( 
    CMOWCamera* camera 
    )
{
    m_activeCamera = camera;
}
//---------------------------------------------
CMOWCamera* CMOWScene::ActiveCamera() const
{
    return m_activeCamera;
}
//---------------------------------------------
void CMOWScene::RemoveModel( 
    CMOWModel* model 
    )
{
    model_collection::iterator itModel = m_models.find(model);
    if( itModel != m_models.end() )
    {
        m_models.erase(itModel);
    }
}
//---------------------------------------------
void CMOWScene::GatherStatistics( 
    bool gather 
    )
{
    m_gatherStatistics = gather;
}
//---------------------------------------------
int CMOWScene::NumOfRenderedModels() const
{
    return m_renderedModels.size();
}
//---------------------------------------------
long CMOWScene::NumOfRenderedPolygons() const
{
    long numOfPolys = 0;
    model_collection::iterator itModel = m_renderedModels.begin();

    while( itModel != m_renderedModels.end() )
    {
        numOfPolys += (*itModel)->PolygonCount();
        itModel++;
    }
    return numOfPolys;
}
//---------------------------------------------
long CMOWScene::NumOfRenderedVertices() const
{
    long numOfVertices = 0;
    model_collection::iterator itModel = m_renderedModels.begin();

    while( itModel != m_renderedModels.end() )
    {
        numOfVertices += (*itModel)->VertexCount();
        itModel++;
    }
    return numOfVertices;
}
//---------------------------------------------
void CMOWScene::RenderLights( 
    RenderLightCB renderCB, 
    void* renderArg 
    )
{
    light_collection::iterator itLight = m_lights.begin();
    while( itLight != m_lights.end() )
    {
        CMOWLight* light = (*itLight);
        light->UpdateViewPoint();

        if( 1/*IsModelInFrustum(light->Model())*/ )
        {
            renderCB(light,this,renderArg);
            if( m_gatherStatistics )
            {
                m_renderedModels.insert(light->MutableModel());
            }
        }
        
        itLight++;
    }
}
//------------------------------------------------------
void CMOWScene::RenderShadows(
    RenderShadowCB shadowCB,
    CMOWLight* light,
    void* shadowArg
    )
{

    
        
    model_collection::iterator itModel = m_models.begin();

    while( itModel != m_models.end() )
    {
        shadowCB(light,*itModel,this,shadowArg);
        itModel++;
    }

    
    /*itModel = m_octTreeRoot->Models().begin();
    while( itModel != m_octTreeRoot->Models().end() )
    {

        shadowCB(light,*itModel,this,shadowArg);
        itModel++;
    }*/
    
}
//---------------------------------------------
bool CMOWScene::IsModelInFrustum( 
    const CMOWModel* model 
    )const
{
    bool isInFrustum = true;

    if( model->BoundingShape() == PbMOWGraphics::PMBS_SPHERE )
    {
        isInFrustum = m_frustum.IsSphereInFrustum(model->BoundingSphere()->m_xWorldCenter,
            model->BoundingSphere()->m_yWorldCenter,
            model->BoundingSphere()->m_zWorldCenter,
            model->BoundingSphere()->m_radius);
    }
    else if( model->BoundingShape() == PbMOWGraphics::PMBS_BOX )
    {
        isInFrustum = m_frustum.IsBoxInFrustum(XMVectorGetX(model->BoundingBox()->Position()),
            XMVectorGetY(model->BoundingBox()->Position()),
            XMVectorGetZ(model->BoundingBox()->Position()),
            model->BoundingBox()->Width(),
            model->BoundingBox()->Height(),
            model->BoundingBox()->Depth());
    }

    return isInFrustum;
}
//---------------------------------------------
void CMOWScene::CreateOctTreeFromFixedObjects()
{
    model_collection::iterator fixedObjIterator = m_fixedModels.begin();
    

    XMFLOAT3 max;
    XMFLOAT3 min;

    max.x = -999999.0f;
    max.y = -999999.0f;
    max.z = -999999.0f;
    min.x = 999999.0f;
    min.y = 999999.0f;
    min.z = 999999.0f;

    

    
    while (fixedObjIterator != m_fixedModels.end())
    {
        CMOWModel* model = (*fixedObjIterator);
        CMOWBoundingBox* box = model->BoundingBox();

        float x = XMVectorGetX(box->Position());
        float y = XMVectorGetY(box->Position());
        float z = XMVectorGetZ(box->Position());

        max.x = x >= max.x ? x + box->Width() / 2.0f : max.x;
        min.x = x <= min.x ? x - box->Width() / 2.0f : min.x;

        max.y = y >= max.y ? y + box->Height() / 2.0f : max.y;
        min.y = y <= min.y ? y - box->Height() / 2.0f : min.y;

        max.z = z >= max.z ? z + box->Depth() / 2.0f : max.z;
        min.z = z <= min.z ? z - box->Depth() / 2.0f : min.z;
        
        fixedObjIterator++;
    }
    
    
    XMVECTOR maxVec = XMVectorSet(max.x,max.y,max.z,1.0f);
    XMVECTOR minVec = XMVectorSet(min.x,min.y,min.z,1.0f);
    XMVECTOR cubeSize = XMVectorSubtract(maxVec,minVec);
    

    CMOWBounds bounds;

    XMVECTOR temp = minVec + cubeSize * 0.5f;

    XMStoreFloat3(&bounds.m_center,temp);

    bounds.m_size = fabs(XMVectorGetX(cubeSize));

    if( bounds.m_size < fabs(XMVectorGetY(cubeSize)) )
    {
        bounds.m_size = fabs(XMVectorGetY(cubeSize));
    }
    if( bounds.m_size < fabs(XMVectorGetZ(cubeSize) ))
    {
        bounds.m_size = fabs(XMVectorGetZ(cubeSize));
    }

    CreateOctTree(bounds);
}
//------------------------------------------------------
void CMOWScene::ToPb(
    PbMOWGraphics::PbMOWScene& toPb,
    const CMOWPhysics& physics
    ) const
{

    for( const CMOWModel* model : m_octTreeRoot->Models() )
    {
        model->ToPb(*(toPb.add_fixedmodels()),physics);
    }
    for(const CMOWModel* model : m_models)
    {
        model->ToPb(*(toPb.add_dynamicmodels()),physics);
    }
    for( const CMOWCamera* camera : m_serializableCameras )
    {
        camera->ToPb(*(toPb.add_cameras()),physics);
    }
    for(const CMOWLight* light : m_serializableLights)
    {
        light->ToPb(*(toPb.add_lights()), physics);
    }
}
//------------------------------------------------------
CMOWScene* CMOWScene::FromPb(
    const PbMOWGraphics::PbMOWScene& scene, 
    CMOWPhysics& physics, 
    ID3D11Device* device
    )
{
    CMOWScene* newScene = new CMOWScene(device);

    for( const PbMOWGraphics::PbMOWModel& model : scene.fixedmodels() )
    {
        CMOWModel* newModel = CMOWModel::FromPb(model,physics);
        newScene->AddModel(newModel);
        physics.AddModel(newModel);
    }
    for(const PbMOWGraphics::PbMOWModel& model : scene.dynamicmodels())
    {
        CMOWModel* newModel = CMOWModel::FromPb(model, physics);
        newScene->AddModel(newModel);
        physics.AddModel(newModel);
    }
    for(const PbMOWGraphics::PbMOWModel& camera : scene.cameras())
    {
        CMOWCamera* newCamera = CMOWCamera::FromPb(camera, physics);
        newScene->AddCamera(newCamera);
        physics.AddModel(newCamera);
    }
    for(const PbMOWGraphics::PbMOWLight& light : scene.lights())
    {
        CMOWLight* newLight = CMOWLightCreator::CreateLight(light,physics);
        newScene->AddLight(newLight,true);
        if( newLight->Model() )
        {
            physics.AddModel(newLight->MutableModel());
        }
        
    }
    newScene->CreateOctTreeFromFixedObjects();

    //TODO: Lights and cameras!

    return newScene;
}
//---------------------------------------------
void CMOWScene::CreateOctTree(
    CMOWBounds& bounds
    )
{
    m_octTreeRoot = new CMOWOctTreeNode;
    m_octTreeRoot->Initialize(bounds,m_fixedModels,m_d3dDevice);
}
//---------------------------------------------
void CMOWScene::RenderModelsFromNodesInFrustum( 
    CMOWOctTreeNode* node,
    ID3D11DeviceContext* context,
    CMOWShader* shader,
    DirectX::XMFLOAT4X4& projectionMatrix,
    CMOWShader::LightBufferDefinition& lightBuff,
    XMFLOAT4X4& lightViewMatrix,
    XMFLOAT4X4& lightProjMatrix,
    int screenWidth,
    int screenHeight
    )
{

    if( node )
    {
        s_iterations++;
        if( m_frustum.IsBoxInFrustum(XMVectorGetX(node->BoundingBox()->Position()),
            XMVectorGetY(node->BoundingBox()->Position()),
            XMVectorGetZ(node->BoundingBox()->Position()),
            node->BoundingBox()->Width(),
            node->BoundingBox()->Height(),
            node->BoundingBox()->Depth()) )
        {
            if( node->Children().size() == 0 )
            {
                auto itModel = node->Models().begin();
                while( itModel != node->Models().end() )
                {

                    CMOWModel* model = (*itModel);
                    model->Update();
                    if( IsModelInFrustum(model) )
                    {
                        model->Render(context,
                            shader,
                            ActiveCamera()->GetViewMatrix(),
                            projectionMatrix,
                            lightViewMatrix,
                            lightProjMatrix,
                            ActiveCamera()->Position(),
                            ShaderLight(),
                            screenWidth,
                            screenHeight,
                            0
                            );

                        if( m_gatherStatistics )
                        {
                            m_renderedModels.insert(model);
                        }
                    }
                    itModel++;
                }

            }
            else
            {
                for( size_t i=0; i<node->Children().size(); i++ )
                {
                    RenderModelsFromNodesInFrustum(node->Children()[i],
                        context,
                        shader,
                        projectionMatrix,
                        lightBuff,
                        lightViewMatrix,
                        lightProjMatrix,
                        screenWidth,
                        screenHeight);
                }
            }
        }
    }
    
}
//---------------------------------------
void CMOWScene::RenderBoundingVolumes(
    ID3D11DeviceContext* context,
    DirectX::XMFLOAT4X4& projectionMatrix,
    int screenWidth,
    int screenHeight
    )
{
    if( m_octTreeRoot )
    {
        RenderBoundingVolumes(m_octTreeRoot->Models(),context,projectionMatrix,screenWidth,screenHeight);
    }
    RenderBoundingVolumes(m_models,context,projectionMatrix,screenWidth,screenHeight);
}
//---------------------------------------
void CMOWScene::RenderBoundingVolumes(
    const model_collection& models,
    ID3D11DeviceContext* context,
    DirectX::XMFLOAT4X4& projectionMatrix,
    int screenWidth,
    int screenHeight
    )
{
    ActiveCamera()->UpdateViewPoint();
    auto itModel = models.begin();

    while( itModel != models.end() )
    {
        
        CMOWModel* model = (*itModel);

        model->BoundingBox()->Render(context,
                                        ActiveCamera()->GetViewMatrix(),
                                        projectionMatrix,
                                        ActiveCamera()->Position(),
                                        screenWidth,
                                        screenHeight
                                        );
        itModel++;
    }
}
//---------------------------------------
void CMOWScene::RenderOctTree(
    ID3D11DeviceContext* context, 
    DirectX::XMFLOAT4X4& projectionMatrix, 
    int screenWidth, 
    int screenHeight
    )
{
    ActiveCamera()->UpdateViewPoint();
    m_octTreeRoot->Render(context,
                          ActiveCamera()->GetViewMatrix(),
                          projectionMatrix,
                          ActiveCamera()->Position(),
                          screenWidth,
                          screenHeight
                          );
}
//---------------------------------------
bool CMOWScene::SerializeSave( 
    const char* fileName,
    const CMOWPhysics& physics
    ) const
{
    

    std::ofstream fOut;
    fOut.open(fileName,std::ios_base::binary);
    Serialize(fOut,physics);
    fOut.flush();
    fOut.close();
    
    return true;
}
//---------------------------------------
bool CMOWScene::Serialize(
    std::ofstream& fOut,
    const CMOWPhysics& physics
    ) const
{
    PbMOWGraphics::PbMOWScene scene;
    ToPb(scene, physics);
    scene.SerializeToOstream(& fOut);

    return true;
}
//---------------------------------------
CMOWScene* CMOWScene::SerializeLoad(
    const char* fileName, 
    ID3D11Device* device, 
    CMOWPhysics& physicsWorld
    )
{
    

    std::ifstream fIn;
    fIn.open(fileName,std::ios_base::binary);

    PbMOWGraphics::PbMOWScene scene;
    scene.ParseFromIstream(&fIn);

    fIn.close();

    return FromPb(scene,physicsWorld,device);
}
//------------------------------------------------------
void CMOWScene::ClearModels( 
    bool deleteModels 
    )
{
    if(deleteModels)
    {
        ClearModelCollectionAndDeleteModels(m_fixedModels);
        ClearModelCollectionAndDeleteModels(m_models);
        delete m_octTreeRoot;
        m_octTreeRoot = 0;
    }
    else
    {
        m_fixedModels.clear();
        m_models.clear();
        delete m_octTreeRoot;
        m_octTreeRoot = 0;
    }

}
//------------------------------------------------------
void CMOWScene::ClearModelCollectionAndDeleteModels(
    model_collection& models
    )
{
    auto itModel = models.begin();

    while(itModel != models.end())
    {
        CMOWModel* model = *itModel;
        delete model;
        itModel = models.erase(itModel);
    }
}
//------------------------------------------------------
std::set<CMOWModel*> CMOWScene::FixedModels()
{
    std::set<CMOWModel*> models;

    ExtractModelsFromOctTreeNode(models,m_octTreeRoot);

    return models;
    

}
//------------------------------------------------------
const std::set<CMOWModel*>& CMOWScene::DynamicModels()
{
    return m_models;
}
//------------------------------------------------------
void CMOWScene::ExtractModelsFromOctTreeNode(
    std::set<CMOWModel*>& models,
    CMOWOctTreeNode* node
    )
{
    for(CMOWModel* model : node->Models() )
    {
        models.insert(model);
    }
}
//------------------------------------------------------
const std::vector<CMOWLight*>& CMOWScene::Lights()
{
    return m_lights;
}

