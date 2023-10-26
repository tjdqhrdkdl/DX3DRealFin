#include "yaMapCollider.h"
#include "yaObject.h"
#include "yaWallScript.h"
#include "yaCollider2D.h"
namespace ya
{
	MapCollider::MapCollider()
	{
	}
	MapCollider::~MapCollider()
	{
	}
	void MapCollider::Initialize()
	{
		//성복
		SetName(L"mapColliders");
		mWalkerName = L"KSB";
		AddWallCollider(Vector3(1, 1, 1));
		AddGroundCollider(Vector3(1, 1, 1));
		//대연
		mWalkerName = L"KDY";

		//은혜
		mWalkerName = L"JU";

		//상수
		mWalkerName = L"KSS";
	}
	void MapCollider::AddWallCollider(Vector3 transform, Vector3 rotation, Vector3 size)
	{
		GameObject* obj = object::Instantiate<GameObject>(eLayerType::Wall);
		std::wstring num = std::to_wstring(mWallCount++);
		obj->SetName(mWalkerName + L"_WallCollider_" + num);
		Collider3D* col3D = obj->AddComponent<Collider3D>();
		col3D->SetType(eColliderType::Box, true);


		Transform* tr = obj->GetComponent<Transform>();
		tr->SetLocalPosition(transform);
		tr->SetLocalRotation(rotation);
		tr->SetLocalScale(size);
		obj->AddComponent<WallScript>();
	}
	void MapCollider::AddGroundCollider(Vector3 transform, Vector3 rotation, Vector3 size)
	{
		GameObject* obj = object::Instantiate<GameObject>(eLayerType::Wall);
		std::wstring num = std::to_wstring(mGroundCount++);
		obj->SetName(mWalkerName + L"_GroundCollider_" + num);
		obj->AddComponent<Collider3D>()->SetType(eColliderType::Box, true);
		Transform* tr = obj->GetComponent<Transform>();
		tr->SetLocalPosition(transform);
		tr->SetLocalRotation(rotation);
		tr->SetLocalScale(size);
	}
}