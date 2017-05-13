#include "Common.fx"
Texture2D diffuseTexture : register(t0);

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
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);

    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    return output;
}

float4 PS(PixelInputType input) : SV_TARGET
{
    float4 textureColor;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = diffuseTexture.Sample(WrapSamplerState, input.tex);

    return textureColor;
}

technique11 TextureTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}