#include "ShaderTypes.h"

PixelInputType main(VertexInputType input)
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
    output.position = mul(input.position, worldMatrix);
	output.worldPos = output.position.xyz;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	output.lightViewPosition.w = 1.0f;
	// Calculate the position of the vertex as viewed by the light source.
	//output.lightViewPosition = input.position;
    output.lightViewPosition = mul(input.position, worldMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);

    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
	
    // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    return output;
}