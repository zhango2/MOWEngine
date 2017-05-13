
struct Light
{
    float4 ambient;
    float4 diffuse;
    float4 specular;

    float3 position;
	float range;

	float3 direction;
	float spot;

	float3 attenuation;

    int lightType; //0=none,1=directional,2=point,3=spot

};

struct Material
{
    float4 ambient;
    float4 diffuse;
    float4 specular; //w=specular power
    float4 reflection;
};

cbuffer PerObject
{
    matrix world;
    matrix view;
    matrix projection;
    matrix lightView;
    matrix lightProjection;
    float3 cameraPosition;
    int screenWidth;
    int screenHeight;
    Material material;
    Light light;
    
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
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

struct DeferredPixelOutputType
{
    float4 color : SV_TARGET0;

    float4 ambient : SV_TARGET1;
    float4 diffuse : SV_TARGET2;
    float4 specular : SV_TARGET3;
    float4 normal : SV_TARGET4;

    //contains the position in xyz
    //and lighViewPos.w in w
    float4 position : SV_TARGET5;
	
};

//Texture2D diffuseTexture;

SamplerState WrapSamplerState
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
    AddressW = WRAP;
    ComparisonFunc = ALWAYS;
};

SamplerState WrapPointSamplerState
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
    AddressW = WRAP;
    ComparisonFunc = ALWAYS;
};

SamplerState ClampSamplerState
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
    AddressW = CLAMP;
    ComparisonFunc = ALWAYS;
};
