#include "yaRedOgre.h"
#include "yaMeshData.h"
#include "yaActionScript.h"
#include "yaBoneAnimator.h"
#include "yaBoneCollider.h"
#include "yaInput.h"


namespace ya
{
	ya::RedOgre::RedOgre()
		: mMeshData(nullptr)
		, mLHandCollider(nullptr)
		, mRHandCollider(nullptr)
		, mCollider(nullptr)
		, mActionScript(nullptr)
		, mState(0)
		, mBeforeState(0)
	{
	}

	ya::RedOgre::~RedOgre()
	{
	}

	void ya::RedOgre::Initialize()
	{
		SetName(L"RedOgre");
		//GameObject* player = mMeshData->Instantiate(eLayerType::Monster);

		//mMeshData = MeshData::LoadFromFbx(L"Monster\\RedOgre\\Mesh\\c5020.fbx");
		//
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a000_000000.fbx", L"a000_000000");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a000_000401.fbx", L"a000_000401");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a000_000402.fbx", L"a000_000402");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a000_010000.fbx", L"a000_010000");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003000.fbx", L"a100_003000");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003001.fbx", L"a100_003001");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003002.fbx", L"a100_003002");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003003.fbx", L"a100_003003");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003005.fbx", L"a100_003005");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003006.fbx", L"a100_003006");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003007.fbx", L"a100_003007");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003008.fbx", L"a100_003008");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_003011.fbx", L"a100_003011");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_005401.fbx", L"a100_005401");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_005402.fbx", L"a100_005402");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_005403.fbx", L"a100_005403");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_005400.fbx", L"a100_005400");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_005000.fbx", L"a100_005000");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_405010.fbx", L"a100_405010");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_405001.fbx", L"a100_405001");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_405002.fbx", L"a100_405002");
		//mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\a100_405003.fbx", L"a100_405003");
		////mMeshData->LoadAnimationFromFbx(L"Monster\\RedOgre\\Animation\\.fbx", L"");


		//mMeshData->AnimationSave(L"Monster\\RedOgre\\AnimationData\\RedOgre.animationdata");

		mMeshData = std::make_shared<MeshData>();
		mMeshData->Load(L"Monster\\RedOgre\\MeshData\\c5020.meshdata");
		mMeshData->AnimationLoad(L"Monster\\RedOgre\\AnimationData\\RedOgre.animationdata");
		MeshObject* object = mMeshData->Instantiate(eLayerType::Monster);

		//오브젝트 트랜스폼
		Transform* tr = GetComponent<Transform>();
		tr->SetPosition(Vector3(-30, 20, 0));
		tr->SetScale(Vector3(3, 3, 3));

		//메시 데이터 트랜스폼
		Transform* meshTr = object->GetComponent<Transform>();
		meshTr->SetRotation(Vector3(180, 180, 0));
		meshTr->SetPosition(Vector3(0, 0, 0));
		meshTr->SetScale(Vector3(1, 1, 1));
		//meshTr->SetRotationOffset(Vector3(0, 1.5, 0));
		meshTr->SetParent(GetComponent<Transform>());

		//무기 콜라이더 추가
		//Initialize
		BoneCollider* LHand = object::Instantiate<BoneCollider>(eLayerType::MonsterProjectile);
		LHand->SetMeshAndBone(mMeshData, L"L_Hand");

		//LHand->SetAnimOffSet(L"a100_003005", Vector3(1, 0.5, 1));
		LHand->SetColliderActiveFrame(L"a100_003005", 1, 17);

		LHand->SetScale(Vector3(2, 0.3, 0.3));
		mLHandCollider = LHand;

		// 피격 충돌체
		Collider2D* mCollider = AddComponent<Collider2D>();
		mCollider->SetType(eColliderType::Box);
		mCollider->SetSize(Vector3(1.0, 3.0f, 1.0f));

		AddComponent<Rigidbody>();
		mActionScript = AddComponent<ActionScript>();
		assert(mActionScript);

		mAnimationName = L"a100_003005";

		MonsterBase::Initialize();
	}

	void ya::RedOgre::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N_1))
			mMeshData->Play(L"a000_000401");

		if (Input::GetKeyDown(eKeyCode::N_2))
			mMeshData->Play(L"a000_000402");

		MonsterBase::Update();
	}

	void ya::RedOgre::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void ya::RedOgre::Render()
	{
		MonsterBase::Render();
	}
}