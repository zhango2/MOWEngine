#include "ShaderTypes.h"

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;
    
    output.position = float4(0.0f,0.0f,0.0f,0.0f);
    output.worldPos = float3(0.0f,0.0f,0.0f);
    output.tex = float2(0.0f,0.0f);
    output.normal = float3(0.0f,0.0f,0.0f);
    output.viewDirection = float3(0.0f,0.0f,0.0f);
    output.lightViewPosition = float4(0.0f,0.0f,0.0f,0.0f);

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
	output.worldPos = output.position.xyz;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	// Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	
    // Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);
    
    return output;
}