
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix lightViewMatrix;
    matrix lightProjectionMatrix;
    int screenWidth;
    int screenHeight;
};

cbuffer LightBuffer
{
    float4 lightAmbientColor;
    float4 lightDiffuseColor;
    float4 lightSpecularColor;
    float3 lightPos;
    float specularPower;
    float4 Ka;
    float4 Kd;
    float4 Ks;
    float4 Ke;
    float3 lightDirection;
    int hasPosition;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
    float padding;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 biTangent : BINORMAL;
    float4 color : COLOR;
    
};

struct PixelInputType
{
    //float4 screenPos : SV_Position;
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
    float3 worldPos : TEXCOORD3;
    float4 lightViewPosition : TEXCOORD4;
    float4 color : COLOR;
};

struct PixelOutputType
{
    float4 color : SV_TARGET0;

    //Contains depth value in x, 
    //lightViewPos.x in y,
    //lightViewPos.y in z,
    //and lightViewPos.z in w.
    float4 depth : SV_TARGET1;
    
    float4 normal : SV_TARGET2;

    //contains the position in xyz
    //and lighViewPos.w in w
    float4 position : SV_TARGET3;
    
};
