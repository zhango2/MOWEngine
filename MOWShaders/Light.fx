#include "Common.fx"

Texture2D albedoTexture : register(t0);
Texture2D metalRoughHeightTexture : register(t1);
Texture2D normalTexture : register(t2);
Texture2D positionTexture : register(t3);

DepthStencilState FirstPassStencilState
{
    DepthEnable = true;
    DepthWriteMask = ZERO;
    DepthFunc = LESS;

    StencilEnable = true;
    StencilReadMask = 0xff;
    StencilWriteMask = 0xff;

    FrontFaceStencilFunc = ALWAYS;
    FrontFaceStencilDepthFail = DECR;
    FrontFaceStencilFail = KEEP;
    FrontFaceStencilPass = KEEP;
    
    BackFaceStencilFunc = ALWAYS;
    BackFaceStencilDepthFail = INCR;
    BackFaceStencilFail = KEEP;
    BackFaceStencilPass = KEEP;
};

DepthStencilState SecondPassStencilState
{
    DepthEnable = false;
    DepthWriteMask = ZERO;
    DepthFunc = LESS;

    StencilEnable = true;
    StencilReadMask = 0xff;
    StencilWriteMask = 0xff;

    FrontFaceStencilFunc = NOT_EQUAL;
    FrontFaceStencilDepthFail = DECR;
    FrontFaceStencilFail = KEEP;
    FrontFaceStencilPass = KEEP;
    
    BackFaceStencilFunc = NOT_EQUAL;
    BackFaceStencilDepthFail = INCR;
    BackFaceStencilFail = KEEP;
    BackFaceStencilPass = KEEP;
};

RasterizerState FirstPassRasterState
{
    AntialiasedLineEnable = false;
    CullMode = NONE;
    DepthBias = 0;
    DepthBiasClamp = 0.0f;
    DepthClipEnable = true;
    FillMode = SOLID;
    FrontCounterClockwise = false;
    MultisampleEnable = false;
    ScissorEnable = false;
    SlopeScaledDepthBias = 0.0f;    
};


RasterizerState SecondPassRasterState
{
    AntialiasedLineEnable = false;
    CullMode = FRONT;
    DepthBias = 0;
    DepthBiasClamp = 0.0f;
    DepthClipEnable = true;
    FillMode = SOLID;
    FrontCounterClockwise = false;
    MultisampleEnable = false;
    ScissorEnable = false;
    SlopeScaledDepthBias = 0.0f;
};

BlendState SecondPassBlendState
{
    BlendEnable[0] = true;
    BlendOp[0] = ADD;
    RenderTargetWriteMask[0] = 0x0f;
    SrcBlend[0] = ONE;
    DestBlend[0] = SRC_COLOR;
    BlendOpAlpha[0] = ADD;
    DestBlendAlpha[0] = ONE;
    SrcBlendAlpha[0] = ONE;
};

static const float PI = 3.14159265359;

void DistributionGGX(
    float3 N, 
    float3 H, 
    float roughness,
    out float NDF
    )
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    NDF = nom / denom;
}

float GeometrySchlickGGX(
    float NdotV, 
    float roughness
    )
{
    float r = (roughness + 1.0f);
    float k = (r*r) / 8.0f;

    float nom   = NdotV;
    float denom = NdotV * (1.0f - k) + k;

    return nom / denom;
}

void GeometrySmith(
    float3 N, 
    float3 V, 
    float3 L, 
    float roughness,
    out float G
    )
{
    float NdotV = max(dot(N, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    G = ggx1 * ggx2;
}

void FresnelSchlick(
    float cosTheta,
    float3 F0,
    out float3 F
    )
{
    F = F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

void ComputePointLightPBR(
    Light light,
    float3 worldPos,
    float3 N,
    float3 cameraPos,
    float3 albedo,
    float metallic,
    float roughness,
    out float3 color
    )
{

    /*vec3 lightDir   = normalize(lightPos - FragPos);
    vec3 viewDir    = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);*/



    float3 V = normalize(cameraPos - worldPos); //View direction
    float3 lightColor = light.diffuse.rgb;
    float3 L = normalize(light.position - worldPos); //Light direction
    float3 H = normalize(V + L); //Halfway direction
    float distance = length(light.position - worldPos);

    float attenuation = light.attenuation.x + (light.attenuation.y * distance) + (light.attenuation.z * (distance*distance));
    attenuation = 1.0f / attenuation;
    //float attenuation = 1.0f / (distance * distance);
    float3 radiance = lightColor * attenuation;
    float3 F0 = float3(0.04f,0.04f,0.04f);
    float G = 1.0f;
    float NDF = 1.0f;
    float3 F = F0;

    F0 = lerp(F0,albedo,metallic);

    
    DistributionGGX(N, H, roughness, NDF);
    GeometrySmith(N, V, L, roughness, G);
    FresnelSchlick(max(dot(H, V), 0.0f), F0, F);

    float3 kS = F;
    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
    //float3 kD = albedo;
    kD *= 1.0f - metallic;

    float3 nominator = NDF * G * F;
    float denominator = 4.0f * (max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f));
    float3 specular = nominator / max(denominator, 0.001f);
    

    float NdotL = max(dot(N, L), 0.0f);
    //float ao = 0.3f;

    color = (kD * albedo / PI + specular) * radiance * NdotL;
    float3 ambient = float3(0.003f, 0.003f, 0.003f) * albedo;

    color += ambient;
    float colorPow = 1.0f/2.2f;

    color = color / (color + float3(1.0f, 1.0f, 1.0f));
    color = pow(color, float3( colorPow, colorPow, colorPow));
    

}


void ComputePointLight(Material mat, 
                       Light light, 
                       float3 pos, 
                       float3 normal, 
                       float3 toEye,
                       out float4 ambient, 
                       out float4 diffuse, 
                       out float4 spec
                       )
{
    // Initialize outputs.
    ambient = float4(0.01f, 0.01f, 0.01f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // The vector from the surface to the light.
    float3 lightVec = light.position - pos;
        
    // The distance from surface to light.
    float d = length(lightVec);
        
    // Normalize the light vector.
    lightVec /= d; 
    
    // Ambient term.
    ambient = mat.ambient * light.ambient;

    // Add diffuse and specular term, provided the surface is in 
    // the line of site of the light.

    float diffuseFactor = dot(lightVec, normal);

    // Flatten to avoid dynamic branching.
    [flatten]
    if( diffuseFactor > 0.0f )
    {
        float3 v         = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
                    
        diffuse = diffuseFactor * mat.diffuse * light.diffuse;
        spec    = specFactor * mat.specular * light.specular;
    }

    // Attenuate
    //float att = 1.0f / dot(light.attenuation, float3(1.0f, d, d*d));
    float att = light.attenuation.x + (light.attenuation.y * d) + (light.attenuation.z * (d*d));

    diffuse *= (1.0 / att);
    spec    *= (1.0 / att);
}

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;
    
    output.position = float4(0.0f,0.0f,0.0f,0.0f);
    output.worldPos = float3(0.0f,0.0f,0.0f);
    output.tex = float2(0.0f,0.0f);
    output.normal = float3(0.0f,0.0f,0.0f);
    output.viewDirection = float3(0.0f,0.0f,0.0f);
    output.lightViewPosition = float4(0.0f,0.0f,0.0f,0.0f);
    output.color = float4(0.0f,0.0f,0.0f,0.0f);

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, world);
    output.worldPos = output.position.xyz;
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, world);

    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    /*output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
    
    // Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);*/
    
    return output;
}

float4 PS(PixelInputType input, in float4 screenPos : SV_position) : SV_Target0
{
    float4 worldPos = float4(0.0f,0.0f,0.0f,1.0f);
    float4 albedo = float4(0.0f,0.0f,0.0f,0.0f);
    float4 normal;

    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular    = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 resultColor    = float4(0.0f, 0.0f, 0.0f, 0.0f);

    Material mat;

    mat.ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    mat.diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    mat.specular    = float4(0.0f, 0.0f, 0.0f, 0.0f);

    screenPos.x = screenPos.x / (float)screenWidth;
    screenPos.y = screenPos.y / (float)screenHeight;

    //ambient = float4(screenPos.x,screenPos.y,1.0f,1.0f);

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    
    albedo = albedoTexture.Sample(WrapPointSamplerState, screenPos.xy);
    float4 metallicRoughHeight = metalRoughHeightTexture.Sample(WrapPointSamplerState, screenPos.xy);

    //Sample normal
    normal = normalTexture.Sample(WrapPointSamplerState,screenPos.xy);
    
    //Sample position
    worldPos = positionTexture.Sample(WrapPointSamplerState,screenPos.xy);

    /*float4 a,d,s;
    
    if(light.lightType == 2)
    {
        float3 toEye = normalize(cameraPosition - worldPos);
        ComputePointLight(mat,light,worldPos,normal,toEye,a,d,s);
    }
    ambient += a;
    diffuse += d;
    specular += s;

    

    resultColor = textureColor*(ambient + diffuse) + specular;*/
    //resultColor *= textureColor;
    /*resultColor = saturate(resultColor/* + (specular * attenuation));*/

    if(light.lightType == 2)
    {
        float3 color;
        float3 toEye = normalize(cameraPosition - worldPos);
        ComputePointLightPBR(light,worldPos.xyz,normal.xyz,cameraPosition, albedo.xyz, metallicRoughHeight.x, metallicRoughHeight.y, color);
        resultColor = float4(color,0.0f);
    }
    

    return resultColor;
}

technique11 LightTechNoPixelShader
{
    pass P0
    {
        
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( NULL );
    
    }
}

technique11 LightTech
{
    pass P0
    {
        
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}

/*technique11 LightTech
{
    pass P0
    {
        
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( NULL );
        SetDepthStencilState(FirstPassStencilState,1);
        SetRasterizerState(FirstPassRasterState);
    }
    pass P1
    {
        
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
        SetDepthStencilState(SecondPassStencilState,1);
        SetRasterizerState(SecondPassRasterState);
        SetBlendState(SecondPassBlendState,float4(0.0f,0.0f,0.0f,0.0f),0xffffffff);
        
    }
}*/


