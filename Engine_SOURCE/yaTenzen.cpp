#include "yaTenzen.h"
#include "yaMeshData.h"

//test
#include "yaInput.h"
#include "yaCollider2D.h"
#include "yaBoneAnimator.h"
#include "yaBoneCollider.h"
#include "yaActionScript.h"

#define STATE_HAVE(STATE) (mState & STATE) == STATE
#define ADD_STATE(STATE) mState |= STATE
#define RM_STATE(STATE) mState &= ~STATE

#define BEFORE_STATE_HAVE(STATE) (mBeforeState & STATE) == STATE

namespace ya
{
	Tenzen::Tenzen()
		:mState(0)
	{
	}
	Tenzen::~Tenzen()
	{
	}
	void Tenzen::Initialize()
	{
		//3천번대 공격
		//5천번대 이동
		//8천번대 피격
		// 8400~1 공격자세 방어성공
		// 8500~3 방어자세 방어성공
		// 이후 전부 방어 및 무너짐관련
		// 
		//1만번대 사망
		//500000 방어자세
		// 501400 방어자세 해제
		// 505000~3 방어자세 이동
		//  505400~3 방어자세 회전
		SetName(L"TenzenObject");
		//mMeshData = MeshData::LoadFromFbx(L"Monster\\Boss_tenzen\\Mesh\\c1020.fbx");
		 
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000000.fbx", L"Idle");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000401.fbx", L"SitIdle");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000402.fbx", L"SitToGetUp");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000410.fbx", L"ArmCrossStart");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000411.fbx", L"ArmCrossIdle");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000412.fbx", L"ArmCrossEnd");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000600.fbx", L"LookAround");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_000700.fbx", L"Surprised");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_001040.fbx", L"DrawSword");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_003000.fbx", L"SwingSword1"); // 칼을 우상단에서 우하단으로 크게 휘두르고 제자리로.
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_405000.fbx", L"Walk");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_405010.fbx", L"Run");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\Boss_tenzen\\Animation\\a000_500000.fbx", L"Defense");
		//mMeshData->AnimationSave(L"Monster\\Boss_tenzen\\AnimationData\\tenzen.animationdata");

		//fbx 로드
		mMeshData = std::make_shared<MeshData>();
		mMeshData->Load(L"Monster\\Boss_tenzen\\MeshData\\c1020.meshdata");
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
		mKatanaObjectTr->SetScale(Vector3(0, 0, 0));
		
		



		//오브젝트 트랜스폼
		Transform* tr = GetComponent<Transform>();
		tr->SetPosition(Vector3(0, 100, 0));
		tr->SetScale(Vector3(10, 10, 10));


		//메시 데이터 트랜스폼
		Transform* meshTr = object->GetComponent<Transform>();
		meshTr->SetRotation(Vector3(180, 0, 0));
		meshTr->SetPosition(Vector3(0, 0, 0));
		meshTr->SetScale(Vector3(1, 1, 1));
		meshTr->SetRotationOffset(Vector3(-1, 0, -1));
		meshTr->SetParent(tr);


		//무기 콜라이더 추가
		//Initialize
		BoneCollider* katana =  object::Instantiate<BoneCollider>(eLayerType::MonsterProjectile);
		katana->SetMeshAndBone(mMeshData, L"R_Katana_long");

		//애니메이션 별로 오프셋과 발동 프레임 정해주기. 
		//발동 프레임은 기본이 false로 발동하고싶으면 조정해주면 된다.
		katana->SetAnimOffSet(L"SwingSword1",Vector3(1, 0.5, 1));
		katana->SetColliderActiveFrame(L"SwingSword1", 0, 100);

		//칼의 크기 정해주기
		katana->SetScale(Vector3(2, 0.3, 0.3));
		mKatanaCollider = katana;

		//피격 콜라이더
		mCollider = AddComponent<Collider2D>();
		mCollider->SetType(eColliderType::Box);
		mCollider->SetSize(Vector3(1,2,0.5));

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
		mMeshData->GetAnimationFrameEvent(L"SwingSword1", 12) = std::bind(&Tenzen::SwingSword1Frame12Event, this);
		mMeshData->GetAnimationEndEvent(L"SwingSword1") = std::bind(&Tenzen::AttackEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"Run") = std::bind(&Tenzen::TraceEndEvent, this);
		mMeshData->GetAnimationEndEvent(L"Defense") = std::bind(&Tenzen::DefenseEndEvent, this);

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
		if (STATE_HAVE(TenzenState_Idle) && !(BEFORE_STATE_HAVE(TenzenState_Idle)))
		{
			mAnimationName = L"ArmCrossStart";
		}
	}
	void Tenzen::Alert()
	{
		if (STATE_HAVE(TenzenState_Alert) && !(BEFORE_STATE_HAVE(TenzenState_Alert)))
		{
			mAnimationName = L"ArmCrossIdle";

		}
	}
	void Tenzen::Recognize()
	{
		if (STATE_HAVE(TenzenState_Recognize))
		{
			if(!(STATE_HAVE(TenzenState_DrawSword)))
				mAnimationName = L"DrawSword";

		}

		if (STATE_HAVE(TenzenState_DrawSword))
		{
			if (!(STATE_HAVE(TenzenState_Attack)) && !(STATE_HAVE(TenzenState_Defense)) && !(STATE_HAVE(TenzenState_Trace)))
			{
				int choice = rand() % 3;
				switch (choice)
				{
				case 0:
					ADD_STATE(TenzenState_Attack);
					break;
				case 1:
					ADD_STATE(TenzenState_Defense);
					break;
				case 2:
					ADD_STATE(TenzenState_Trace);
					break;
				}
			}
		}

	}
	void Tenzen::Attack()
	{
		if (STATE_HAVE(TenzenState_Attack))
		{
			mAnimationName = L"SwingSword1";
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
			mAnimationName = L"Run";
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
	}
	void Tenzen::TraceEndEvent()
	{
		RM_STATE(TenzenState_Trace);
	}
	void Tenzen::SwingSword1Frame12Event()
	{
		Transform* tr = GetComponent<Transform>();
		mActionScript->Move(tr->Forward(), 1000);
	}
}