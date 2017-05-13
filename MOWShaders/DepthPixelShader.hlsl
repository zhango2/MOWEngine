#include "ShaderTypes.h"

float4 main(PixelInputType input) : SV_TARGET
{
	float depthValue;
	float4 color;

	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
	depthValue = input.position.z / input.position.w;

	color = float4(depthValue, depthValue, depthValue, 1.0f);

	return color;
}