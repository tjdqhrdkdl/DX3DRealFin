#include "globals.hlsli"

RWStructuredBuffer<Particle> ParticleBuffer : register(u0);
RWStructuredBuffer<ParticleShared> ParticleSharedBuffer : register(u1);

//각각의 스레드가 동기화할 데이터가 필요하다

//1024개가 최대 스레드 개수
[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if ( maxParticles <= DTid.x )
        return;
    
    if (ParticleBuffer[DTid.x].active == 0)
    {
        if (ParticleBuffer[DTid.x].bJump == false)
        {
            while (0 < ParticleSharedBuffer[0].gActiveCount)
            {
                int originValue = ParticleSharedBuffer[0].gActiveCount;
                int exchange = originValue - 1;
            
            // 스레드 동기화
            // dest값을 exchange값으로 바꾸는 동안
            // 다른스레드는 멈춘다.
            //InterlockedExchange(ParticleSharedBuffer[0].gActiveCount, exchange, exchange);
                InterlockedCompareExchange(ParticleSharedBuffer[0].gActiveCount
                                        , originValue, exchange, exchange);
            
                if (originValue == exchange)
                {
                    ParticleBuffer[DTid.x].active = 1;
                    break;
                }
            }
       
            if (ParticleBuffer[DTid.x].active)
            {
                // 랜덤값으로 위치와 방향을 설정해준다.
                // 샘플링을 시도할 UV 계산해준다.
                float4 Random = (float4) 0.0f;
                float2 UV = float2((float) DTid.x / maxParticles, 0.5f);
                UV.x += elapsedTime;
                UV.y += sin((UV.x + elapsedTime) * 3.14592f + 2.0f * 10.0f) * 0.5f;
                
                Random = float4
                (
                    GaussianBlur(UV + float2(0.0f, 0.0f)).x
                    , GaussianBlur(UV + float2(0.1f, 0.0f)).x
                    , GaussianBlur(UV + float2(0.2f, 0.0f)).x
                    , GaussianBlur(UV + float2(0.2f, 0.0f)).x
                );

            //// radius 원형 범위로 스폰
                float fTheta = Random.xy * 3.141592f * 2.0f;
                ParticleBuffer[DTid.x].position.xy = float2(0, 0);
                ParticleBuffer[DTid.x].position.z = 0.0f;
            
            
                if (simulationSpace) // 1 world , 0 local
                {
                    ParticleBuffer[DTid.x].position.xyz += worldPosition.xyz;
                }
            
                ParticleBuffer[DTid.x].velocity = ParticleBuffer[DTid.x].direction;
            
            ////파티클 속력
                ParticleBuffer[DTid.x].time = 0.0f;
                ParticleBuffer[DTid.x].speed = startSpeed;
            
            }
        }
        else
            ParticleBuffer[DTid.x].bJump = false;

    }
    else // active == 1
    {
        uint i = DTid.x % 4;
        float delay = 0.02f;
        bool isActive = true;
        if(i != 0)
        {
            isActive = false;
            if (ParticleBuffer[DTid.x / 4 * 4].time > i * delay)
            {
                isActive = true;
            }
        }
        
        if(isActive)
        {
        
            ParticleBuffer[DTid.x].time += deltaTime;
            if (ParticleBuffer[DTid.x].lifeTime < ParticleBuffer[DTid.x].time)
            {
                ParticleBuffer[DTid.x].active = 0;
            }
            else
            {
            //이동
                ParticleBuffer[DTid.x].position 
            += ParticleBuffer[DTid.x].velocity * deltaTime;
            
            //중력
                if (ParticleBuffer[DTid.x].velocity.y > -10)
                    ParticleBuffer[DTid.x].velocity += float4(0, -10, 0, 0) * deltaTime;
            
            //마찰력에 의해 힘을 잃다
                float4 friction = -ParticleBuffer[DTid.x].velocity;
                normalize(friction);
                friction = friction * deltaTime;

            
			//마찰력으로 인한 속도 감소량이 현재 속도보다 더 큰 경우
                if (length(ParticleBuffer[DTid.x].velocity) < length(friction))
				//속도를 0 로 만든다.
                    ParticleBuffer[DTid.x].velocity = float4(0, 0, 0, 0);
                else
				//속도에서 마찰력으로 인한 반대방향으로 속도를 차감한다.
                    ParticleBuffer[DTid.x].velocity += friction;
            }
        }
    }
}