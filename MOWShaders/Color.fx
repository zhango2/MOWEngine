#include "Common.fx"

PixelInputType VS(VertexInputType input)
{
	PixelInputType output;
	
	// Transform to homogeneous clip space.
    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
	
	// Just pass vertex color into the pixel shader.
    output.color = input.color;
    
    return output;
}

float4 PS(PixelInputType input) : SV_Target
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
    //return input.color;
}

technique11 ColorTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}
