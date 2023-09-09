#ifndef UI_HLSL
#define UI_HLSL
#include "ConstantBuffer.hlsli"
#include "Resource.hlsli"
#include "Sampler.hlsli"

struct tUIRenderInfo
{
	int TextureSlot;
	float3 Padding;
	
	float2 UVBeginInTexture;
	float2 UVSizeInTexture;
	
	float2 UVBeginInCanvas;
	float2 UVSizeInCanvas;
	
	float ZValue;
	float3 Padding_UIRenderInfo;
};
StructuredBuffer<tUIRenderInfo> UISBuffer : register(t20);
#define UISBufferSize int_0

float4 UISample(in tUIRenderInfo _info)
{	
	float4 color = (float4) 0;
	
	if (0 == _info.TextureSlot && 1u == usedAlbedo)
	{
		//albedoTexture.Sample(anisotropicSampler, )
	}
	
	
	return color;
}


struct VSIn
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
	float2 UV : TEXCOORD;
};

struct VSOut
{
	float4 Pos : SV_Position;
	float4 Color : COLOR;
	float2 UV : TEXCOORD;
};

#endif