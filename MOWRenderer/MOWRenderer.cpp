#include "MOWRenderer.h"

//DXGraphics
#include "MOWGraphics\MOWModel.h"
#include "MOWGraphics\MOWCamera.h"
#include "MOWGraphics\MOWLight.h"
#include "MOWGraphics\MOWPointLight.h"
#include "MOWGraphics\MOWRenderableObject.h"
#include "MOWGraphics\MOWScene.h"
#include "MOWGraphics\MOWCommon.h"
#include "MOWGraphics\MOWResourceManager.h"

//MOWShaders
#include "MOWShaders/MOWColorShader.h"
#include "MOWShaders/MOWTextureShader.h"

#include "MOWCommon/MOWTimer.h"

#include "Effects11/inc/d3dx11effect.h"

using namespace DirectX;

static int s_frameCount = 0;
static DWORD s_dwTimeStart = 0;
static float s_seconds = 0.0f;
CMOWRenderer::CMOWRenderer()
{
    m_d3dDevice = 0;
    m_immediateContext = 0;
    m_renderTargetView = 0;
    m_swapChain = 0;
    m_depthStencil = 0;
    m_depthStencilView = 0;
    m_depthStencilState = 0;
    m_rasterState = 0;
    m_screenWidth = 0;
    m_screenHeight = 0;
    m_textureShader = 0;
    m_lightShader = 0;
    m_depthShader = 0;
    m_deferredShader = 0;
    m_shadowShader = 0;
    m_lightDepthStencilState = 0;
}

CMOWRenderer::~CMOWRenderer()
{
    Clear();
}

bool CMOWRenderer::Initialize(
    HINSTANCE hInstance,
    HWND hWnd, 
    const RECT& rc
    )
{
    bool retVal = false;

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    m_screenWidth = rc.right - rc.left;
    m_screenHeight = rc.bottom - rc.top;
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof(sd) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = m_screenWidth;
    sd.BufferDesc.Height = m_screenHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    if( !FAILED( D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0/*D3D11_CREATE_DEVICE_DEBUG*/, featureLevels, numFeatureLevels,
                     D3D11_SDK_VERSION, &sd, &m_swapChain, &m_d3dDevice, NULL, &m_immediateContext ) ) )
    {
        
        m_spriteBatch = std::unique_ptr<DirectX::SpriteBatch>(new DirectX::SpriteBatch(m_immediateContext));
        m_spriteFont = std::unique_ptr<DirectX::SpriteFont>(new DirectX::SpriteFont(m_d3dDevice, L"d:\\develop\\dx\\DXTest\\jens_test.spritefont"));
        

        D3D11_BLEND_DESC BlendState;
        ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
        BlendState.RenderTarget[0].BlendEnable = TRUE;
        BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;//D3D11_BLEND_SRC_COLOR;
        BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
        BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        
        HRESULT hr = m_d3dDevice->CreateBlendState(&BlendState, &m_blendState);

        BlendState.RenderTarget[0].BlendEnable = TRUE;
        BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;
        BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;//D3D11_BLEND_SRC_COLOR;
        BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
        BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;

        hr = m_d3dDevice->CreateBlendState(&BlendState, &m_textBlendState);

        // Create a render target view
        ID3D11Texture2D* pBackBuffer = NULL;
        hr = m_swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
        if( !FAILED( hr ) )
        {
            hr = m_d3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_renderTargetView );
            pBackBuffer->Release();
            if( !FAILED( hr ) )
            {

                // Create depth stencil texture
                D3D11_TEXTURE2D_DESC descDepth;
	            ZeroMemory( &descDepth, sizeof(descDepth) );
                descDepth.Width = m_screenWidth;
                descDepth.Height = m_screenHeight;
                descDepth.MipLevels = 1;
                descDepth.ArraySize = 1;
                descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
                descDepth.SampleDesc.Count = 1;
                descDepth.SampleDesc.Quality = 0;
                descDepth.Usage = D3D11_USAGE_DEFAULT;
                descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
                descDepth.CPUAccessFlags = 0;
                descDepth.MiscFlags = 0;
                if( !FAILED(m_d3dDevice->CreateTexture2D( &descDepth, NULL, &m_depthStencil )) )
                {

                    // Initialize the description of the stencil state.
                    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	                ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	                // Set up the description of the stencil state.
	                depthStencilDesc.DepthEnable = true;
	                depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	                depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	                depthStencilDesc.StencilEnable = true;
	                depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	                depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	                // Stencil operations if pixel is front-facing.
	                depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	                depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	                depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	                depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	                // Stencil operations if pixel is back-facing.
	                depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	                depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	                depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	                depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
                
	                // Create the depth stencil state.
	                if( !FAILED(m_d3dDevice->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState)) )
                    {
                        
                        m_immediateContext->OMSetDepthStencilState(m_depthStencilState, 1);

                        CreateLightStencilPassState();
                        CreateLightPassState();

                        CreateShadowStencilPassState();

                        // Setup the raster description which will determine how and what polygons will be drawn.
                        D3D11_RASTERIZER_DESC rasterDesc;
	                    rasterDesc.AntialiasedLineEnable = false;
                        rasterDesc.CullMode = D3D11_CULL_BACK;
	                    rasterDesc.DepthBias = 0;
	                    rasterDesc.DepthBiasClamp = 0.0f;
	                    rasterDesc.DepthClipEnable = true;
                        rasterDesc.FillMode = D3D11_FILL_SOLID;
	                    rasterDesc.FrontCounterClockwise = false;
	                    rasterDesc.MultisampleEnable = false;
	                    rasterDesc.ScissorEnable = false;
	                    rasterDesc.SlopeScaledDepthBias = 0.0f;

	                    // Create the rasterizer state from the description we just filled out.
	                    
	                    if(!FAILED(m_d3dDevice->CreateRasterizerState(&rasterDesc, &m_rasterState)))
	                    {
		                    // Now set the rasterizer state.
	                        m_immediateContext->RSSetState(m_rasterState);

                            //create another one for the lights

                            
                            // Create the depth stencil view
                            D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	                        ZeroMemory( &descDSV, sizeof(descDSV) );
                            descDSV.Format = descDepth.Format;
                            descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
                            descDSV.Texture2D.MipSlice = 0;
                            if( !FAILED( m_d3dDevice->CreateDepthStencilView( m_depthStencil, &descDSV, &m_depthStencilView )) )
                            {
                                m_immediateContext->OMSetRenderTargets( 1, &m_renderTargetView, m_depthStencilView );

                                // Setup the viewport
                                D3D11_VIEWPORT vp;
                                vp.Width = (FLOAT)m_screenWidth;
                                vp.Height = (FLOAT)m_screenHeight;
                                vp.MinDepth = 0.0f;
                                vp.MaxDepth = 1.0f;
                                vp.TopLeftX = 0;
                                vp.TopLeftY = 0;
                                m_immediateContext->RSSetViewports( 1, &vp );

                                // Setup the projection matrix.
	                            float fieldOfView = (float)XM_PI / 4.0f;
	                            float screenAspect = (float)m_screenWidth / (float)m_screenHeight;

	                            // Create the projection matrix for 3D rendering.
                                XMStoreFloat4x4(&m_projectionMatrix,XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH));

                                // Initialize the world matrix to the identity matrix.
                                XMStoreFloat4x4(&m_worldMatrix,XMMatrixIdentity());

                                // Create an orthographic projection matrix for 2D rendering.
                                XMStoreFloat4x4(&m_orthoMatrix,XMMatrixOrthographicLH((float)m_screenWidth, (float)m_screenHeight, SCREEN_NEAR, SCREEN_DEPTH));

                                m_renderToTexture.Initialize(m_d3dDevice,m_screenWidth,m_screenHeight,4);
                                m_renderToCube.Initialize(m_d3dDevice,m_screenWidth,m_screenHeight,6);
                                m_depthWindow.Initialize(m_d3dDevice,m_immediateContext,m_screenWidth,m_screenHeight,m_screenWidth / 3,m_screenHeight / 3,1000,450);
                                m_shadowWindow.Initialize(m_d3dDevice,m_immediateContext,m_screenWidth,m_screenHeight,m_screenWidth,m_screenHeight,0,0);
                                m_fullScreenWindow.Initialize(m_d3dDevice,m_immediateContext,m_screenWidth,m_screenHeight,m_screenWidth,m_screenHeight,0,0);

                                CMOWResourceManager::Instance()->SetDevice(m_d3dDevice);
                                InitializeShaders();

                                m_input.Initialize(hInstance,hWnd,m_screenWidth,m_screenHeight);
                                
                                retVal = true;
                        
                            }
	                    }   
                    }
                }
                                
            }
        }
    }
    return retVal;
}
//---------------------------------------------
void CMOWRenderer::CreateLightStencilPassState()
{

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INVERT;
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INVERT;
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    

    m_d3dDevice->CreateDepthStencilState(&depthStencilDesc, &m_lightDepthStencilState);

    D3D11_RASTERIZER_DESC rasterDesc;
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    m_d3dDevice->CreateRasterizerState(&rasterDesc, &m_lightStencilRasterState);
}
//------------------------------------------------------
void CMOWRenderer::CreateShadowStencilPassState()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INVERT;
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INVERT;
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;


    m_d3dDevice->CreateDepthStencilState(&depthStencilDesc, &m_shadowDepthStencilState);
}

//---------------------------------------------
void CMOWRenderer::CreateLightPassState()
{

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    depthStencilDesc.DepthEnable = false;

    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    
    m_d3dDevice->CreateDepthStencilState(&depthStencilDesc, &m_lightState);

    D3D11_RASTERIZER_DESC rasterDesc;
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_FRONT;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    m_d3dDevice->CreateRasterizerState(&rasterDesc, &m_lightRasterState);
}
void CMOWRenderer::Render()
{
    CMOWTimer totalTime;
    CMOWTimer geoTime;
    CMOWTimer lightTime;

    totalTime.Start();
    //Update keyboard and mouse input
    m_input.Update();

    //Temp variables
    /*XMFLOAT4X4 lightViewMatrix;
    XMFLOAT4X4 lightProjectionMatrix;
    CDXShader::LightBufferDefinition lightDef;*/
    std::set<CMOWLight*> affectingLights;
    

    geoTime.Start();
    RenderToTexture(&m_renderToTexture,m_deferredShader,affectingLights);
    geoTime.Stop();
    lightTime.Start();
    RenderLights(m_lightShader,&m_renderToTexture.ShaderResourceViews());
    lightTime.Stop();
    //RenderBoundingVolumes();
    //RenderOctTrees();    
    
    /*std::vector<ID3D11ShaderResourceView*> resources;
    resources.push_back(m_renderToTexture.ShaderResourceView(1));

    m_shadowWindow.Render(m_immediateContext,
        m_textureShader,
        m_worldMatrix,
        m_scenes[0]->ActiveCamera()->GetViewMatrix(),
        m_orthoMatrix,
        lightViewMatrix,
        lightProjectionMatrix,
        m_scenes[0]->ActiveCamera()->GetPosition(),
        lightDef,
        m_screenWidth,
        m_screenHeight,
        &resources
        );*/

   /* m_fullScreenWindow.Render(m_immediateContext,
        &m_textureShader,
        m_worldMatrix,
        m_scenes[0]->ActiveCamera()->GetViewMatrix(),
        m_orthoMatrix,
        lightViewMatrix,
        lightProjectionMatrix,
        m_scenes[0]->ActiveCamera()->GetPosition(),
        lightDef,
        m_screenWidth,
        m_screenHeight,
        &resources
        );*/

    /*resources.clear();

    resources.push_back(m_renderToTexture.ShaderResourceView(1));

    m_shadowWindow.Render(m_immediateContext,
        &m_textureShader,
        m_worldMatrix,
        m_scenes[0]->ActiveCamera()->GetViewMatrix(),
        m_orthoMatrix,
        lightViewMatrix,
        lightProjectionMatrix,
        m_scenes[0]->ActiveCamera()->GetPosition(),
        lightDef,
        &resources
        );*/

    /*RenderToTexture(&m_renderToTexture,m_light2,cam,m_shaders[0]);
    resources.push_back(m_renderToTexture.ShaderResourceView());

    m_shadowWindow.Render(m_immediateContext,
        &m_textureShader,
        m_worldMatrix,
        GetActiveCamera()->GetViewMatrix(),
        m_orthoMatrix,
        lightViewMatrix,
        lightProjectionMatrix,
        GetActiveCamera()->GetPosition(),
        lightDef,
        &resources
        );*/


    
    /*shader_collection::iterator itShader = m_shaders.begin();

    while( itShader != m_shaders.end() )
    {
        CDXShader* shader = (*itShader);
        RenderScenes(shader);
        itShader++;
    }*/

    totalTime.Stop();
    if( m_gatherStatistics )
    {
        m_statistics.Clear();
        scene_collection::iterator itScene = m_scenes.begin();

        while( itScene != m_scenes.end() )
        {
            m_statistics.m_modelCount += (*itScene)->NumOfRenderedModels();
            m_statistics.m_polygonCount += (*itScene)->NumOfRenderedPolygons();
            m_statistics.m_vertexCount += (*itScene)->NumOfRenderedVertices();
            m_statistics.m_geoPassTime = geoTime.ElapsedTimeMicroSeconds();
            m_statistics.m_lightingPassTime = lightTime.ElapsedTimeMicroSeconds();
            m_statistics.m_totalTime = totalTime.ElapsedTimeMicroSeconds();
            itScene++;
        }
    }
}
//---------------------------------------------
void CMOWRenderer::AddScene( 
    CMOWScene* scene 
    )
{
    m_scenes.push_back(scene);   
}
//---------------------------------------------
void CMOWRenderer::Clear()
{
    
    m_d3dDevice->Release();
    m_immediateContext->ClearState();
    m_immediateContext->Release();
    m_renderTargetView->Release();
    m_swapChain->Release();
    m_blendState->Release();
    m_textBlendState->Release();
    
}
//---------------------------------------------
ID3D11Device* CMOWRenderer::Device() const
{
    return m_d3dDevice;
}
//---------------------------------------------
void CMOWRenderer::DrawText( 
    const wchar_t* text, 
    float xPos, 
    float yPos 
    )
{
    m_immediateContext->OMSetBlendState(m_textBlendState, 0, 0xffffffff);
    m_spriteBatch->Begin();
    m_spriteFont->DrawString(m_spriteBatch.get(), text, XMFLOAT2(xPos, yPos));
    m_spriteBatch->End();
    m_immediateContext->OMSetBlendState(0, 0, 0xffffffff);
}
//---------------------------------------------
void CMOWRenderer::ClearBuffers()
{
     m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
    // Clear the backbuffer
    float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha
    m_immediateContext->ClearRenderTargetView( m_renderTargetView, ClearColor );

    // Clear the depth buffer.
    m_immediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_immediateContext->OMSetDepthStencilState(m_depthStencilState, 1);
    m_immediateContext->RSSetState(m_rasterState);

    m_deferredShader->Clear(m_immediateContext);
    m_lightShader->Clear(m_immediateContext);
}
//---------------------------------------------
void CMOWRenderer::Present()
{
    m_swapChain->Present( 0, 0 );

    DWORD dwTimeCur = GetTickCount();
    float t = 0.0f;
    
    if( s_dwTimeStart == 0 )
        s_dwTimeStart = dwTimeCur;
    t = ( dwTimeCur - s_dwTimeStart ) / 1000.0f;

    s_dwTimeStart = dwTimeCur;
    if( t > 0.0f )
    {
        s_seconds += t;

        if( s_seconds >= 0.5f )
        {
            m_statistics.m_fps = s_frameCount*2;
            s_frameCount = 0;
            s_seconds = 0.0f;
        }
        s_frameCount++;
    }
    
}
//---------------------------------------------
void CMOWRenderer::RenderToTexture( 
    CMOWRenderToTexture* renderToTexture,
    CMOWShader* shader,
    std::set<CMOWLight*>& affectingLights
    )
{
    // Set the render target to be the render to texture.
    renderToTexture->SetRenderTargets(m_immediateContext, m_depthStencilView);
    //Clear the render to texture background to blue so we can differentiate it from the rest of the normal scene.

    // Clear the render to texture.
    renderToTexture->ClearRenderTargets(m_immediateContext, m_depthStencilView, 0.0f, 0.0f, 0.0f, 1.0f);

    // Render the scene now and it will draw to the render to texture instead of the back buffer.
    RenderScenes(shader,affectingLights);

    // Reset the render target back to the original back buffer and not the render to texture anymore.
    // Bind the render target view and depth stencil buffer to the output render pipeline.
    m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}
//---------------------------------------------
void CMOWRenderer::RenderScenes(
    CMOWShader* shader,
    std::set<CMOWLight*>& affectingLights
    )
{
    
    scene_collection::iterator itScene = m_scenes.begin();
    while( itScene != m_scenes.end() )
    {
        CMOWScene* scene = *itScene;

        scene->GatherStatistics(m_gatherStatistics);
        scene->Render(m_immediateContext,
                      shader,
                      m_projectionMatrix,
                      affectingLights,
                      m_screenWidth,
                      m_screenHeight
                     );

        itScene++;
    }
   
}
//---------------------------------------------
void CMOWRenderer::RemoveScene( CMOWScene* scene )
{
    scene_collection::iterator itScene = m_scenes.begin();

    while( itScene != m_scenes.end() )
    {
        if( *itScene == scene )
        {
            itScene = m_scenes.erase(itScene);
        }
        else
        {
            itScene++;
        }
    }
}
//---------------------------------------------
void CMOWRenderer::GatherStatistics( bool gather )
{
    m_gatherStatistics = gather;
}
//---------------------------------------------
const CMOWRendererStatistics* CMOWRenderer::Statistics() const
{
    return &m_statistics;
}
//---------------------------------------------
void CMOWRenderer::RenderLights( 
    CMOWShader* shader,
    std::vector<ID3D11ShaderResourceView*>* resources
    )
{
    
    LightRenderCBArg cbArg;

    cbArg.renderer = this;
    cbArg.resources = resources;

    //m_immediateContext->ClearDepthStencilView(m_depthStencilView,D3D11_CLEAR_STENCIL,1.0f,0);
    scene_collection::iterator itScene = m_scenes.begin();
    while( itScene != m_scenes.end() )
    {
        CMOWScene* scene = *itScene;

        scene->GatherStatistics(m_gatherStatistics);
        scene->RenderLights(RenderLight,&cbArg);
        itScene++;
    }
    m_immediateContext->OMSetBlendState(0,0,0xffffffff);
}
//---------------------------------------------
void CMOWRenderer::RegisterInputSink( 
    CMOWInput::IEvents* sink 
    )
{
    m_input.AddSink(sink);
}
//---------------------------------------------
void CMOWRenderer::RenderLight( 
    CMOWLight* light, 
    CMOWScene* scene,
    void* arg 
    )
{


    LightRenderCBArg* cbArg = reinterpret_cast<LightRenderCBArg*>(arg);
    CMOWRenderer* pThis = cbArg->renderer;
    std::vector<ID3D11ShaderResourceView*>* resources = cbArg->resources;

    pThis->m_immediateContext->ClearDepthStencilView(pThis->m_depthStencilView,D3D11_CLEAR_STENCIL,1.0f,0);
    //First render the stencil buffer
    
    
    pThis->m_immediateContext->RSSetState(pThis->m_lightStencilRasterState);
    
    
    //scene->RenderShadows(RenderShadow,light,pThis);

    pThis->m_immediateContext->OMSetDepthStencilState(pThis->m_lightDepthStencilState, 0);

    if( light->RenderableObject() )
    {
        light->RenderableObject()->Render(pThis->m_immediateContext,
            pThis->m_lightShader,
            scene->ActiveCamera()->GetViewMatrix(),
            pThis->m_projectionMatrix,
            light->GetViewMatrix(),
            light->GetProjectionMatrix(),
            scene->ActiveCamera()->Position(),
            light->AsShaderLight(),
            pThis->m_screenWidth,
            pThis->m_screenHeight,
            resources,
            false
            );
    }

    

    //And now the geometry
    pThis->m_immediateContext->OMSetDepthStencilState(pThis->m_lightState, 0);
    pThis->m_immediateContext->RSSetState(pThis->m_lightRasterState);
    pThis->m_immediateContext->OMSetBlendState(pThis->m_blendState,0,0xffffffff);

    

    if( light->RenderableObject() )
    {
        light->RenderableObject()->Render(pThis->m_immediateContext,
                                          pThis->m_lightShader,
                                          scene->ActiveCamera()->GetViewMatrix(),
                                          pThis->m_projectionMatrix,
                                          light->GetViewMatrix(),
                                          light->GetProjectionMatrix(),
                                          scene->ActiveCamera()->Position(),
                                          light->AsShaderLight(),
                                          pThis->m_screenWidth,
                                          pThis->m_screenHeight,
                                          resources
                                          );

        /*light->RenderableObject()->Render(pThis->m_immediateContext,
                                          CDXResourceManager::Instance()->GetShader("ColorPixelShader"),
                                          scene->ActiveCamera()->GetViewMatrix(),
                                          pThis->m_projectionMatrix,
                                          light->GetViewMatrix(),
                                          light->GetProjectionMatrix(),
                                          scene->ActiveCamera()->GetPosition(),
                                          light->AsShaderLight(),
                                          pThis->m_screenWidth,
                                          pThis->m_screenHeight,
                                          resources
                                          );*/
    }
}
//------------------------------------------------------
void CMOWRenderer::RenderShadow(
    CMOWLight* light, 
    CMOWModel* model,
    CMOWScene* scene,
    void* arg
    )
{
    
    CMOWRenderer* pThis = reinterpret_cast<CMOWRenderer*>(arg);

    pThis->m_immediateContext->OMSetDepthStencilState(pThis->m_shadowDepthStencilState, -1);
    
    model->Render(pThis->m_immediateContext,
                  pThis->m_shadowShader,
                  scene->ActiveCamera()->GetViewMatrix(),
                  pThis->m_projectionMatrix,
                  light->GetViewMatrix(),
                  light->GetProjectionMatrix(),
                  scene->ActiveCamera()->Position(),
                  light->AsShaderLight(),
                  pThis->m_screenWidth,
                  pThis->m_screenHeight,
                  0,
                  true,
                  true
                  );

}
//---------------------------------------
void CMOWRenderer::InitializeShaders()
{
    if( !m_textureShader && 
        !m_depthShader && 
        !m_deferredShader &&
        !m_lightShader &&
        !m_shadowShader &&
        m_d3dDevice)
    {
        m_textureShader = new CMOWTextureShader;
        m_depthShader = new CDXDepthShader;
        m_deferredShader = new CMOWDeferredShader;
        m_lightShader = new CMOWLightShader;
        m_shadowShader = new CMOWShadowShader;
        CMOWColorShader* colorShader = new CMOWColorShader;

        m_textureShader->Initialize(m_d3dDevice);
        m_depthShader->Initialize(m_d3dDevice);
        m_deferredShader->Initialize(m_d3dDevice);
        m_lightShader->Initialize(m_d3dDevice);
        m_shadowShader->Initialize(m_d3dDevice);
        colorShader->Initialize(m_d3dDevice);

        CMOWResourceManager::Instance()->AddShader(m_textureShader);
        CMOWResourceManager::Instance()->AddShader(m_depthShader);
        CMOWResourceManager::Instance()->AddShader(m_deferredShader);
        CMOWResourceManager::Instance()->AddShader(m_lightShader);
        CMOWResourceManager::Instance()->AddShader(m_shadowShader);
        CMOWResourceManager::Instance()->AddShader(colorShader);

    }

    
}
//---------------------------------------
void CMOWRenderer::RenderBoundingVolumes()
{
    scene_collection::iterator itScene = m_scenes.begin();
    while( itScene != m_scenes.end() )
    {
        CMOWScene* scene = *itScene;
        scene->RenderBoundingVolumes(m_immediateContext,m_projectionMatrix,m_screenWidth,m_screenHeight);
        itScene++;
    }
}
//---------------------------------------
void CMOWRenderer::RenderOctTrees()
{
    auto itScene = m_scenes.begin();
    while( itScene != m_scenes.end() )
    {
        CMOWScene* scene = *itScene;
        scene->RenderOctTree(m_immediateContext,m_projectionMatrix,m_screenWidth,m_screenHeight);
        itScene++;
    }
}
//------------------------------------------------------
void CMOWRenderer::AcquireMouse(bool acquire)
{
	m_input.AcquireMouse(acquire);
}
//------------------------------------------------------
void CMOWRenderer::AcquireKeyboard(bool acquire)
{
	m_input.AcquireKeyboard(acquire);
}
//---------------------------------------------
CMOWRendererStatistics::CMOWRendererStatistics()
{
    m_fps = 0;
    Clear();
}
//---------------------------------------------
void CMOWRendererStatistics::Clear()
{
    
    m_vertexCount = 0;
    m_modelCount = 0;
    m_polygonCount = 0;
    m_geoPassTime = 0;
    m_lightingPassTime = 0;
    m_totalTime = 0;
}

