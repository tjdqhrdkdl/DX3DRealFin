
struct Particle
{
    float4 position;
    float4 direction;
    float4 velocity;
    
    float lifeTime;
    float time;
    float speed;
    uint active;
    bool bJump;
};

struct ParticleShared
{
    uint gActiveCount;
};

