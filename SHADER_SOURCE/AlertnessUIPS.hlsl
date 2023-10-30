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

//cbuffer time
//int 0

float4 main(VSOut In) : SV_Target
{
    float4 color = (float) 0.0f;
    
    

    color = albedoTexture.Sample(anisotropicSampler, In.UV);
    color.a = color.r;
    
    // int_0 바운더리면 0 , 게이지면 1
    
    if (int_1 > 0.5)
        color.rgb = float3(1.0f, 0.1f, 0);
    else
    {
        if (int_0 == 0)
        {
    
            color.rgb = float3(1.0f, 1.0f, 1.0f);
            if (float_0 > 0.999)
                color.rgb = float3(0.8f, 0.7f, 0.2f);
        }
        else
        {
            color.rgb = float3(0.8f, 0.7f, 0.2f);
            if (In.UV.y < 1 - float_0)
                color.a = 0;
        }
    }

    //int_1 인지면 1 , 아니면 0
    
    // float_0 0 ~ 1 경계도

        return color;
    }