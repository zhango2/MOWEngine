#ifndef _DXRENDERER_H
#define _DXRENDERER_H

#include <d3d11.h>
#include "MOWFrustum.h"
#include "MOWRenderToTexture.h"
#include "MOWCommon/MOWInput.h"
#include <vector>
#include <set>
#include <string>

#include "MOWGraphics\MOWCommon.h"
#include "MOWGraphics\MOW2DObject.h"
#include "MOWGraphics\MOWViewPoint.h"

#include "MOWShaders\MOWTextureShader.h"
#include "MOWShaders\MOWDepthShader.h"
#include "MOWShaders\MOWDeferredShader.h"
#include "MOWShaders\MOWLightShader.h"
#include "MOWShaders\MOWShadowShader.h"

#include "DirectXTK\Inc\SpriteBatch.h"
#include "DirectXTK\Inc\SpriteFont.h"
#include <memory>



class CMOWScene;
class CMOWModel;
class CMOWCamera;
class CMOWShader;
class CMOWRenderToTexture;
class CMOWLight;

class CMOWRendererStatistics
{
public:
    
    CMOWRendererStatistics(
        );

    void
        Clear(
            );

    int                         
        m_fps;

    long                        
        m_vertexCount;

    int                         
        m_modelCount;

    long                        
        m_polygonCount;

    double                      
        m_geoPassTime;

    double                      
        m_lightingPassTime;

    double                      
        m_totalTime;

};

class CMOWRenderer
{
public:

    struct ConstantBuffer
    {
        DirectX::XMMATRIX m_world;
        DirectX::XMMATRIX m_view;
        DirectX::XMMATRIX m_proj;

    };


    typedef std::vector<CMOWScene*>  scene_collection;
    
    CMOWRenderer(
        );

    virtual ~CMOWRenderer(
                );

    bool                            
        Initialize(
            HINSTANCE hInstance,
            HWND hWnd, 
            const RECT& rc
            );

    void                            
        Render(
            );

    void                            
        AddScene(
            CMOWScene* scene
            );

    void                            
        RemoveScene(
            CMOWScene* scene
            );
    
    ID3D11Device*                   
        Device(
            ) const;

    void                            
        DrawText(
            const wchar_t* text,
            float xPos,
            float yPos
            );

    void                            
        ClearBuffers(
            );

    void                            
        Present(
            );

    void                            
        GatherStatistics( 
            bool gather
            );

    const CMOWRendererStatistics*
        Statistics(
            ) const;

    void                            
        RegisterInputSink(
            CMOWInput::IEvents* sink
            );

    void
        AcquireMouse( 
            bool acquire 
            );

    void    
        AcquireKeyboard( 
            bool acquire 
            );
    
private:

    struct LightRenderCBArg
    {
        CMOWRenderer* renderer;
        std::vector<ID3D11ShaderResourceView*>* resources;

    };


    void                            
        Clear(
            );

    void                            
        RenderToTexture(
            CMOWRenderToTexture* renderToTexture,
            CMOWShader* shader,
            std::set<CMOWLight*>& affectingLights
            );

    void                            
        RenderScenes(
            CMOWShader* shader,
            std::set<CMOWLight*>& affectingLights
            );

    void                            
        RenderLights(
            CMOWShader* shader,
            std::vector<ID3D11ShaderResourceView*>* resources
            );

    void                            
        CreateLightStencilPassState(
            );

    void                            
        CreateLightPassState(
            );

    void                            
        CreateShadowStencilPassState(
            );

    static void                     
        RenderLight(
            CMOWLight* light,
            CMOWScene* scene,
            void* arg
            );

    static void                     
        RenderShadow(
            CMOWLight* light,
            CMOWModel* model,
            CMOWScene* scene,
            void* arg
            );

    void                            
        InitializeShaders(
            );

    void                            
        RenderBoundingVolumes(
            );

    void                            
        RenderOctTrees(
            );

    

    ID3D11Device*               
        m_d3dDevice;

    ID3D11DeviceContext*        
        m_immediateContext;

    IDXGISwapChain*             
        m_swapChain;

    ID3D11RenderTargetView*     
        m_renderTargetView;

    scene_collection            
        m_scenes;

    DirectX::XMFLOAT4X4         
        m_projectionMatrix;

	DirectX::XMFLOAT4X4         
        m_worldMatrix;

	DirectX::XMFLOAT4X4         
        m_orthoMatrix;

    ID3D11Texture2D*            
        m_depthStencil;

    ID3D11DepthStencilView*     
        m_depthStencilView;

    ID3D11DepthStencilState*    
        m_depthStencilState;

    ID3D11DepthStencilState*    
        m_lightDepthStencilState;

    ID3D11DepthStencilState*    
        m_shadowDepthStencilState;

    ID3D11DepthStencilState*    
        m_lightState;

    ID3D11RasterizerState*      
        m_rasterState;

    ID3D11RasterizerState*      
        m_lightRasterState;

    ID3D11RasterizerState*      
        m_lightStencilRasterState;

    ID3D11BlendState*           
        m_blendState;

    ID3D11BlendState*           
        m_textBlendState;

    CMOWRenderToTexture          
        m_renderToTexture;

    CMOWRenderToTexture          
        m_renderToCube;

    CMOW2DObject                 
        m_depthWindow;

    CMOW2DObject                 
        m_shadowWindow;

    CMOW2DObject                 
        m_fullScreenWindow;

    CMOWTextureShader*           
        m_textureShader;

    CDXDepthShader*             
        m_depthShader;

    CMOWDeferredShader*          
        m_deferredShader;

    CMOWLightShader*             
        m_lightShader;

    CMOWShadowShader*            
        m_shadowShader;

    bool                        
        m_gatherStatistics;

    CMOWRendererStatistics       
        m_statistics;

    CMOWInput                    
        m_input;

    int                         
        m_screenWidth;

    int                         
        m_screenHeight;

    std::unique_ptr<DirectX::SpriteBatch> 
        m_spriteBatch;

    std::unique_ptr<DirectX::SpriteFont> 
        m_spriteFont;
	

};

class CMOWBeginEndScene
{
public:

        CMOWBeginEndScene(CMOWRenderer* renderer)
        {
            m_renderer = renderer;
            m_renderer->ClearBuffers();
        }
        ~CMOWBeginEndScene()
        {
            m_renderer->Present();
        }
private:

    CMOWRenderer*    
        m_renderer;
};

#endif