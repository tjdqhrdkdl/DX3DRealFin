#include "globals.hlsli"

struct VSIn
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float3 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};


VSOut main(VSIn In)
{
    VSOut output;

    float4 worldPosition = mul(In.Pos, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 ProjPosition = mul(viewPosition, projection);

    output.WorldPos = worldPosition;
    output.Pos = ProjPosition;
    output.Color = In.Color;


    float2 uv = In.UV;
    float barSize = 0.5;
    float barStart = 0.0;
    float barEnd = 0.5;

	barStart += PostureMeter;
	barEnd += PostureMeter;

	if (uv.x < barStart || uv.x > barEnd)
	{
		uv.x = clamp(uv.x, barStart, barEnd);
	}
	else if (uv.x >= barStart && uv.x <= barStart + barSize)
	{
		uv.x = lerp(barStart, barEnd, (uv.x - barStart) / barSize);
	}
	else if (uv.x >= barEnd - barSize && uv.x <= barEnd)
	{
		uv.x = lerp(barStart, barEnd, (uv.x - (barEnd - barSize)) / barSize);
	}

	output.UV = uv;
   

    return output;
}