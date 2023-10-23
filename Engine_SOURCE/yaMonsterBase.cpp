#include "yaMonsterBase.h"

#include <iostream>

extern ya::Application application;




#include "yaPlayer.h"


namespace ya
{
	MonsterBase::MonsterBase()
		: mPlayerObject(nullptr)
		, mMonsterState(nullptr)
		, mSituation(eSituation::None)
		/*, mPlayerPos(Vec3::Zero)
		, mMonster2PlayerNormalize(Vec3::Zero)
		, mPlayer2MonsterNormalize(Vec3::Zero)*/
		//, mRecoveryTime(3.0f)
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
			if (!mbRecognize)
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
					// 각도
					Quaternion quater = Quaternion::FromToRotation(-playerTr->Forward(), Vector3(playerPos.x - pos.x, playerPos.y - pos.y, playerPos.z - pos.z));
					Vector3 quaterToEuler = quater.ToEuler();
					Vector3 theta = quaterToEuler * 180.0f / XM_PI;


					if (dist <= 5.0f)
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

		///깜빡이 녀석 거슬리면 주석
//		if (mMonsterState)
//		{			
//#pragma region MonsterState
//			
//			//int MonsterState;
//			float MonsterDeathBlowCount = GetDeathBlowCount();
//			float MonsterDeathBlow = IsDeathBlow();
//			float MonsterHP = GetHP();
//
//
//			{
//				wchar_t szFloat[50] = {};
//				swprintf_s(szFloat, 50, L"MonsterDeathBlowCount: %.2f", MonsterDeathBlowCount);
//				TextOut(application.GetHdc(), 200, 200, szFloat, wcslen(szFloat));
//			}
//
//			{
//				wchar_t szFloat[50] = {};
//				if (MonsterDeathBlow)
//				{
//					swprintf_s(szFloat, 50, L"MonsterDeathBlowCount: TRUE");
//					TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
//				}
//				else
//				{
//					swprintf_s(szFloat, 50, L"MonsterDeathBlowCount: FALSE");
//					TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
//				}
//			}
//
//			{
//				wchar_t szFloat[50] = {};
//				swprintf_s(szFloat, 50, L"MonsterHP: %.2f", MonsterHP);
//				TextOut(application.GetHdc(), 200, 280, szFloat, wcslen(szFloat));
//			}
//
//
//			if ((int)mMonsterState->GetSituation() == 0)
//			{
//				wchar_t szFloat[50] = {};
//				swprintf_s(szFloat, 50, L"None");
//				TextOut(application.GetHdc(), 200, 320, szFloat, wcslen(szFloat));
//			}
//			else if ((int)mMonsterState->GetSituation() == 1)
//			{
//				wchar_t szFloat[50] = {};
//				swprintf_s(szFloat, 50, L"Idle");
//				TextOut(application.GetHdc(), 200, 320, szFloat, wcslen(szFloat));
//			}
//			else if ((int)mMonsterState->GetSituation() == 2)
//			{
//				wchar_t szFloat[50] = {};
//				swprintf_s(szFloat, 50, L"Boundary");
//				TextOut(application.GetHdc(), 200, 320, szFloat, wcslen(szFloat));
//			}
//			else if ((int)mMonsterState->GetSituation() == 3)
//			{
//				wchar_t szFloat[50] = {};
//				swprintf_s(szFloat, 50, L"Chase");
//				TextOut(application.GetHdc(), 200, 320, szFloat, wcslen(szFloat));
//			}
//			else if ((int)mMonsterState->GetSituation() == 4)
//			{
//				wchar_t szFloat[50] = {};
//				swprintf_s(szFloat, 50, L"Battle");
//				TextOut(application.GetHdc(), 200, 320, szFloat, wcslen(szFloat));
//			}
//			else if ((int)mMonsterState->GetSituation() == 5)
//			{
//				wchar_t szFloat[50] = {};
//				swprintf_s(szFloat, 50, L"Run");
//				TextOut(application.GetHdc(), 200, 320, szFloat, wcslen(szFloat));
//			}
//			else if ((int)mMonsterState->GetSituation() == 6)
//			{
//				wchar_t szFloat[50] = {};
//				swprintf_s(szFloat, 50, L"Defense");
//				TextOut(application.GetHdc(), 200, 320, szFloat, wcslen(szFloat));
//			}
//			else if ((int)mMonsterState->GetSituation() == 7)
//			{
//				wchar_t szFloat[50] = {};
//				swprintf_s(szFloat, 50, L"Attack");
//				TextOut(application.GetHdc(), 200, 320, szFloat, wcslen(szFloat));
//			}
//			else if ((int)mMonsterState->GetSituation() == 8)
//			{
//				wchar_t szFloat[50] = {};
//				swprintf_s(szFloat, 50, L"Sit");
//				TextOut(application.GetHdc(), 200, 320, szFloat, wcslen(szFloat));
//			}
//#pragma endregion			
//		}

	}



	

	bool MonsterBase::NavigationPlayer(float range)
	{
		Vec3 monsterPos = GetComponent<Transform>()->GetPosition();
		Vec3 monsterScale = GetComponent<Transform>()->GetScale();


		if ((monsterPos.x - (monsterScale.x * range) <= mPlayerPos.x &&
			monsterPos.x + (monsterScale.x * range) >= mPlayerPos.x) &&
			(monsterPos.z - (monsterScale.y * range) <= mPlayerPos.z &&
				monsterPos.z + (monsterScale.y * range) >= mPlayerPos.z))
			return true;

		return false;
	}


	void MonsterBase::TurnToPlayer()
	{
		Transform* tr = GetComponent<Transform>();
		Vec3 rot = tr->GetRotation();

		Quaternion quater = Quaternion::FromToRotation
		(tr->Forward(), GetMonster2PlayerNormalize());
		Vec3 monDirection = quater.ToEuler();

		monDirection *= 180.f / XM_PI;

		tr->SetRotation(Vec3(0.0f, rot.y + monDirection.y, 0.0f));
	}

	float MonsterBase::TurnToPlayerDir()
	{
		Transform* tr = GetComponent<Transform>();
		Vec3 rot = tr->GetRotation();

		Quaternion quater = Quaternion::FromToRotation
		(tr->Forward(), GetMonster2PlayerNormalize());
		Vec3 monDirection = quater.ToEuler();

		monDirection *= 180.f / XM_PI;

		//tr->SetRotation(Vec3(0.0f, rot.y + monDirection.y, 0.0f));

		return monDirection.y;
	}

	Vector3 MonsterBase::TurnToPointDir(Vector3 point)
	{
		Transform* tr = GetComponent<Transform>();
		Vec3 rot = tr->GetRotation();

		
		Vector3 pointNomalize = point - tr->GetPosition();
		pointNomalize.Normalize();

		Quaternion quater = Quaternion::FromToRotation
		(tr->Forward(), pointNomalize);
		Vec3 monDirection = quater.ToEuler();

		monDirection *= 180.f / XM_PI;

		//tr->SetRotation(Vec3(0.0f, rot.y + monDirection.y, 0.0f));

		return monDirection;
	}

	bool MonsterBase::WalkToPlayer(float range, float Speed)
	{
		Transform* tr = GetComponent<Transform>();
		Rigidbody* rigi = GetComponent<Rigidbody>();
		

		if (NavigationPlayer(range))
		{
			return true;
		}
		else
		{
			TurnToPlayer();
			mActionScript->Move(tr->Forward(), Speed);
			return false;
		}

	}

	void MonsterBase::MonsterRotation(Vector3 target_point)
	{
		Transform* tr = GetComponent<Transform>();
		Vec3 rot = tr->GetRotation();

		Vec3 pos = tr->GetPosition();
		Vec3 nomalize = target_point - pos;
		nomalize.Normalize();

		Quaternion quater = Quaternion::FromToRotation
		(tr->Forward(), nomalize);
		Vec3 monDirection = quater.ToEuler();

		monDirection *= 180.f / XM_PI;

		tr->SetRotation(Vec3(0.0f, rot.y + monDirection.y, 0.0f));
	}



	void MonsterBase::AlertnessLevel()
	{
		if (IsPlayerFront())
		{
			if (mSituation == enums::eSituation::Battle ||
				mSituation == enums::eSituation::Attack ||
				mSituation == enums::eSituation::Chase)
				return;


			if (NavigationPlayer(3.0f))
			{
				mSituation = enums::eSituation::Battle;
			}

			//else if (NavigationPlayer(4.5f))
			//{
			//	mAlertnessCount = 60.f;
			//}

			if (NavigationPlayer(7.0f))
			{
				mAlertnessCount = Time::DeltaTime() * 5;
			}
		}
		else
		{
			if (NavigationPlayer(5.0f))
			{
				mAlertnessCount = Time::DeltaTime() * 5;
			}
		}

		if (mAlertnessCount > 80.f)
		{
			mSituation = eSituation::Attack;
		}
		else if (mAlertnessCount > 60.f)
		{
			TurnToPlayer();
			mSituation = eSituation::Boundary;
		}
	}

	bool MonsterBase::IsPlayerFieldview(float minangle, float maxangle)
	{
		float dir = atan2(mMonster2PlayerNormalize.z, mMonster2PlayerNormalize.x);
		float angle = (float)((dir * 180) / XM_PI);
		if (angle <= maxangle && angle > minangle)
			return true;
		else
			return false;
	}

	/*int MonsterBase::RandomNumber(int ieast, int Max)
	{
		int result = 0;
		if (Max - ieast + 1 == 0)
			return 1;
		result = (rand() % (Max - ieast + 1)) + ieast;


		return result;
	}*/

	void MonsterBase::OnceAniamtion(const std::wstring& animation)
	{
		if (mbOnceAnimation)
		{
			mMeshData->Play(animation);
			mbOnceAnimation = false;
		}
	}

	/*
	Vector3 MonsterBase::Convert3DTo2DScreenPos(Transform* tr)
	{
		Matrix viewMatrix;         // 뷰 매트릭스   (카메라의 위치와 방향 정보)
		Matrix worldMatrix;         // 월드 매트릭스 (몬스터의 위치 정보가 담긴 매트릭스)
		Matrix projectionMatrix;   // 3D 공간을 2D 화면으로 투영하는데 사용 하는 설정(가로,세로,비율,시야각)

		UINT width = application.GetWidth();
		UINT height = application.GetHeight();


		Transform* cameratr = mainCamera->GetOwner()->GetComponent<Transform>();


		worldMatrix = Matrix::CreateWorld(tr->GetPosition(), tr->Forward(), tr->Up());
		
		viewMatrix = Camera::GetGpuViewMatrix();
		projectionMatrix = Camera::GetGpuProjectionMatrix();

		//viewMatrix = mainCamera->GetViewMatrix();
		//projectionMatrix = mainCamera->GetProjectionMatrix();


		Matrix worldViewProjection = worldMatrix * viewMatrix * projectionMatrix;
		Vector3 MonsterNDCPos = Vector3::Transform(tr->GetPosition(), worldViewProjection);

		Vector3 monsterScreenPosition;

		//monsterScreenPosition.x = (MonsterNDCPos.x + 1.0f) * 0.5f * (float)width;
		//monsterScreenPosition.y = (1.0f - MonsterNDCPos.y) * 0.5f * (float)height;
		//monsterScreenPosition.z = 1.0f;

		float LeftMax = -800;
		float RightMax = 800;
		float TopMax = -450;
		float BottomMax = 450;

		monsterScreenPosition.x = (MonsterNDCPos.x + 1.0f) * 0.5f * (RightMax - LeftMax) + LeftMax;
		monsterScreenPosition.y = (1.0f - MonsterNDCPos.y) * 0.5f * (TopMax - BottomMax) + BottomMax;


		return monsterScreenPosition;
	}
	*/

	/// Monster 인살 이벤트
	void MonsterBase::DeathBlow()
	{
		// 각자 override해서 인살 이벤트 넣어주시면 되겠습니다

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
		mDeathBlowMark = object::Instantiate<GameObject>(eLayerType::MonsterProjectile);
		mDeathBlowMark->SetName(L"DeathBlowMark");

		Transform* marktr = mDeathBlowMark->GetComponent<Transform>();
		marktr->SetPosition(Vector3(1000.0f, 1000.0f, 0.0f));
		//marktr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		marktr->SetScale(Vector3(0.75f, 0.75f, 0.75f));
		MeshRenderer* faceRenderer = mDeathBlowMark->AddComponent<MeshRenderer>();
		faceRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));		
		std::shared_ptr<Material> mat = Resources::Find<Material>(L"SpriteMaterial");

		faceRenderer->SetMaterial(mat, 0);
		mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"DeathBlowTexture"));
	

	}

}