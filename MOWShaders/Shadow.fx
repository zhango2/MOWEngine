#include "Common.fx"

struct VertexOut
{
    float4 worldPos : SV_POSITION;
    float4 pos : POSITION;
};

float EPSILON = 0.0001f;

void EmitQuad(
    float3 edge1,
    float3 edge2,
    float3 lightDir,
    int startIndex,
    int endIndex,
    Light light,
    inout VertexOut vertices[6],
    inout TriangleStream<VertexOut> triangleStream
    )
{
    float3 normal = cross(edge1,edge2);

    if( dot(normal,lightDir) <=0.0f )
    {

        

        float4 startVert = vertices[startIndex].worldPos;
        float4 endVert = vertices[endIndex].worldPos;
        

        lightDir = normalize(startVert.xyz - light.position);
        float3 l = lightDir * EPSILON;
        float4 tempLightDir = float4(lightDir.xyz,1.0f);

        VertexOut output;
        output.pos = float4(0.0f,0.0f,0.0f,0.0f);

        

        // Vertex #1: the starting vertex (just a tiny bit below the original edge)
        
        float4 finalPos = float4((startVert.xyz + l),1.0f);
        
        
        finalPos = mul(finalPos,view);
        finalPos = mul(finalPos,projection);
        output.worldPos = finalPos;
        

        triangleStream.Append(output);

       // Vertex #2: the starting vertex projected to infinity
        //finalPos = float4(lightDir,0.0f);
        
        finalPos = float4(lightDir,0.0f);

        
        finalPos = mul(finalPos,view);
        finalPos = mul(finalPos,projection);
        output.worldPos = finalPos;

        triangleStream.Append(output);

        // Vertex #3: the ending vertex (just a tiny bit below the original edge)
        lightDir = normalize(endVert.xyz - light.position);
        l = lightDir*EPSILON;
        finalPos = float4((endVert.xyz + l),1.0f);
        
        finalPos = mul(finalPos,view);
        finalPos = mul(finalPos,projection);
        output.worldPos = finalPos;
        

        triangleStream.Append(output);
        
        

        // Vertex #4: the ending vertex projected to infinity
        
        finalPos = float4(lightDir,0.0f);
        
        finalPos = mul(finalPos,view);
        finalPos = mul(finalPos,projection);
        output.worldPos = finalPos;

        triangleStream.Append(output);

        
        
    }
}

VertexOut VS(VertexInputType input)
{
    VertexOut output;
    
    output.pos = input.position;
    output.worldPos = float4(0.0f,0.0f,0.0f,1.0f);
    // Calculate the position of the vertex against the world, view, and projection matrices.
    
    output.worldPos = mul(input.position, world);

    return output;
}


[maxvertexcount(18)]
void GS(triangleadj VertexOut input[6],inout TriangleStream<VertexOut> triStream)
{
    float4 e1 = input[2].worldPos - input[0].worldPos;
    float4 e2 = input[4].worldPos - input[0].worldPos;
    float4 e3 = input[1].worldPos - input[0].worldPos;
    float4 e4 = input[3].worldPos - input[2].worldPos;
    float4 e5 = input[4].worldPos - input[2].worldPos;
    float4 e6 = input[5].worldPos - input[0].worldPos;

    float3 normal = cross(e1.xyz,e2.xyz);
    float3 lightDir = light.position - input[0].worldPos.xyz;

    if( dot(normal.xyz,lightDir) > 0.0001f )
    {
        VertexOut output;
        output.pos = float4(0.0f,0.0f,0.0f,0.0f);
        float4 tempPos = float4(0.0f,0.0f,0.0f,0.0f);
        EmitQuad(e3.xyz,e1.xyz,lightDir,0,2,light,input,triStream);

        lightDir = light.position - input[2].worldPos.xyz;

        EmitQuad(e4.xyz,e5.xyz,lightDir,2,4,light,input,triStream);

        lightDir = light.position - input[4].worldPos.xyz;

        EmitQuad(e2.xyz,e6.xyz,lightDir,4,0,light,input,triStream);

        //Front cap
        
       
        triStream.RestartStrip();
    
        /*lightDir = (normalize(input[0].worldPos.xyz - light.position)) * EPSILON;
        tempPos = float4((input[0].worldPos.xyz + lightDir),1.0f);
        
        output.worldPos = mul(tempPos,view);
        output.worldPos = mul(output.worldPos,projection);

        triStream.Append(output);

        lightDir = (normalize(input[2].worldPos.xyz - light.position))*EPSILON;
        tempPos = float4((input[2].worldPos.xyz + lightDir),1.0f);
        
        output.worldPos = mul(tempPos,view);
        output.worldPos = mul(output.worldPos,projection);

        triStream.Append(output);

        lightDir = (normalize(input[4].worldPos.xyz - light.position))*EPSILON;
        tempPos = float4((input[4].worldPos.xyz + lightDir),1.0f);
        
        output.worldPos = mul(tempPos,view);
        output.worldPos = mul(output.worldPos,projection);

        triStream.Append(output);

        triStream.RestartStrip();*/

        //Back cap

        

        

        /*lightDir = normalize(input[4].worldPos.xyz - light.position);
        tempPos = float4(lightDir,0.0f);
        //tempPos = mul(tempPos,world);
        output.worldPos = mul(tempPos,view);
        output.worldPos = mul(output.worldPos,projection);
        
        triStream.Append(output);

        lightDir = normalize(input[2].worldPos.xyz - light.position);
        tempPos = float4(lightDir,0.0f);
        //tempPos = mul(tempPos,world);
        output.worldPos = mul(tempPos,view);
        output.worldPos = mul(output.worldPos,projection);
    
        triStream.Append(output);

        lightDir = normalize(input[0].worldPos.xyz - light.position);
        tempPos = float4(lightDir,0.0f);

        //tempPos = mul(tempPos,world);
        output.worldPos = mul(tempPos,view);
        output.worldPos = mul(output.worldPos,projection);
        
        triStream.Append(output);

        triStream.RestartStrip();*/
    }

    

    //TODO: render front and back cap
}

float4 PS(VertexOut input):SV_Target
{
    float4 output = float4(0.5f,0.0f,0.5f,0.0f);
    return output;
}

technique11 ShadowTech
{
    pass P0
    {
        
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( CompileShader( gs_5_0, GS() ) );
        //TODO:Fix pixel shader
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    
    }
}