#ifndef _DXSHADER_H
#define _DXSHADER_H

#include "MOWGraphics/MOWCommon.h"

struct ID3D11Device;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;
struct ID3D11DeviceContext;
struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectScalarVariable;
struct ID3DX11EffectVariable;
struct ID3DX11EffectVectorVariable;

class CMOWShader
{
public:

    struct MatrixBufferDefinition
    {
        DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
        DirectX::XMMATRIX lightViewMatrix;
        DirectX::XMMATRIX lightProjectionMatrix;
        int screenWidth;
        int screenHeight;

    };

    struct FxVariables
    {
        FxVariables();
        ~FxVariables();

        ID3DX11EffectMatrixVariable* world;
        ID3DX11EffectMatrixVariable* view;
        ID3DX11EffectMatrixVariable* projection;
        ID3DX11EffectMatrixVariable* lightView;
        ID3DX11EffectMatrixVariable* lightProjection;
        ID3DX11EffectVectorVariable* cameraPosition;
        ID3DX11EffectScalarVariable* screenWidth;
        ID3DX11EffectScalarVariable* screenHeight;
        ID3DX11EffectVariable* material;
        ID3DX11EffectVariable* light;
    };

    struct LightBufferDefinition
    {


        DirectX::XMFLOAT4 lightAmbientColor;
        DirectX::XMFLOAT4 lightDiffuseColor;
        DirectX::XMFLOAT4 lightSpecularColor;
        DirectX::XMFLOAT3 lightPosition;
        float specularPower;
        DirectX::XMFLOAT4 Ka;
        DirectX::XMFLOAT4 Kd;
        DirectX::XMFLOAT4 Ks;
        DirectX::XMFLOAT4 Ke;
        DirectX::XMFLOAT3 lightDirection;
        int hasPosition;

    };

    struct CameraBufferDefinition
    {
        DirectX::XMFLOAT3 cameraPosition;
        float padding;
    };

    CMOWShader(
        );

    virtual ~CMOWShader(
                );

    bool                            
        Initialize(
            ID3D11Device* device
            );

    bool                            
        Update(
            ID3D11DeviceContext* context,
            const DirectX::XMFLOAT4X4& worldMatrix,
            const DirectX::XMFLOAT4X4& viewMatrix,
            const DirectX::XMFLOAT4X4& projectionMatrix,
            const DirectX::XMFLOAT4X4& lightViewMatrix,
            const DirectX::XMFLOAT4X4& lightProjectionMatrix,
            const LightBufferDefinition& lightDef,
            const DirectX::XMVECTOR& cameraPosition,
            int screenWidth,
            int screenHeight,
            const std::vector<ID3D11ShaderResourceView*>* resources = 0,
            bool usePixelShader = true
            );

    bool                            
        Update(
            ID3D11DeviceContext* context,
            const DirectX::XMFLOAT4X4& worldMatrix,
            const DirectX::XMFLOAT4X4& viewMatrix,
            const DirectX::XMFLOAT4X4& projectionMatrix,
            const DirectX::XMFLOAT4X4& lightViewMatrix,
            const DirectX::XMFLOAT4X4& lightProjectionMatrix,
            const ShaderLight& light,
            const ShaderMaterial& material,
            const DirectX::XMVECTOR& cameraPosition,
            int screenWidth,
            int screenHeight,
            const std::vector<ID3D11ShaderResourceView*>* resources = 0,
            bool usePixelShader = true
            );

    void                            
        Clear(
            ID3D11DeviceContext* context
            );

    virtual const char*             
        VertexShaderDescription(
            );

    virtual const char*             
        PixelShaderDescription(
            );

    virtual const char*             
        VertexShaderFilename(
            );

    virtual const char*             
        PixelShaderFilename(
            );

    virtual const char*             
        FxFilename(
            );

    virtual const char*             
        FxTechName(
            );

    virtual const char*             
        FxTechNoPixelShaderName(
            );

protected:

    virtual void                    
        SetupElementDescriptions(
            std::vector<D3D11_INPUT_ELEMENT_DESC>& descriptions
            );

    virtual void                    
        CreateBuffers(
            ID3D11Device* device
            );
    
    
    virtual void                    
        CreateSamplerStates(
            ID3D11Device* device,
            std::vector<ID3D11SamplerState*>& samplerStates
            );

    virtual void                    
        GetVSBuffers(
            std::vector<ID3D11Buffer*>& buffers
            );

    virtual void                    
        GetPSBuffers(
            std::vector<ID3D11Buffer*>& buffers
            );

    virtual HRESULT                 
        SetupBuffer(
            ID3D11Buffer* buffer,
            ID3D11DeviceContext* context,
            DirectX::XMMATRIX& worldMatrix,
            DirectX::XMMATRIX& viewMatrix,
            DirectX::XMMATRIX& projectionMatrix,
            DirectX::XMMATRIX& lightViewMatrix,
            DirectX::XMMATRIX& lightProjectionMatrix,
            LightBufferDefinition& lightDef,
            const DirectX::XMVECTOR& cameraPosition,
            int screenWidth,
            int screenHeight
            );

    virtual bool                    
        UseResources(
            ) const;

    virtual bool                    
        CreateFX(
            ID3D11Device* device
            );

    virtual void ApplyFXResources(
            const std::vector<ID3D11ShaderResourceView*>* resources
            );

    D3D11_INPUT_ELEMENT_DESC        
        CreateVertexElementDesc(
            const char* name,
            DXGI_FORMAT format,
            UINT alignedByOffset = D3D11_APPEND_ALIGNED_ELEMENT
            );

    std::string                     
        m_vertexShaderDescription;

    std::string                     
        m_pixelShaderDescription;

    std::string                     
        m_vertexShaderFilename;

    std::string                     
        m_pixelShaderFilename;

    std::string                     
        m_fxFileName;

    std::string                     
        m_fxTechName;

    std::string                     
        m_fxTechNoPixelShaderName;

    FxVariables                     
        m_fxVariables;

    ID3DX11Effect*                  
        m_fx;

    ID3DX11EffectTechnique*         
        m_fxTech;

    ID3DX11EffectTechnique*         
        m_fxTechNoPixelShader;


private:

    std::vector<byte>               
        GetShaderBytes(
            const char* shaderFileName
            );

    ID3D11VertexShader*             
        m_vertexShader;

	ID3D11PixelShader*              
        m_pixelShader;

	ID3D11InputLayout*              
        m_layout;

	ID3D11Buffer*                   
        m_matrixBuffer;

    std::vector<ID3D11SamplerState*> 
        m_samplerStates;

    int                             
        m_resourceCount;
    

};


#endif