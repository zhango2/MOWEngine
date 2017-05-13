#include "ShaderTypes.h"

Texture2D colorTexture : register(t0);
Texture2D depthMapTexture : register(t1);
Texture2D normalTexture : register(t2);
Texture2D positionTexture : register(t3);

SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypePoint : register(s1);

float DiffuseDirectionalLight(float4 normal,float3 direction)
{
	float diffuseLight = 0.0f;
	// Invert the light direction for calculations.
	float3 lightDir = -direction;
	diffuseLight = saturate(dot(normal.xyz, lightDir));
	return diffuseLight;
}

float SpecularDirectionalLight(float4 normal,float3 direction,float diffuseLight, float specPower,float4 viewDirection)
{
	float specularLight = 0.0f;

	if(diffuseLight > 0.0f && specPower > 0.0f)
	{
		float3 lightDir = -direction;
		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
		float3 reflection = normalize(2 * diffuseLight * normal.xyz - lightDir);

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
		specularLight = pow(saturate(dot(reflection, viewDirection.xyz)), specPower);
	}

	return specularLight;

}

bool LightPixel(float4 lightViewPosition)
{
	bool light = false;
	float bias = 0.001f;
	float2 projectTexCoord;
    float4 depth = float4(0.0f,0.0f,0.0f,0.0f);
    float depthValue = 0.0f;
    float lightDepthValue = 0.0f;

	// Calculate the projected texture coordinates.
	projectTexCoord.x = lightViewPosition.x / lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -lightViewPosition.y / lightViewPosition.w / 2.0f + 0.5f;

	if((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		depth = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord);
		depthValue = depth.z;
		// Calculate the depth of the light.
		lightDepthValue = lightViewPosition.z / lightViewPosition.w;

		// Subtract the bias from the lightDepthValue.
		lightDepthValue = lightDepthValue - bias;
        
		// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
		// If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
        
        light = lightDepthValue < depthValue;
	}

	return light;
}

//float4 main(PixelInputType input) : SV_TARGET
float4 main(in float4 screenPos : SV_position ) : SV_Target0
{
    float4 textureColor = float4(0.0f,0.0f,0.0f,0.0f);
    float3 lightDir;
    float lightIntensity;
    float4 color = float4(0.0f,0.0f,0.0f,1.0f);
	float3 reflection;
	float attConst;
	float attLinear;
	float attenuation = 1.0f;
	float fallOffDistance;
	float4 emissive = float4(0.0f,0.0f,0.0f,1.0f);
	float4 ambient = float4(0.0f,0.0f,0.0f,1.0f);
	float4 diffuse = float4(0.0f,0.0f,0.0f,1.0f);  
	float4 specular = float4(0.0f,0.0f,0.0f,1.0f);
	float bias = 0.001f;
	float2 projectTexCoord;
	float lightDepthValue;
	float4 lightViewPosition;
	float4 worldPos = float4(0.0f,0.0f,0.0f,1.0f);
	float4 normal;
	float4 viewDirection;
	float4 depth;
	
	

	attConst = 1.0f;
	attLinear = 0.5f;
	fallOffDistance = 15.0f;

	emissive = 0.0f/*Ke*/;

	ambient = /*Ka **/ lightAmbientColor;
	lightIntensity = 1.0f;

    
    
    /*float4 screenPos = float4(0.0f,0.0f,0.0f,0.0f);

    screenPos.x = input.position.x + 0.5f;
    screenPos.y = input.position.y + 0.5f;*/

    screenPos.x = screenPos.x / (float)screenWidth;
    screenPos.y = screenPos.y / (float)screenHeight;

    //ambient = float4(screenPos.x,screenPos.y,1.0f,1.0f);

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    
    textureColor = colorTexture.Sample(SampleTypePoint, screenPos.xy);
    //textureColor = textureColor + float4(0.5f,0.5f,0.5f,1.0f);
	//Sample normal
	normal = normalTexture.Sample(SampleTypePoint,screenPos.xy);

	//Sample position
	worldPos = positionTexture.Sample(SampleTypePoint,screenPos.xy);

	//Sample depth and lightViewPosition
	depth = depthMapTexture.Sample(SampleTypePoint,screenPos.xy);

	//Get the light view position
	lightViewPosition.x = depth.x;
	lightViewPosition.y = depth.y;
	lightViewPosition.z = depth.w;
	lightViewPosition.w = worldPos.w;

	// Calculate the projected texture coordinates.
	projectTexCoord.x = lightViewPosition.x / lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -lightViewPosition.y / lightViewPosition.w / 2.0f + 0.5f;

	depth = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord);
	float depthValue = depth.z;

	float diffuseLight = 0.0f;
	float specularLight = 0.0f;
	float4 dist = float4(0.0f,0.0f,0.0f,1.0f);
	

	if( hasPosition == 0 )
	{
		if( LightPixel(lightViewPosition) )
		{
			diffuseLight = DiffuseDirectionalLight(normal,lightDirection);
			//specularLight = SpecularDirectionalLight(normal,lightDirection,diffuseLight,specularPower,viewDirection);
		}
		else
		{
			
		}
	}
	else
	{
		
		float3 pointLightDirection = lightPos - worldPos.xyz;
		float dist2 = length(pointLightDirection);
		pointLightDirection = normalize(pointLightDirection);
		attenuation = 1.0f - saturate(dist2/fallOffDistance);
		
		float dotL = dot(normal.xyz, pointLightDirection);
		diffuseLight = saturate(dotL);
	}
	
	// Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
	/*if((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		// Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.
		float depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;
		
		// Calculate the depth of the light.
		lightDepthValue = lightViewPosition.z / lightViewPosition.w;

		// Subtract the bias from the lightDepthValue.
		lightDepthValue = lightDepthValue - bias;

		// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
		// If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
		if(lightDepthValue < depthValue)
		{
			// Calculate the amount of light on this pixel.
			if( hasPosition == 1 )
			{
				float dist2 = distance(lightPos,worldPos.xyz);
				attenuation = 1.0f - saturate(dist2/fallOffDistance);
				dist.xyz = normalize(lightPos - worldPos.xyz);
				diffuseLight = saturate(max(dot(normal.xyz, dist.xyz),0));
			}
			else
			{
				diffuseLight = DiffuseDirectionalLight(normal,lightDirection);
			}
    
			diffuse = Kd * lightDiffuseColor * diffuseLight;//diffuse = Kd * diffuseColor * diffuseLight;

			if(diffuseLight > 0.0f)
			{
		
				if( specularPower > 0.0f && !hasPosition )
				{
					// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
					reflection = normalize(2 * diffuseLight * normal.xyz - lightDir);

					// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
					specularLight = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
				}
				else if( specularPower > 0.0f )
				{
					float3 V;
					V.xyz = normalize(input.viewDirection - input.worldPos);
					float3 H;
					H = normalize(lightDir + V);
					specularLight = pow(max(dot(normal.xyz,H),0),specularPower);
				}
		

			}
			else
			{
				specularLight = 0.0f;
			}
		}
		
	}*/
	
	diffuse = /*Kd **/ lightDiffuseColor * diffuseLight;
    specular = /*Ks**/lightSpecularColor*specularLight;

    // Multiply the texture pixel and the final color to get the final pixel color result.
	//don't use the attenuation on the ambient light
	color = (emissive + diffuse) * attenuation;
	color = color + ambient*attenuation;
    color = color * textureColor;
	color = saturate(color + (specular * attenuation));
    

	//color = saturate(depthValue);

	color.w = 1.0f;
    return color;
}


