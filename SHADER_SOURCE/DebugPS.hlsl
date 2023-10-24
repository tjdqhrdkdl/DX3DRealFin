#include "globals.hlsli"

float4 main(float4 position : SV_Position) : SV_TARGET
{
	if (g_collisionCount != 0)
		return float4(1.f, 0.f, 0.f, 1.f);
    
	if (g_bTrigger != 0)
		return float4(0.f, 0.f, 1.f, 1.f);
    
	return float4(0.f, 1.f, 0.f, 1.f);
}