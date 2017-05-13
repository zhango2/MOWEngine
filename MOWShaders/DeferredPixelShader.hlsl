#include "ShaderTypes.h"

Texture2D shaderTexture : register(t0);
SamplerState SampleTypeWrap : register(s0);

PixelOutputType main(PixelInputType input) : SV_TARGET
{
    PixelOutputType output;
	float depthValue;

    // Sample the color from the texture and store it for output to the render target.
    output.color = shaderTexture.Sample(SampleTypeWrap, input.tex);
	
    // Store the normal for output to the render target.
    output.normal = float4(input.normal, 1.0f);

	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
	float zPos = input.lightViewPosition.z;
	float wPos = input.lightViewPosition.w;
	depthValue = zPos / wPos;

	//output.depth = float4(depthValue,depthValue,depthValue,1.0f);

	output.depth = float4(input.lightViewPosition.x,input.lightViewPosition.y,depthValue,input.lightViewPosition.z);

	output.position = float4(input.worldPos,input.lightViewPosition.w);

	



    return output;
}