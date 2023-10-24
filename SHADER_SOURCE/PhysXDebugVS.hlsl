#include "PhysXDebug.hlsli"

VSOut main(VSIn input)
{
	
	VSOut output;
	output.positionHomogeneous = mul(input.positionLocal, world);
	output.positionHomogeneous = mul(output.positionHomogeneous, view);
	output.positionHomogeneous = mul(output.positionHomogeneous, projection);
	output.color = input.color;

	return output;
}