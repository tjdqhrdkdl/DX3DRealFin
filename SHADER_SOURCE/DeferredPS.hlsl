#include "globals.hlsli"

struct VSIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Tangent : TANGENT;
    float3 Normal : NORMAL;
    float3 BiNormal : BINORMAL;
};

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    
    float3 ViewPos : POSITION;
    
    float3 ViewNormal : NORMAL;
    float3 ViewTanget : TANGENT;
    float3 ViewBiNormal : BINORMAL;
};

struct PSOut
{
    float4 Position : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Albedo : SV_Target2;
    float4 Data : SV_Target3;
};


PSOut main(VSOut In) : SV_Target
{
    PSOut OutColor;
    
    float4 objColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    if (usedAlbedo == 1)
    {
        objColor = albedoTexture.Sample(anisotropicSampler, In.UV);
    }
    
    float3 vNormal = In.ViewNormal;
    
    if (usedNormal == 1)
    {
    // ��ü�� ǥ�鿡 ����� ź��Ʈ ���� ���� ���⺤�͸� �����´�.
        vNormal = normalTexture.Sample(anisotropicSampler, In.UV);
        
    // 0~1���� -1~1�� ������ ��ȯ
        vNormal = (vNormal * 2.0f) - 1.0f;
        
        float3x3 matTBN =
        {
            In.ViewTanget,
            In.ViewBiNormal,
            In.ViewNormal,
        };

        vNormal = normalize(mul(vNormal, matTBN));
    }
    OutColor.Position = float4(In.ViewPos, 1.0f);
    OutColor.Normal = float4(vNormal, 1.0f);
    OutColor.Albedo = objColor;
    OutColor.Data = float4(1.0f, 1.0f, 1.0f, 1.0f);

    
    return OutColor;
}