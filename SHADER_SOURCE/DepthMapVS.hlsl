#include "globals.hlsli"

struct VSIn
{
    float4 Position : POSITION;
};

struct VSOut
{
    float4 Position : SV_Position;
    float4 ProjPosition : POSITION;
};



VSOut main(VSIn In)
{
    VSOut output = (VSOut) 0.0f;
    
    float4 worldPosition = mul(In.Position, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 ProjPosition = mul(viewPosition, projection);
    
    output.Position = ProjPosition;
    output.ProjPosition = ProjPosition;
    
    return output;
}