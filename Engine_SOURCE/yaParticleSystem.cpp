#include "yaParticleSystem.h"
#include "yaMesh.h"
#include "yaResources.h"
#include "yaMaterial.h"
#include "yaStructedBuffer.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaTexture.h"
#include "yaTime.h"
#include "yaInput.h"


namespace ya
{
	ParticleSystem::ParticleSystem()
		: BaseRenderer(eComponentType::ParticleSystem)
		, mMaxParticles(1020)
		, mStartSize(Vector4(.1f, .1f, 0.1f, 1.0f))
		, mStartColor(Vector4(1.0f, 0.4f, 0.f, 1.0f))
		, mStartLifeTime(1.0f)
		, mFrequency(1.0f)
		, mTime(0.0f)
		, mCBData{}
		, mSimulationSpace(eSimulationSpace::World)
		, mRadius(500.0f)
		, mStartSpeed(10.0f)
		, mElapsedTime(0.0f)
		, mNumParticle(32)
	{

	}

	ParticleSystem::~ParticleSystem()
	{
		delete mBuffer;
		mBuffer = nullptr;

		delete mSharedBuffer;
		mSharedBuffer = nullptr;
	}

	void ParticleSystem::Initialize()
	{
		mCS = Resources::Find<ParticleShader>(L"ParticleCS");

		std::shared_ptr<Mesh> point = Resources::Find<Mesh>(L"PointMesh");
		SetMesh(point);

		// Material 세팅
		std::shared_ptr<Material> material = Resources::Find<Material>(L"ParticleMaterial");
		material->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"SparkParticle"));
		SetMaterial(material, 0);
		

		Particle particles[1020] = {};

		Vector4 startPos = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		for (size_t i = 0; i < mMaxParticles; i++)
		{
			particles[i].position = Vector4(0.0f, 0.0f, 0.f, 1.0f);
			particles[i].active = 0;
			float randomAngle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * XM_2PI;

			particles[i].speed = 100.0f;

			if (i % 3 == 0)
			{
				particles[i].velocity = Vector4((float)(((float)rand() / (float)RAND_MAX) - 0.5)
					, (float)((float)(rand() / (float)RAND_MAX) - 0.5)
					, (float)((float)(rand() / (float)RAND_MAX) - 0.5), 0);
				particles[i].bGravity = false;
				particles[i].lifeTime = 0.2f;

			}
			else
			{
				particles[i].velocity = particles[i - 1].velocity;
				particles[i].bGravity = false;
				particles[i].lifeTime = 0.2f;


			}
			if (i % 9 == 0)
			{
				particles[i].velocity = Vector4((float)(((float)rand() / (float)RAND_MAX) * 20 - 10)
					, (float)((float)(rand() / (float)RAND_MAX) * 8)
					, (float)((float)(rand() / (float)RAND_MAX) * 40 - 20), 0);
				particles[i].bGravity = true;
				particles[i].lifeTime = 0.5f;


			}
			else if (i%9 == 1 || i%9 == 2)
			{
				particles[i].velocity = particles[i - 1].velocity;
				particles[i].bGravity = true;
				particles[i].lifeTime = 0.5f;


			}

			particles[i].direction = particles[i].velocity;
			particles[i].bJump = false;
			
		}

		mBuffer = new StructedBuffer();
		mBuffer->Create(sizeof(Particle), mMaxParticles, eSRVType::UAV, particles);

		mSharedBuffer = new StructedBuffer();
		mSharedBuffer->Create(sizeof(ParticleShared), 1, eSRVType::UAV, nullptr, true);
	}

	void ParticleSystem::Update()
	{
		if (Input::GetKeyDown(eKeyCode::K))
			mbParticleCreate = true;
	}

	void ParticleSystem::FixedUpdate()
	{
		//파티클 생성 시간
		float aliveTime = 1.0f / mFrequency;
		//누적시간	
		mTime += Time::DeltaTime();

		if (mbParticleCreate)
		{
			mbParticleCreate = false;
			float f = (mTime / aliveTime);
			UINT iAliveCount = (UINT)f;
			mTime = f - std::floor(f);

			ParticleShared shared = { 200, };
			mSharedBuffer->SetData(&shared, 1);
		}
		else
		{
			ParticleShared shared = {  };
			mSharedBuffer->SetData(&shared, 1);
		}

		mMaxParticles = mBuffer->GetStride();
		Vector3 pos = GetOwner()->GetComponent<Transform>()->GetPosition();
		mCBData.worldPosition = Vector4(pos.x, pos.y, pos.z, 1.0f);
		mCBData.maxParticles = mMaxParticles;
		mCBData.radius = mRadius;
		mCBData.simulationSpace = (UINT)mSimulationSpace;
		mCBData.startSpeed = mStartSpeed;
		mCBData.startSize = mStartSize;
		mCBData.startColor = mStartColor;
		mCBData.startLifeTime = mStartLifeTime;
		mCBData.deltaTime = Time::DeltaTime();
		mCBData.elapsedTime += Time::DeltaTime();
		//gravity , force , velocity , friction
		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::ParticleSystem];
		cb->SetData(&mCBData);
		cb->Bind(eShaderStage::ALL);

		mCS->SetSharedStrutedBuffer(mSharedBuffer);
		mCS->SetStrcutedBuffer(mBuffer);
		mCS->OnExcute();
	}

	void ParticleSystem::Render()
	{
		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();
		mBuffer->BindSRV(eShaderStage::GS, 15);

		GetMaterial(0)->Bind();
		GetMesh()->RenderInstanced(mMaxParticles);

		mBuffer->Clear();
	}
}
