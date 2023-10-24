#ifndef PHYSX_DEBUG
#define PHYSX_DEBUG
#include "globals.hlsli"

struct VSIn
{
	float4 positionLocal : POSITION;
	float4 color : COLOR;
	float2 UV : TEXCOORD;
	float3 Tangent : TANGENT;
	float3 Normal : NORMAL;
	float3 BiNormal : BINORMAL;
    
	float4 Weights : BLENDWEIGHT;
	float4 Indices : BLENDINDICES;
};

struct VSOut
{
	float4 positionHomogeneous : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float3 uv : TEXCOORD;
};

#endif //PHYSX_DEBUG