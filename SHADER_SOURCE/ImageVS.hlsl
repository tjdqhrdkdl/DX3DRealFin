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
    output.UV = In.UV;
    
    if (useRate == 1)
    {
        output.Pos.x = output.Pos.x * imageRate.x;
        output.Pos.y = output.Pos.y * imageRate.y;
    }

    return output;
}