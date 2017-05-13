#include "ShaderTypes.h"

Texture2D shaderTexture;
SamplerState SampleType;

float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    return textureColor;
}