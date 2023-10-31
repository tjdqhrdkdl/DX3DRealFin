#pragma once
#include "yaBaseRenderer.h"
#include "yaParticleShader.h"


namespace ya::graphics
{

	enum class eSimulationSpace
	{
		Local,
		World,
	};

	class ParticleSystem : public BaseRenderer, ComponentType<ParticleSystem>
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void ParticleOn() { mbParticleCreate = true; }
		void SetParticleNum(UINT num) { mNumParticle = num; }
	private:
		class StructedBuffer* mBuffer;
		class StructedBuffer* mSharedBuffer;

		std::shared_ptr<graphics::ParticleShader> mCS;
		renderer::ParticleSystemCB mCBData;


		Vector4 mStartSize;
		Vector4 mStartColor;
		
		eSimulationSpace mSimulationSpace;
		UINT mMaxParticles;
		float mStartLifeTime;
		float mFrequency;
		float mRadius;

		float mStartSpeed;
		float mTime;
		float mElapsedTime; //누적시간

		bool mbParticleCreate;

		UINT mNumParticle;

	};
}
