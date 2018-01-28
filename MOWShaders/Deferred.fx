#include "Common.fx"

Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D metallicRoughHeightTexture : register(t2);

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;
    output.position = float4(0.0f,0.0f,0.0f,0.0f);
    output.worldPos = float3(0.0f,0.0f,0.0f);
    output.tex = float2(0.0f,0.0f);
    output.normal = float3(0.0f,0.0f,0.0f);
    output.viewDirection = float3(0.0f,0.0f,0.0f);
    output.lightViewPosition = float4(0.0f,0.0f,0.0f,0.0f);
    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, world);
    output.worldPos = output.position.xyz;
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);

    /*output.lightViewPosition.w = 1.0f;
    // Calculate the position of the vertex as viewed by the light source.
    //output.lightViewPosition = input.position;
    output.lightViewPosition = mul(input.position, worldMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);*/

    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)world);
    output.tangent = mul(input.tangent, (float3x3)world);
    output.biTangent = mul(input.biTangent, (float3x3)world);

    // Normalize the normal vector.
    output.normal = normalize(output.normal);
    output.tangent = normalize(output.tangent);
    output.biTangent = normalize(output.biTangent);
    return output;
}

DeferredPixelOutputType PS(PixelInputType input)
{
    DeferredPixelOutputType output;
    float depthValue;

    // Sample the color from the texture and store it for output to the render target.
    output.albedo = albedoTexture.Sample(ClampSamplerState, input.tex);
    output.metalRoughHeight = metallicRoughHeightTexture.Sample(ClampSamplerState, input.tex);
    float3 bumpNormal = normalTexture.Sample(ClampSamplerState, input.tex);
    float3x3 tbn = float3x3(input.tangent, input.biTangent, input.normal);

    bumpNormal = normalize(bumpNormal*2.0f - 1.0f);
    float3 tempNormal = mul(bumpNormal, tbn);

    //output.normal = float4(tempNormal*0.5f + 0.5f,1.0f);
    output.normal = float4(tempNormal,1.0f);

    // Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
    /*float zPos = input.lightViewPosition.z;
    float wPos = input.lightViewPosition.w;
    depthValue = zPos / wPos;

    //output.depth = float4(depthValue,depthValue,depthValue,1.0f);

    output.depth = float4(input.lightViewPosition.x,input.lightViewPosition.y,depthValue,input.lightViewPosition.z);*/

    
    
    output.position = float4(input.worldPos,input.lightViewPosition.w);
    

    return output;
}

technique11 DeferredTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}