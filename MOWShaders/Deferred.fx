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

    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    return output;
}

DeferredPixelOutputType PS(PixelInputType input)
{
    DeferredPixelOutputType output;
    float depthValue;

    // Sample the color from the texture and store it for output to the render target.
    output.color = diffuseTexture.Sample(ClampSamplerState, input.tex);

    

    // Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
    /*float zPos = input.lightViewPosition.z;
    float wPos = input.lightViewPosition.w;
    depthValue = zPos / wPos;

    //output.depth = float4(depthValue,depthValue,depthValue,1.0f);

    output.depth = float4(input.lightViewPosition.x,input.lightViewPosition.y,depthValue,input.lightViewPosition.z);*/

    
    output.ambient = float4(material.ambient);
    output.diffuse = float4(material.diffuse);
    output.specular = float4(material.specular);
    output.normal = float4(input.normal, 1.0f);
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