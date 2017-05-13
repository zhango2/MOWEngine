
#include "MOWRenderToTexture.h"

//---------------------------------------------
CMOWRenderToTexture::CMOWRenderToTexture()
{
   
}
//---------------------------------------------
CMOWRenderToTexture::~CMOWRenderToTexture()
{
    ClearTextures();
    ClearTargetViews();
    ClearResourceViews();
}
//---------------------------------------------
void CMOWRenderToTexture::Initialize( 
    ID3D11Device* device, 
    int textureWidth, 
    int textureHeight,
    int bufferSize
    )
{
    D3D11_TEXTURE2D_DESC textureDesc;
    HRESULT result;
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;


    // Initialize the render target texture description.
    ZeroMemory(&textureDesc, sizeof(textureDesc));

    // Setup the render target texture description.
    textureDesc.Width = textureWidth;
    textureDesc.Height = textureHeight;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    
    
    // Setup the description of the render target view.
    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    
    
    
    // Setup the description of the shader resource view.
    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    

    for( int i=0; i<bufferSize; i++ )
    {
        ID3D11Texture2D* renderTargetTexture = 0;
        ID3D11RenderTargetView* renderTargetView = 0;
        ID3D11ShaderResourceView* shaderResourceView = 0;

        // Create the render target texture.
        result = device->CreateTexture2D(&textureDesc, NULL, &renderTargetTexture);
        // Create the render target view.
        result = device->CreateRenderTargetView(renderTargetTexture, &renderTargetViewDesc, &renderTargetView);

        // Create the shader resource view.
        result = device->CreateShaderResourceView(renderTargetTexture, &shaderResourceViewDesc, &shaderResourceView);

        m_renderTargetTextures.push_back(renderTargetTexture);
        m_renderTargetViews.push_back(renderTargetView);
        m_shaderResourceViews.push_back(shaderResourceView);
    }
}
//---------------------------------------------
void CMOWRenderToTexture::SetRenderTargets( 
    ID3D11DeviceContext* context, 
    ID3D11DepthStencilView* view 
    )
{
    // Bind the render target view and depth stencil buffer to the output render pipeline.
    context->OMSetRenderTargets(m_renderTargetViews.size(), &m_renderTargetViews[0], view);
}
//---------------------------------------------
void CMOWRenderToTexture::ClearRenderTargets( 
    ID3D11DeviceContext* context, 
    ID3D11DepthStencilView* view, 
    float red, 
    float green, 
    float blue, 
    float alpha 
    )
{
    float color[4];

    // Setup the color to clear the buffer to.
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    color[3] = alpha;

    for( size_t i=0; i<m_renderTargetViews.size(); i++ )
    {
        // Clear the back buffer.
        context->ClearRenderTargetView(m_renderTargetViews[i], color);
    }
    

    // Clear the depth buffer.
    context->ClearDepthStencilView(view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
//---------------------------------------------
ID3D11ShaderResourceView* CMOWRenderToTexture::ShaderResourceView(size_t index) const
{
    ID3D11ShaderResourceView* resView = 0;
    if( index >=0 && index < m_shaderResourceViews.size() )
    {
        resView = m_shaderResourceViews[index];
    }
    return resView;
}
//---------------------------------------------
void CMOWRenderToTexture::ClearTextures()
{
    texture_collection::iterator itTexture = m_renderTargetTextures.begin();

    while( itTexture != m_renderTargetTextures.end() )
    {
        ID3D11Texture2D* texture = (*itTexture);
        texture->Release();
        itTexture = m_renderTargetTextures.erase(itTexture);
    }
}
//---------------------------------------------
void CMOWRenderToTexture::ClearTargetViews()
{
    target_view_collection::iterator itTarget = m_renderTargetViews.begin();

    while( itTarget != m_renderTargetViews.end() )
    {
        ID3D11RenderTargetView* target = (*itTarget);
        target->Release();
        itTarget = m_renderTargetViews.erase(itTarget);
    }
}
//---------------------------------------------
void CMOWRenderToTexture::ClearResourceViews()
{
    resource_view_collection::iterator itRes = m_shaderResourceViews.begin();

    while( itRes != m_shaderResourceViews.end() )
    {
        ID3D11ShaderResourceView* res = (*itRes);
        res->Release();
        itRes = m_shaderResourceViews.erase(itRes);
    }
}
//---------------------------------------------
CMOWRenderToTexture::resource_view_collection& CMOWRenderToTexture::ShaderResourceViews()
{
    return m_shaderResourceViews;
}

