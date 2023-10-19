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
		AddWallCollider(Vector3(96.5, -20, 20), Vector3(0, -25, 0), Vector3(8, 9, 0.1));
		AddWallCollider(Vector3(104.000, -20, 21.700), Vector3(0, 0.000, 0), Vector3(8, 9, 0.1));
		AddWallCollider(Vector3(107.000, -20, 21.000), Vector3(0, 40.000, 0), Vector3(2.000, 9, 0.1));
		AddWallCollider(Vector3(112.000, -20, 20.500), Vector3(0, -5.000, 0), Vector3(8.000, 11.000, 0.1));
		AddWallCollider(Vector3(118.000, -22.000, 19.800), Vector3(0, 40.000, 0), Vector3(8.000, 5.000, 0.1));
		AddWallCollider(Vector3(124.000, -22.000, 21.000), Vector3(0, -50.000, 0), Vector3(10.000, 5.000, 0.1));
		AddWallCollider(Vector3(110.000, -19.000, -13.500), Vector3(0, -5, 0), Vector3(15.000, 13.000, 0.1));
		AddWallCollider(Vector3(103.500, -19.000, -19.000), Vector3(0, 85.000, 6.000), Vector3(11.000, 13.000, 0.1));
		AddWallCollider(Vector3(99.500, -22.000, -17.000), Vector3(0, 10.000, 0), Vector3(8.000, 7, 0.1));
		AddWallCollider(Vector3(97.500, -22.000, -16.000), Vector3(0, -50.000, 0), Vector3(3.500, 5.000, 0.1));
		AddWallCollider(Vector3(100.000, -22.000, -16.000), Vector3(0, 50.000, 0), Vector3(3.800, 5.000, 0.1));
		AddWallCollider(Vector3(114.500, -22.000, -11.000), Vector3(0, 90.000, 0), Vector3(6.000, 5.000, 0.1));
		AddWallCollider(Vector3(117.000, -21.000, -4.000), Vector3(0, 115.000, 0), Vector3(9.000, 10.000, 0.1));
		AddWallCollider(Vector3(123.000, -20.000, 5.000), Vector3(0, -25.000, 0), Vector3(12.000, 7, 0.1));
		AddWallCollider(Vector3(118.000, -20.000, 0.000), Vector3(0, 70.000, 0), Vector3(4.000, 8.000, 0.1));
		AddWallCollider(Vector3(130.0, -22.000, 8.000), Vector3(0, -30.000, 0), Vector3(5.000, 5.000, 0.1));
		AddWallCollider(Vector3(131.500, -22.000, 15.500), Vector3(0, -50.000, 0), Vector3(5.000, 5.000, 0.1));
		AddWallCollider(Vector3(127.000, -22.000, 18.000), Vector3(0, -50.000, 0), Vector3(4.000, 5.000, 0.1));
		AddWallCollider(Vector3(131.000, -22.000, 12.000), Vector3(0, 40.000, 0), Vector3(4.000, 5.000, 0.1));
		AddWallCollider(Vector3(124.000, -22.000, 18.000), Vector3(0, 40.000, 0), Vector3(5.000, 5.000, 0.1));
		AddWallCollider(Vector3(127.500, -17.000, 14.800), Vector3(0, 40.000, 0), Vector3(17.500, 4.000, 0.1));
		AddWallCollider(Vector3(136.500, -17.000, 11.200), Vector3(0, 130.000, 0), Vector3(5.600, 4.000, 0.1));
		AddWallCollider(Vector3(131.500, -17.000, 19.000), Vector3(0, 40.000, 0), Vector3(17.500, 4.000, 0.1));
		AddWallCollider(Vector3(123.000, -17.000, 22.000), Vector3(0, 130.000, 0), Vector3(5.600, 4.000, 0.1));
		AddWallCollider(Vector3(119.000, -15.000, 24.500), Vector3(0, 120.000, 0), Vector3(10.000, 8.000, 0.1));

		//마굿간
		AddWallCollider(Vector3(99.300, -21.900, 11.000), Vector3(0, 95.000, 0), Vector3(18.000, 5.000, 0.1));
		AddWallCollider(Vector3(104.500, -21.900, 10.500), Vector3(0, 95.000, 0), Vector3(18.000, 5.000, 0.1));
		AddWallCollider(Vector3(101.000, -21.900, 1.900), Vector3(0, 5.000, 0), Vector3(5.000, 5.000, 0.1));
		AddWallCollider(Vector3(103.000, -21.900, 20.000), Vector3(0, 5.000, 0), Vector3(5.000, 5.000, 0.1));


		AddGroundCollider(Vector3(116.000, -24.00, 2), Vector3(0, -5, 0), Vector3(400.000, 1.000, 400));
		AddGroundCollider(Vector3(111.000, -13.000, -19.000), Vector3(0, -5, 0), Vector3(13.000, 1.000, 10.000));
		AddGroundCollider(Vector3(124.000, -16.500, -2.000), Vector3(10.000, -45.000, -5.000), Vector3(20.000, 1.000, 6.000));
		AddGroundCollider(Vector3(124.500, -17.00, 2.800), Vector3(0, -25.000, 0), Vector3(11.000, 1.000, 6.000));
		AddGroundCollider(Vector3(121.000, -19.700, 23.500), Vector3(0, 42.000, 0), Vector3(9.000, 1.000, 9.000));
		AddGroundCollider(Vector3(100.500, -18.500, 11.000), Vector3(2.000, 6.000, 30.000), Vector3(3.000, 0.500, 21.000));
		AddGroundCollider(Vector3(103.300, -18.500, 11.000), Vector3(-3.500, 6.000, -30.000), Vector3(3.000, 0.500, 21.000));
		AddGroundCollider(Vector3(105.500, -20.000, 11.000), Vector3(-0.000, 6.000, -10.000), Vector3(4.000, 0.500, 21.000));
		AddGroundCollider(Vector3(128.000, -13.800, 15.000), Vector3(-35.000, 39.000, 0), Vector3(18.500, 1.000, 6.000));
		AddGroundCollider(Vector3(131.000, -13.800, 18.500), Vector3(35.000, 39.000, 0), Vector3(18.500, 1.000, 6.000));
		AddGroundCollider(Vector3(137.000, -19.800, 10.500), Vector3(0, 42.000, 0), Vector3(4.500, 1.000, 8.500));
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
		GameObject* obj = object::Instantiate<GameObject>(eLayerType::Wall, GetScene());
		std::wstring num = std::to_wstring(mWallCount++);
		obj->SetName(mWalkerName + L"_WallCollider_" + num);
		obj->AddComponent<Collider2D>()->SetType(eColliderType::Box);
		Transform* tr = obj->GetComponent<Transform>();
		tr->SetPosition(transform);
		tr->SetRotation(rotation);
		tr->SetScale(size);
		obj->AddComponent<WallScript>();
	}
	void MapCollider::AddGroundCollider(Vector3 transform, Vector3 rotation, Vector3 size)
	{
		GameObject* obj = object::Instantiate<GameObject>(eLayerType::Ground, GetScene());
		std::wstring num = std::to_wstring(mGroundCount++);
		obj->SetName(mWalkerName + L"_GroundCollider_" + num);
		obj->AddComponent<Collider2D>()->SetType(eColliderType::Box);
		Transform* tr = obj->GetComponent<Transform>();
		tr->SetPosition(transform);
		tr->SetRotation(rotation);
		tr->SetScale(size);
	}
}