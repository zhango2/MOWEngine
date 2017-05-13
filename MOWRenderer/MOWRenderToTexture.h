#ifndef DXRenderToTexture_H
#define DXRenderToTexture_H

#include <vector>
#include <d3d11.h>

class CMOWRenderToTexture
{
    typedef std::vector<ID3D11Texture2D*> texture_collection;
    typedef std::vector<ID3D11RenderTargetView*> target_view_collection;
    typedef std::vector<ID3D11ShaderResourceView*> resource_view_collection;

public:

    CMOWRenderToTexture(
        );
                                
    ~CMOWRenderToTexture(
        );

    void                        
        Initialize(
            ID3D11Device* device,
            int textureWidth,
            int textureHeight,
            int bufferSize
            );

    void                        
        SetRenderTargets(
            ID3D11DeviceContext* context,
            ID3D11DepthStencilView* view
            );

    void                        
        ClearRenderTargets(
            ID3D11DeviceContext* context,
            ID3D11DepthStencilView* view,
            float red,
            float green,
            float blue,
            float alpha
            );

    ID3D11ShaderResourceView*   
        ShaderResourceView(
            size_t index
            ) const;

    resource_view_collection&   
        ShaderResourceViews(
            );

private:

    void                        
        ClearTextures(
            );

    void                        
        ClearTargetViews(
            );

    void                        
        ClearResourceViews(
            );

    texture_collection          
        m_renderTargetTextures;

    target_view_collection      
        m_renderTargetViews;

    resource_view_collection    
        m_shaderResourceViews;
};

#endif