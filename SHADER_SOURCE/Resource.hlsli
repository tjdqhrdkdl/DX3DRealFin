// Material Default Texture
Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D specularTexture : register(t8);

// defferd
Texture2D positionTarget : register(t2);
Texture2D normalTarget : register(t3);
Texture2D albedoTarget : register(t4);
Texture2D specularTarget : register(t5);

Texture2D diffuseLightTarget : register(t6);
Texture2D specularLightTarget : register(t7);
//Atlas texture
Texture2D atlasTexture : register(t12);
TextureCube skyTexture : register(t11);
//Texture2D skySphererTexture : register(t11);

// Light
//StructuredBuffer<LightAttribute> lightAttributes : register(t13);
//StructuredBuffer<LightAttribute> lightAttributes3D : register(t14);

// Particle
//StructuredBuffer<Particle> particleBuffer : register(t15);
//RWStructuredBuffer<Particle> ParticleBuffer : register(u0);
//RWStructuredBuffer<ParticleShared> ParticleSharedBuffer : register(u1);

// noise
//Texture2D NoiseTexture : register(t16);

// postProcess
Texture2D postProcessTexture : register(t60);
Texture2D guiGameTexture : register(t61);