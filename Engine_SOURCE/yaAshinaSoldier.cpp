#include "yaAshinaSoldier.h"

namespace ya
{
	AshinaSoldier::AshinaSoldier()
	{
	}
	AshinaSoldier::~AshinaSoldier()
	{
	}
	void AshinaSoldier::Initialize()
	{
		SetName(L"SwordManObject");

		////fbx 로드
		mMeshData = MeshData::LoadFromFbx(L"Monster\\AshinaSoldier\\Mesh\\c1010.fbx");
		MeshObject* object = mMeshData->Instantiate(eLayerType::Monster, GetScene());
	}
	void AshinaSoldier::Update()
	{
	}
	void AshinaSoldier::FixedUpdate()
	{
	}
	void AshinaSoldier::Render()
	{
	}
	void AshinaSoldier::DeathBlow()
	{
	}
	void AshinaSoldier::OnCollisionEnter(Collider2D* collider)
	{
	}
	void AshinaSoldier::OnCollisionStay(Collider2D* collider)
	{
	}
	void AshinaSoldier::OnCollisionExit(Collider2D* collider)
	{
	}
}