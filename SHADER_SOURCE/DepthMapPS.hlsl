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

//struct PSOut
//{
//    float4 Position : SV_Target0;
//    float4 Normal : SV_Target1;
//    float4 Albedo : SV_Target2;
//    float4 Specular : SV_Target3;
//};

float4 main(VSOut In) : SV_Target0
{
    float4 output = float4(0.0f, 0.0f, 0.0f, 1.0f);
    output.rgb = In.ProjPosition.z / In.ProjPosition.w;
    //output.rgb *= (900 / 1600);
    
    return output;
}