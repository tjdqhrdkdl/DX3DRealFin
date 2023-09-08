#include "yaRedOgre.h"
#include "yaMeshData.h"


namespace ya
{
	ya::RedOgre::RedOgre()
		: mMeshData(nullptr)
		, mLHandCollider(nullptr)
		, mRHandCollider(nullptr)
		, mCollider(nullptr)
		, mActionScript(nullptr)
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

		//메시 데이터 트랜스폼
		Transform* meshTr = object->GetComponent<Transform>();
		meshTr->SetRotation(Vector3(180, 180, 0));
		meshTr->SetPosition(Vector3(0, 20, 0));
		meshTr->SetScale(Vector3(1, 1, 1));
		meshTr->SetRotationOffset(Vector3(0, 0, 0));
		meshTr->SetParent(GetComponent<Transform>());


		Collider2D* mCollider = AddComponent<Collider2D>();
		mCollider->SetType(eColliderType::Box);
		mCollider->SetSize(Vector3(1.0, 2.0f, 1.0f));

		AddComponent<Rigidbody>();
		mActionScript = AddComponent<ActionScript>();
		assert(mActionScript);
		//mRedOgre->AddComponent<MonsterScript>();

		MonsterBase::Initialize();
	}

	void ya::RedOgre::Update()
	{
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