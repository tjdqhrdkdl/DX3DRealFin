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
			//��ġ ����
			mPlayerPos = mPlayerObject->GetComponent<Transform>()->GetPosition();
			Vec3 monsterPos = GetComponent<Transform>()->GetPosition();

			mMonster2PlayerNormalize = mPlayerPos - monsterPos;
			mMonster2PlayerNormalize.Normalize();

			mPlayer2MonsterNormalize = monsterPos - mPlayerPos;
			mPlayer2MonsterNormalize.Normalize();						

			//�յ� �Ǻ�
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

			// �ؽ�Ʈ ���
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"���� �λ� ī��Ʈ: %.2f", MonsterAlertnessCount);
				TextOut(application.GetHdc(), 200, 200, szFloat, wcslen(szFloat));
			}

#pragma region ���� ���º� �޼��� Ȯ�ο�
			// ���� ���� �� ��� ���߿� ���� ���� �Ұ�
			if ((int)mMonsterState->GetSituation() == 0)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"���� ���� ����: None ����");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
			else if ((int)mMonsterState->GetSituation() == 1)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"���� ���� ����: ���̵� ����");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
			else if ((int)mMonsterState->GetSituation() == 2)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"���� ���� ����: ��� ����");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
			else if ((int)mMonsterState->GetSituation() == 3)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"���� ���� ����: �������·� �i�� ����");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
			else if ((int)mMonsterState->GetSituation() == 4)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"���� ���� ����: ��Ʋ ���� ��");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
			else if ((int)mMonsterState->GetSituation() == 5)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"���� ���� ����: ��������");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
			else if ((int)mMonsterState->GetSituation() == 6)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"���� ���� ����: ���");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
			else if ((int)mMonsterState->GetSituation() == 7)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"���� ���� ����: ����");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
			else if ((int)mMonsterState->GetSituation() == 8)
			{
				wchar_t szFloat[50] = {};
				swprintf_s(szFloat, 50, L"���� ���� ����: �޽�");
				TextOut(application.GetHdc(), 200, 240, szFloat, wcslen(szFloat));
			}
#pragma endregion

			
		}

	}


	//���� �ֺ����� �ÿ��̾� �ִ��� �˻�
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

	//�÷��̾� ��ġ�� ���ƺ���
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

		//�÷��̾� ��ġ �տ�����
		if (IsPlayerFront())
		{
			//�̰� ���߿� �߰��� ��� ���� ����ؼ� ���� �սô�
			if (mMonsterState->GetSituation() == enums::eSituation::Battle ||
				mMonsterState->GetSituation() == enums::eSituation::Attack ||
				mMonsterState->GetSituation() == enums::eSituation::Chase)
				return;

			//�������� �Ѿ ��Ȳ
			if (NavigationPlayer(3.0f))
			{
				mMonsterState->SetSituation(enums::eSituation::Battle);
			}

			////��� �ϸ� �ൿ �� ��Ȳ
			//else if (NavigationPlayer(4.5f))
			//{
			//	mAlertnessCount = 60.f;
			//}
			//������� �þ� �Ÿ������� (��� ���� �׾��ֱ�) 
			if (NavigationPlayer(7.0f))
			{
				SetAlertnessCount(Time::DeltaTime() * 5);
			}

		}
		//�÷��̾� ��ġ �ڿ�����
		else
		{

			//������� �þ� �Ÿ������� (��� ���� �׾��ֱ�) 
			if (NavigationPlayer(5.0f))
			{			
				SetAlertnessCount(Time::DeltaTime() * 5);
			}
		}

		//��� ���� ���� ����?
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