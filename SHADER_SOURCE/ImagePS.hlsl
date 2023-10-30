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

float4 main(VSOut In) : SV_Target
{
    float4 color = (float) 0.0f;

    if (animationType == 1) // 2D
    {
        float2 diff = (atlasSize - spriteSize) / 2.0f;
        float2 UV = (leftTop - diff - offset) + (atlasSize * In.UV);

        if (UV.x < leftTop.x || UV.y < leftTop.y
            || UV.x > leftTop.x + spriteSize.x
            || UV.y > leftTop.y + spriteSize.y)
            discard;

        color = atlasTexture.Sample(anisotropicSampler, UV);
    }
    else
    {
        color = albedoTexture.Sample(anisotropicSampler, In.UV);
    }

    if (color.a <= 0.0f)
        discard;

    LightColor lightColor = (LightColor) 0.0f;
    for (int i = 0; i < numberOfLight; i++)
    {
        CalculateLight(lightColor, In.WorldPos.xyz, i);
    }
    color *= lightColor.diffuse;
    
    if (useRange == 1 && (In.UV.x < imageRange.x || In.UV.x > imageRange.z || In.UV.y < imageRange.y || In.UV.y > imageRange.w))
       discard;
   
    if (imageAlpha == 0.0f)
    {
        color.a = 0.0f;
    }
    else if(imageAlpha == 1.0f)
    {
    }
    else
    {
        color.a *= imageAlpha;
        //if (imageAlphaType == 1)
        //    color.a *= imageAlpha;
        //else if (imageAlphaType == 2)
        //    color.a = imageAlpha;
    }
    
    if (colorType == 1)
    {
        if (color.a > 0.0f)
            color = imageColor;
    }
    else if (colorType == 2)
    {
        color.r += imageColor.x * imageColor.w;
        color.g += imageColor.y * imageColor.w;
        color.b += imageColor.z * imageColor.w;
    }
    
        return color;
    }