#include "MonsterBase.h"




extern ya::Application application;

namespace ya

{

	MonsterBase::MonsterBase()
		: mPlayerObject(nullptr)
		, mMonsterState(nullptr)
		, mPlayerPos(Vec3::Zero)
		, mMonster2PlayerNormalize(Vec3::Zero)
		, mPlayer2MonsterNormalize(Vec3::Zero)

	{

		srand((unsigned int)time(nullptr));
		int Rand = rand();


	}

	MonsterBase::~MonsterBase()
	{
		if (nullptr != mMonsterState)
		{
			delete mMonsterState;
			mMonsterState = nullptr;
		}

	}

	void MonsterBase::Initalize()
	{


		GameObject::Initalize();
	}

	void MonsterBase::Update()
	{
		if (nullptr != mPlayerObject)
		{
			//위치 정보
			mPlayerPos = mPlayerObject->GetComponent<Transform>()->GetPosition();
			Vec3 monsterPos = GetComponent<Transform>()->GetPosition();

			mMonster2PlayerNormalize = mPlayerPos - monsterPos;
			mMonster2PlayerNormalize.Normalize();

			mPlayer2MonsterNormalize = monsterPos - mPlayerPos;
			mPlayer2MonsterNormalize.Normalize();						

			//앞뒤 판별
			Vec3 monForward = GetComponent<Transform>()->Forward();
			float direction = monForward.Dot(mMonster2PlayerNormalize);

			if (direction > 0)
			{
				mbPlayerFront = true;
			}
			else if (direction < 0)
			{
				mbPlayerFront = false;
			}

		}


		
		

		GameObject::Update();
	}



	//TEST RENDER 
	void MonsterBase::Render()
	{

		GameObject::Render();

		if(mMonsterState)
		{

			//int MonsterState;
			float MonsterAlertnessCount = GetAlertnessCount();

			// 텍스트 출력
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"몬스터 인살 카운트: %.2f", MonsterAlertnessCount);
				TextOut(application.GetHdc(), 200, 200, szFloat, wcslen(szFloat));
			}

#pragma region 몬스터 상태별 메세지 확인용
			// 몬스터 상태 별 출력 나중에 전부 삭제 할거
			if ((int)mMonsterState->GetSituation() == 0)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"몬스터 현재 상태: None 상태");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
			else if ((int)mMonsterState->GetSituation() == 1)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"몬스터 현재 상태: 아이들 상태");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
			else if ((int)mMonsterState->GetSituation() == 2)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"몬스터 현재 상태: 경계 상태");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
			else if ((int)mMonsterState->GetSituation() == 3)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"몬스터 현재 상태: 공격형태로 쫒는 상태");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
			else if ((int)mMonsterState->GetSituation() == 4)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"몬스터 현재 상태: 배틀 선택 중");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
			else if ((int)mMonsterState->GetSituation() == 5)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"몬스터 현재 상태: 도망가기");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
			else if ((int)mMonsterState->GetSituation() == 6)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"몬스터 현재 상태: 방어");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
			else if ((int)mMonsterState->GetSituation() == 7)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"몬스터 현재 상태: 공격");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
			else if ((int)mMonsterState->GetSituation() == 8)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"몬스터 현재 상태: 휴식");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
#pragma endregion

			
		}

	}


	//몬스터 주변으로 플에이어 있는지 검색
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

	//플레이어 위치로 돌아보기
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

		//플레이어 위치 앞에있음
		if (IsPlayerFront())
		{
			//이건 나중에 추가로 어떻게 할지 고민해서 변경 합시다
			if (mMonsterState->GetSituation() == enums::eSituation::Battle ||
				mMonsterState->GetSituation() == enums::eSituation::Attack ||
				mMonsterState->GetSituation() == enums::eSituation::Chase)
				return;

			//공격으로 넘어갈 상황
			if (NavigationPlayer(3.0f))
			{
				mMonsterState->SetSituation(enums::eSituation::Battle);
			}

			////경계 하며 행동 할 상황
			//else if (NavigationPlayer(4.5f))
			//{
			//	mAlertnessCount = 60.f;
			//}
			//어느정도 시야 거리에있음 (경계 레벨 쌓아주기) 
			if (NavigationPlayer(7.0f))
			{
				SetAlertnessCount(Time::DeltaTime() * 5);
			}

		}
		//플레이어 위치 뒤에있음
		else
		{

			//어느정도 시야 거리에있음 (경계 레벨 쌓아주기) 
			if (NavigationPlayer(5.0f))
			{			
				SetAlertnessCount(Time::DeltaTime() * 5);
			}
		}

		//경계 레벨 까지 같이?
		if (GetAlertnessCount() > 80.f)
		{
			mMonsterState->SetSituation(eSituation::Attack);
		}
		else if (GetAlertnessCount() > 60.f)
		{
			TurnToPlayer();
			mMonsterState->SetSituation(eSituation::Boundary);
		}

	}

	int MonsterBase::RnadomNumber(int ieast, int Max)
	{
		int result = (rand() % Max - ieast + 1) + ieast;

		return result;
	}

	void MonsterBase::CreateMonsterState()
	{
		if (nullptr == mMonsterState)
		{
			mMonsterState = new State();

			mMonsterState->SetSituation(enums::eSituation::None);
			mMonsterState->SetMaxHP(10.0f);
			mMonsterState->SetHp(10.0f);
			mMonsterState->SetSpeed(5.0f);
			mMonsterState->SetDeathBlowCount(0.f);
			mMonsterState->SetMaxDeathBlowCount(10.f);
			mMonsterState->SetAlertnessCount(0.f);
			mMonsterState->SetDeathBlow(false);
		}
		
		




	}

}