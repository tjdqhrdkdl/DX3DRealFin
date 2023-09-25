#include "yaTenzen.h"
#include "yaMeshData.h"

//test
#include "yaInput.h"
#include "yaCollider2D.h"
#include "yaBoneAnimator.h"
#include "yaBoneCollider.h"
#include "yaActionScript.h"
#include "yaPlayer.h"

#define STATE_HAVE(STATE) (mState & STATE) == STATE
#define ADD_STATE(STATE) mState |= STATE
#define RM_STATE(STATE) mState &= ~STATE

#define BEFORE_STATE_HAVE(STATE) (mBeforeState & STATE) == STATE

namespace ya
{
	float eyeSightAngleCos = 0.2;
	Tenzen::Tenzen()
		:mState(0)
		,mAlertTime(10)
	{
	}
	Tenzen::~Tenzen()
	{
	}
	void Tenzen::Initialize()
	{
		//안쓰는 애들
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000401.fbx", L"SitIdle");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000402.fbx", L"SitToGetUp");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000410.fbx", L"ArmCrossStart");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000411.fbx", L"ArmCrossIdle");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000412.fbx", L"ArmCrossEnd");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000700.fbx", L"Surprised");
		//3천번대 공격
		//5천번대 이동
		//8천번대 피격
		// 8400~1 공격자세 방어성공
		// 8500~3 방어자세 방어성공
		// 이후 전부 방어 및 무너짐관련
		// 
		//1만번대 사망
		//500000 방어ㅁ자세
		// 501400 방어자세 해제
		// 505000~3 방어자세 이동
		//  505400~3 방어자세 회전
		SetName(L"TenzenObject");
		//mMeshData = MeshData::LoadFromFbx(L"Monster\\Boss_tenzen\\Mesh\\c1020.fbx");
		//
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000000.fbx", L"Idle");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000600.fbx", L"LookAround");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_001040.fbx", L"DrawSword");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003000.fbx", L"SwordAttack_1"); // 칼을 우상단에서 우하단으로 크게 휘두르고 제자리로.
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003001.fbx", L"SwordAttack_2"); // 칼을 좌하단에서 우상단으로. 한걸음 내딛으며.
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003003.fbx", L"SwordAttack_3"); // 전진 점프 하며, 칼을 우상단에서 좌하단으로. 한걸음 내딛으며.
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003004.fbx", L"SwordAttack_4"); // 못막는 공격, 하단 베기
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003005.fbx", L"SwordAttack_5"); // 못막는 공격, 찌르기. 전진
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003006.fbx", L"SwordAttack_6"); // 양옆으로 휘두르기, 2회 연속공격, 2회전진
		// 
		// 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_005000.fbx", L"WalkNoSword"); 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_405000.fbx", L"WalkWithSword");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_405010.fbx", L"RunWithSword");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_500000.fbx", L"Defense");
		//mMeshData->AnimationSave(L"Monster\\Boss_tenzen\\AnimationData\\tenzen.animationdata");

		////fbx 로드
		mMeshData = std::make_shared<MeshData>();
		mMeshData->Load(L"Monster\\Boss_tenzen\\Mesh\\c1020.fbx");
		mMeshData->AnimationLoad(L"Monster\\Boss_tenzen\\AnimationData\\tenzen.animationdata");
		MeshObject* object = mMeshData->Instantiate(eLayerType::Monster);


		//칼과 손잡이 찾아두기
		std::vector<GameObject*> childObjects = mMeshData->GetChildObjects();
		std::vector<std::vector<std::shared_ptr<Material>>> materials = mMeshData->GetMaterialsVec();
		for (size_t i = 0; i < materials.size(); i++)
		{
			if (materials[i][0]->GetName() == L"#04#")
				mKatanaObjectTr = childObjects[i]->GetComponent<Transform>();
			if (materials[i][0]->GetName() == L"#06#")
				mKatanaHandleObjectTr = childObjects[i]->GetComponent<Transform>();
		}
		if(mKatanaObjectTr)
			mKatanaObjectTr->SetScale(Vector3(0, 0, 0));
		
		



		//오브젝트 트랜스폼
		Transform* tr = GetComponent<Transform>();
		tr->SetPosition(Vector3(0, 100, 0));
		tr->SetScale(Vector3(1, 1, 1));
		mTransform = tr;

		//메시 데이터 트랜스폼
		Transform* meshTr = object->GetComponent<Transform>();
		meshTr->SetRotation(Vector3(180, 0, 0));
		meshTr->SetPosition(Vector3(0, 0, 0));
		meshTr->SetScale(Vector3(1, 1, 1));
		meshTr->SetParent(tr);


		//무기 콜라이더 추가
		//Initialize
		BoneCollider* katana =  object::Instantiate<BoneCollider>(eLayerType::MonsterProjectile);
		katana->SetMeshAndBone(mMeshData, L"R_Katana_long");

		//애니메이션 별로 오프셋과 발동 프레임 정해주기. 
		//발동 프레임은 기본이 false로 발동하고싶으면 조정해주면 된다.
		katana->SetAnimOffSet(L"SwordAttack_1",Vector3(1, 0.5, 1));
		katana->SetColliderActiveFrame(L"SwordAttack_1", 17, 24);
		katana->SetColliderActiveFrame(L"SwordAttack_2", 0, 500);
		katana->SetColliderActiveFrame(L"SwordAttack_3", 0, 500);
		katana->SetColliderActiveFrame(L"SwordAttack_4", 0, 500);
		katana->SetColliderActiveFrame(L"SwordAttack_5", 0, 500);
		katana->SetColliderActiveFrame(L"SwordAttack_6", 0, 500);

		//칼의 크기 정해주기
		katana->SetScale(Vector3(2, 0.3, 0.3));
		mKatanaCollider = katana;

		//피격 콜라이더
		mCollider = AddComponent<Collider2D>();
		mCollider->SetType(eColliderType::Box);
		mCollider->SetSize(Vector3(1, 2, 0.5));
		mCollider->SetCenter(Vector3(0, 0, 0));

		//리지드 바디 , 액션 스크립트
		AddComponent<Rigidbody>();
		mActionScript = AddComponent<ActionScript>();



		//애니메이션 이벤트 추가
		//complete event는 애니메이션이 루프를 돌때, 한번 애니메이션을 완료할때 생기는 이벤트
		//End Event 는 애니메이션이 종료 또는 Complete될 때 생기는 이벤트
		//Start Event 는 애니메이션이 다른 애니메이션으로 전환되며 새로운 애니메이션이 시작할 때 생기는 이벤트
		mMeshData->GetAnimationFrameEvent(L"DrawSword", 20) = [this]() {
			mKatanaObjectTr->SetScale(Vector3(1, 1, 1));
			mKatanaHandleObjectTr->SetScale(Vector3(0, 0, 0));
		};
		mMeshData->GetAnimationEndEvent(L"DrawSword") = std::bind(&Tenzen::DrawSwordEndEvent, this);
		mMeshData->GetAnimationFrameEvent(L"SwordAttack_1", 12) = [this]() { mActionScript->Move(mTransform->Forward(), GetSpeed()*5); };
		mMeshData->GetAnimationEndEvent(L"SwordAttack_1") = std::bind(&Tenzen::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_2") = std::bind(&Tenzen::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_3") = std::bind(&Tenzen::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_4") = std::bind(&Tenzen::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_5") = std::bind(&Tenzen::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"SwordAttack_6") = std::bind(&Tenzen::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"RunWithSword") = std::bind(&Tenzen::TraceEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"Defense") = std::bind(&Tenzen::DefenseEndEvent, this);


		BoneAnimator* animator = mMeshData->GetAnimator();

		//동일 애니메이션 반복시 보간을 하지 않고 싶은 경우. (기본은 보간을 하도록 되어있음.)
		animator->SetAnimationSelfChange(L"SwordAttack_2", false);

		//애니메이션 끝부분 이상한 것 잘라내는 시간
		animator->SetAnimationTailTime(0.1f);


		SetPlayerObject(dynamic_cast<GameObject*>(SceneManager::GetActiveScene()->GetPlayer()));
		
		//몬스터 스테이트
		CreateMonsterState();
		SetSpeed(200);


		MonsterBase::Initialize();

	}
	void Tenzen::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N_1))
			ADD_STATE(TenzenState_Alert);
		if (Input::GetKeyDown(eKeyCode::N_2))
			ADD_STATE(TenzenState_Recognize);
		if (Input::GetKeyDown(eKeyCode::N_3))
			ADD_STATE(TenzenState_Idle);


		Idle();
		Alert();
		Recognize();
		Attack();
		Defense();
		Trace();
		if (mAnimationName != L"" &&
			mAnimationName != mMeshData->GetPlayAnimationName()
			)
		{
			mMeshData->Play(mAnimationName);
		}
		MonsterBase::Update();
		mBeforeState = mState;

	}
	void Tenzen::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	void Tenzen::Render()
	{
		MonsterBase::Render();
	}
	void Tenzen::Idle()
	{
		if (STATE_HAVE(TenzenState_Idle))
		{
			mAnimationName = L"Idle";
		}
	}
	void Tenzen::Alert()
	{
		if (STATE_HAVE(TenzenState_Recognize))
		{
			SetAlertnessCount(0);
			return;
		}

		//경계게이지 쌓이는 로직
		//시야는 체크만. 시야 내부에 있을경우 거리를 측정한다.
		//거리가 어느정도 가까운가에 따라 경계게이지 축적
		if (!(STATE_HAVE(TenzenState_Alert)))
		{

			float cosTheta = EyeSightCheck();
			if (cosTheta > eyeSightAngleCos)
			{
				float dist = GetDistanceToPlayer();
				float alert = GetAlertnessCount();

				if (dist < 50)
					SetAlertnessCount(alert + 50 * Time::DeltaTime());
				else if (dist < 100)
					SetAlertnessCount(alert + 20 * Time::DeltaTime());

			}
			if (GetAlertnessCount() > 100)
			{
				mPlayerLastPosition = GetPlayerPos();
				ADD_STATE(TenzenState_Alert);
			}
		}

		//경계상태에서 플레이어 추적
		//플레이어 추적 실패 시 다시 Idle
		else if (STATE_HAVE(TenzenState_Alert))
		{
			mAnimationName = L"WalkNoSword";
			float cosTheta = EyeSightCheck();
			Vector3 dir = mPlayerLastPosition - mTransform->GetPosition();
			dir.y = 0;

			if (dir.Length() < 5)
				mAnimationName = L"LookAround";
			else
			{
				dir.Normalize();
				mActionScript->Move(dir, GetSpeed() / 3);

				//mNavMesh->RenewPath(mPlayerLastPosition);
				//mNavMesh->SetSpeed(GetSpeed() / 3);
			}
			

			RotateForwardTo(dir);
			
			
			float dist = GetDistanceToPlayer();
			mAlertTimeChecker += Time::DeltaTime();

			if (mAlertTimeChecker > mAlertTime)
			{
				RM_STATE(TenzenState_Alert);
				ADD_STATE(TenzenState_Idle);
				mAlertTimeChecker = 0;
				SetAlertnessCount(0);
			}
			if (cosTheta > eyeSightAngleCos && dist < 50)
				ADD_STATE(TenzenState_Recognize);
		}
	}
	void Tenzen::Recognize()
	{
		if (STATE_HAVE(TenzenState_Recognize))
		{
			if (!(STATE_HAVE(TenzenState_DrawSword)))
			{
				RotateForwardTo(GetMonster2PlayerNormalize());
				mAnimationName = L"DrawSword";
			}

			else if (STATE_HAVE(TenzenState_DrawSword))
			{
				if (!(STATE_HAVE(TenzenState_Attack)) && !(STATE_HAVE(TenzenState_Defense)) && !(STATE_HAVE(TenzenState_Trace)))
				{
					Vector3 pos = mTransform->GetPosition();
					Vector3 playerPos = GetPlayerPos();
					
					//플레이어 거리가 너무 멀어졌을 때
					if (Vector3::Distance(pos, playerPos) > 40)
					{
						ADD_STATE(TenzenState_Trace);
					}

					//플레이어 거리가 적당히 멀때 -> 근접형 공격 가능
					else if (Vector3::Distance(pos, playerPos) > 10)
					{
						int choice = rand() % 2;
						switch (choice)
						{
						case 0:
							ADD_STATE(TenzenState_Attack);
							break;
						}
					}

					//플레이어 거리가 가까울 때 -> 백스텝 가능
					else
					{
						int choice = rand() % 2;
						switch (choice)
						{
						case 0:
							ADD_STATE(TenzenState_Attack);
							break;
						}
					}
				}

				if (true)
				{
					RotateForwardTo(GetMonster2PlayerNormalize());
				}
			}
		}



	}
	void Tenzen::Attack()
	{
		if (STATE_HAVE(TenzenState_Attack) && !(BEFORE_STATE_HAVE(TenzenState_Attack)))
		{
			//if (curName.substr(0, 11) == L"SwordAttack")
			//	return;
			int choice = rand() % 6;
			switch (choice)
			{
			case 0:
				mAnimationName = L"SwordAttack_1";
				break;
			case 1:
				mAnimationName = L"SwordAttack_2";
				break;
			case 2:
				mAnimationName = L"SwordAttack_3";
				break;
			case 3:
				mAnimationName = L"SwordAttack_4";
				break;
			case 4:
				mAnimationName = L"SwordAttack_5";
				break;
			case 5:
				mAnimationName = L"SwordAttack_6";
				break;
			}
		}
	}
	void Tenzen::Defense()
	{
		if (STATE_HAVE(TenzenState_Defense))
		{
			mAnimationName = L"Defense";
		}
	}
	void Tenzen::Trace()
	{
		if (STATE_HAVE(TenzenState_Trace))
		{
			mAnimationName = L"RunWithSword";
			
			mActionScript->Move(mTransform->Forward(), GetSpeed());
			Vector3 pos = mTransform->GetPosition();
			Vector3 playerPos = GetPlayerPos();
			if (Vector3::Distance(pos, playerPos) < 60)
			{
				RM_STATE(TenzenState_Trace);
			}

		}
	}

	float Tenzen::EyeSightCheck()
	{
		Vector3 pos = mTransform->GetPosition();
		Transform* playerTr = GetPlayerObject()->GetComponent<Transform>();
		Vector3 playerPos = playerTr->GetPosition();

		//시야체크(y축이 동일하다고 가정)
		Vector3 posY = pos;
		Vector3 playerPosY = playerPos;
		posY.y = 0; playerPosY.y = 0;
		Vector3 dir = playerPosY - posY;
		dir.Normalize();
		return dir.Dot(mTransform->Forward());
	}

	float Tenzen::GetDistanceToPlayer()
	{
		Vector3 pos = mTransform->GetPosition();
		Transform* playerTr = GetPlayerObject()->GetComponent<Transform>();
		Vector3 playerPos = playerTr->GetPosition();

		//거리체크(y축 스케일고려)
		Vector3 scaledPos = pos;
		Vector3 scaledPlayerPos = playerPos;

		scaledPos.y -= mTransform->GetScale().y / 2;
		scaledPlayerPos.y -= playerTr->GetScale().y / 2;

		return Vector3::Distance(scaledPos, scaledPlayerPos);
	}

	void Tenzen::RotateForwardTo(Vector3 dir)
	{
		Quaternion quater = Quaternion::FromToRotation(mTransform->Forward(), dir);
		Vec3 theta = quater.ToEuler();

		theta *= 180.f / XM_PI;
		if (fabsf(theta.y) > 1)
			if (theta.y > 0)
			{
				if (theta.y > 5)
					mActionScript->Rotate(mTransform->Up(), GetSpeed() / 2);
				else
					mActionScript->Rotate(mTransform->Up(), GetSpeed() / 10);
			}
			else
			{
				if (theta.y < -5)
					mActionScript->Rotate(-mTransform->Up(), GetSpeed() / 2);
				else
					mActionScript->Rotate(-mTransform->Up(), GetSpeed() / 10);
			}
	}
	void Tenzen::DrawSwordEndEvent()
	{
		ADD_STATE(TenzenState_DrawSword);
		ADD_STATE(TenzenState_Guard);
	}
	void Tenzen::DefenseEndEvent()
	{
		RM_STATE(TenzenState_Defense);
	}
	void Tenzen::AttackEndEvent()
	{
		RM_STATE(TenzenState_Attack);
		mBeforeState &= ~TenzenState_Attack;
	}
	void Tenzen::TraceEndEvent()
	{
		RM_STATE(TenzenState_Trace);
	}
}