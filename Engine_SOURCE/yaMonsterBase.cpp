#include "yaMonsterBase.h"
#include "yaPlayer.h"
#include "yaPlayerDangerUI.h"
#include "yaAudioClip.h"

#include <iostream>

extern ya::Application application;

namespace ya
{
	MonsterBase::MonsterBase()
		: GameObject()
		, mMeshData(nullptr)
		, mActionScript(nullptr)
		, mTransform(nullptr)
		, mCollider(nullptr)
		, mMonsterUI(nullptr)
		, mCamScript(nullptr)
		, mPlayerObject(nullptr)
		, mAttackParams{}
		, mBeforeState(0)
		, mState(0)
		, mMonsterState(nullptr)
		, mAlertnessCount(0)
		, mPlayerPos(Vector3::Zero)
		, mMonster2PlayerNormalize(Vector3::Zero)
		, mPlayer2MonsterNormalize(Vector3::Zero)
		, mbRecognize(false)
		, mbPostureRecovery(false)
		, mBeforePosture(0.f)
		, mPostureRecoveryTimeChecker(0.f)
		, mbAssaination(true)
	{
	}

	MonsterBase::~MonsterBase()
	{
		if (nullptr != mMonsterState)
		{
			delete mMonsterState;
			mMonsterState = nullptr;
		}
	}

	void MonsterBase::Initialize()
	{
		mMonsterState = new State();

		mMonsterUI = object::Instantiate<MonsterUI>(eLayerType::UI, GetScene());
		mMonsterUI->SetMonster(this);
		mBossUI = object::Instantiate<BossUI>(eLayerType::UI, GetScene());
		mBossUI->SetMonster(this);
		mCamScript = mainCamera->GetOwner()->GetScript<CameraScript>();

		GameObject::Initialize();
	}

	void MonsterBase::Update()
	{
		if (nullptr != mPlayerObject)
		{
			//Postion 
			mPlayerPos = mPlayerObject->GetComponent<Transform>()->GetPosition();

			Vec3 monsterPos = GetComponent<Transform>()->GetPosition();

			mMonster2PlayerNormalize = mPlayerPos - monsterPos;
			mMonster2PlayerNormalize.Normalize();
			
			Transform* playerTr = mPlayerObject->GetComponent<Transform>();
			Vector3 playerPos = playerTr->GetPosition();

			Transform* tr = GetComponent<Transform>();
			Vector3 pos = tr->GetPosition();

			// 거리
			float dist = playerPos.Distance(playerPos, pos);

			// 인식 안됐을때 거리 이내일 경우 인살 가능 상태가 된다
			if (!mbRecognize && !(STATE_HAVE(MonsterState_OnHit)) && mbAssaination)
			{
				if(dist < 2.0f)
				{
					SetDeathBlow(true);
					mPlayerObject->SetDeathBlowTarget(this, dist);
				}
				else
				{
					SetDeathBlow(false);
					mPlayerObject->EraseDeathBlowTarget(this);
				}
			}
			else if (mbRecognize && !(STATE_HAVE(MonsterState_Groggy)))
			{
				SetDeathBlow(false);
				mPlayerObject->EraseDeathBlowTarget(this);
			}
			else
			{
				if (IsDeathBlow())
				{
					// 플레이어가 몬스터를 바라보는 각도
					Quaternion quater = Quaternion::FromToRotation(-playerTr->Forward(), Vector3(playerPos.x - pos.x, playerPos.y - pos.y, playerPos.z - pos.z));
					Vector3 quaterToEuler = quater.ToEuler();
					Vector3 theta = quaterToEuler * 180.0f / XM_PI;

					if (dist <= 4.0f)
					{
						if (abs(theta.y) <= 60.0f)
						{
							mPlayerObject->SetDeathBlowTarget(this, dist);
						}
						else
						{
							mPlayerObject->EraseDeathBlowTarget(this);
						}
					}
					else
					{
						mPlayerObject->EraseDeathBlowTarget(this);
					}
				}
				else
				{
					mPlayerObject->EraseDeathBlowTarget(this);
				}
			}

			//체간 자연 회복
			{	
				if (mBeforePosture < GetPosture())
				{
					mbPostureRecovery = false;
				}

				if (mbPostureRecovery == false)
				{
					mPostureRecoveryTimeChecker += Time::DeltaTime();
					if (mPostureRecoveryTimeChecker > 3)
					{
						mPostureRecoveryTimeChecker = 0;
						mbPostureRecovery = true;
					}
				}
				else
				{
					float posture = GetPosture() - Time::DeltaTime() * 3;
					if (posture < 0)
						posture = 0;
					SetPosture(posture);
				}
				mBeforePosture = GetPosture(); 
			}

			if (mbRecognize)
				mbAssaination = false;

		}

		GameObject::Update();
	}

	//TEST RENDER 
	void MonsterBase::Render()
	{
		GameObject::Render();
	}

	/// Monster 인살 이벤트
	void MonsterBase::DeathBlow()
	{
		mMonsterState->AddResurrectionCount(-1);
		mMonsterState->SetPosture(0);
		mMonsterState->SetHp(mMonsterState->GetHPMax());

		mMonsterState->SetDeathBlow(false);
	}

	void MonsterBase::SetAttackUnGuardable(bool b)
	{
		mAttackParams.unGuardable = b;
		if (b)
		{
			mPlayerObject->DangerUION();
			Resources::Find<AudioClip>(L"perilous_attack")->Play();

		}

	}

	/// Monster Reset
	void MonsterBase::Reset()
	{
		// state 리셋
		mMonsterState->SetHPMax(mOriginSetting.state.GetHPMax());
		mMonsterState->SetHp(mOriginSetting.state.GetHP());

		mMonsterState->SetSpeed(mOriginSetting.state.GetSpeed());
		mMonsterState->SetDeathBlow(false);
		mMonsterState->SetPosture(mOriginSetting.state.GetPosture());

		mMonsterState->SetResurrectionCountMax(mOriginSetting.state.GetResurrectionCountMax());
		mMonsterState->SetResurrectionCount(mOriginSetting.state.GetResurrectionCount());

		// monsterState 리셋
		mState = 0;
		ADD_STATE(MonsterState_Idle);
		mMeshData->GetAnimator()->SetStop(false);
		mCollider->Active(true);
		mActionScript->SetCheckCollider(true);

		mbRecognize = false;
		mAlertnessCount = 0;

		// position 리셋
		Transform* tr = GetComponent<Transform>();
		tr->SetPosition(mOriginSetting.position);

		mMeshData->GetAnimator()->SetStop(false);
	}
	void MonsterBase::ParryEffectOn()
	{
		 mPlayerObject->ParryEffectOn(); 
	}
	GameObject* MonsterBase::GetParticleObject()
	{
		return mPlayerObject->GetParticleObject();
	}
}