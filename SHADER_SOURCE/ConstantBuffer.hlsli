cbuffer Transform : register(b0)
{
    row_major matrix scale;
    row_major matrix rotation;
    row_major matrix translation;
    row_major matrix world;
    row_major matrix view;
    row_major matrix projection;
    
    bool useQuaternion;
    float4 rotQuaternion;
}

cbuffer MaterialData : register(b1)
{
    uint usedAlbedo;
    uint usedNormal;
    uint padd1;
    uint padd2;
}

cbuffer Grid : register(b2)
{
    float4 cameraPosition;
    float2 cameraScale;
    float2 resolution;
}

cbuffer Animation : register(b3)
{
    float2 leftTop;
    float2 spriteSize;
    float2 offset;
    float2 atlasSize;

    uint animationType;
}

cbuffer NumberOfLight : register(b4)
{
    uint numberOfLight;
}

cbuffer ParticleSystem : register(b5)
{
    float4 worldPosition;
    float4 startColor;
    float4 startSize;
    
    uint maxParticles;
    uint simulationSpace;
    float radius;
    float startSpeed;
    
    float startLifeTime;
    float deltaTime;
    float elapsedTime; //�����ð�
    int padding;
}

//cbuffer Noise : register(b6)
//{
//    float4 NoiseSize;
//}