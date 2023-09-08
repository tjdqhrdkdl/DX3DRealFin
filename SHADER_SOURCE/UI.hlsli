#ifndef UI_HLSL
#define UI_HLSL
#include "ConstantBuffer.hlsli"
#include "Resource.hlsli"
#include "Sampler.hlsli"

struct tUIInfo
{
	int TextureSlot;
	
	int3 Padding;
	
	float2 UVBegin;
	float2 UVEnd;
};
StructuredBuffer<tUIInfo> UISBuffer : register(t20);

float4 UISample(in tUIInfo _info)
{	
	float4 color = (float4) 0;
	
	if (0 == _info.TextureSlot && 1u == usedAlbedo)
	{
		//albedoTexture.Sample(anisotropicSampler, )
	}
	
	
	return color;
}

#endif