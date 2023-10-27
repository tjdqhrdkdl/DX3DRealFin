#include "yaMonsterBase.h"
#include "yaPlayer.h"

#include <iostream>

extern ya::Application application;

namespace ya
{
	MonsterBase::MonsterBase()
		: mPlayerObject(nullptr)
		, mMonsterState(nullptr)
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
		CreateMonsterState();
		mMonsterUI = object::Instantiate<MonsterUI>(eLayerType::UI, GetScene());
		mMonsterUI->SetMonster(this);
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
			if (!mbRecognize && !(STATE_HAVE(MonsterState_OnHit)))
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


			//if (IsDeathBlow())
			//{
			//	Transform* marktr = mDeathBlowMark->GetComponent<Transform>();
			//	//Vector3 pos = Convert3DTo2DScreenPos(GetComponent<Transform>());
			//	Transform* camtr = mainCamera->GetOwner()->GetComponent<Transform>();
			//	Vector3 rot = TurnToPointDir(camtr->GetPosition());
			//	
			//	marktr->SetRotation(Vec3(0.0f, rot.y, 0.0f));
			//	//marktr->SetRotation(rot);
			//	marktr->SetPosition(monsterPos + mDeathBlowMarkOffSet);
      //
			//}
			//else
			//{
			//	Transform* marktr = mDeathBlowMark->GetComponent<Transform>();
			//	marktr->SetPosition(Vector3(1000.0f, 1000.0f, 0.0f));
			//}

			

			////DeathBlowRecovery
			//if (IsDeathBlowOnOff())
			//{
			//	SetDeathBlowCount(-(Time::DeltaTime() / 2));
			//}
			//else
			//{
			//	mTime += Time::DeltaTime();
			//	if (mTime >= 3.0f)
			//	{
			//		SetDeathBlowonoff(true);
			//		mTime = 0.f;
			//	}
			//}

			////체간 게이지 차서 그로기 걸렸을때는 3초
			//if (GetSituation() == enums::eSituation::Groggy)
			//{
			//	mTime += Time::DeltaTime();
			//	if (mTime >= mRecoveryTime)
			//	{
			//		SetSituation(enums::eSituation::None);
			//		SetDeathBlow(false);
			//		mTime = 0.f;
			//	}
			//}

			//if (IsDeathBlow())
			//{
			//	Transform* marktr = mDeathBlowMark->GetComponent<Transform>();
			//	Vector3 pos = Convert3DTo2DScreenPos(GetComponent<Transform>());
			//	marktr->SetPosition(pos + mDeathBlowMarkOffSet);
			//}
			//else
			//{
			//	Transform* marktr = mDeathBlowMark->GetComponent<Transform>();
			//	marktr->SetPosition(Vector3(1000.0f, 1000.0f, 0.0f));
			//}


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


	void MonsterBase::CreateMonsterState()
	{
		if (nullptr == mMonsterState)
		{
			mMonsterState = new State();


			mMonsterState->SetSituation(enums::eSituation::None);
			mMonsterState->SetHPMax(20.0f);				//HP 총 량
			mMonsterState->SetHp(20.0f);				//



			mMonsterState->SetSpeed(5.0f);				//기본 이동속도
			mMonsterState->SetDeathBlowCount(0.f);		//현재 체간 상태
			mMonsterState->SetMaxDeathBlowCount(10.f);	//총 체간
			mMonsterState->SetDeathBlow(false);			//인살 가능한 상태

			mMonsterState->SetResurrectionCountMax(0);
			mMonsterState->SetResurrectionCount(0);
		}
	}

	void MonsterBase::CreateDeathBlowMark()
	{

		Transform* tr = GetComponent<Transform>();
		tr->SetPosition(mOriginSetting.position);
	}
}