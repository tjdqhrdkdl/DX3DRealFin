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
    float3 ViewPos : POSITION2;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

//cbuffer time
//int 0

float4 main(VSOut In) : SV_Target
{
    float4 color = (float) 0.0f;

    //color.a += time;

    if (animationType == 1) // 2D
    {
        float2 diff = (atlasSize - spriteSize) / 2.0f;
        float2 UV = (leftTop - diff - offset) + (atlasSize * In.UV);

        if (UV.x < leftTop.x || UV.y < leftTop.y
            || UV.x > leftTop.x + spriteSize.x
            || UV.y > leftTop.y + spriteSize.y)
            discard;

        //UV.x = -UV.x;
        color = atlasTexture.Sample(anisotropicSampler, UV);
    }
    else
    {
        //UV.x = -UV.x;
        color = albedoTexture.Sample(anisotropicSampler, In.UV);
    }

    
    if (isMain)
    {
        color.r = 1;
        color.g = 0.7;
        color.b = 0;
        color.a -= 2 * time;
    }
    else
    {
        color.r = 1;
        color.g = 0.5;
        color.b = 0;
        color.a -= 0.2 + 2 * time ;
    }
    //color.a = color.r;
    //color.r = 1;
    //color.g = 0;
    //color.b = 0;
    //color.a = 0.5f;
    //color = defaultTexture.Sample(anisotropicSampler, In.UV);
        return color;
    }