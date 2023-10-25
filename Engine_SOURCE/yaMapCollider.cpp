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
		//AddWallCollider(Vector3(1, 1, 1));
		//AddGroundCollider(Vector3(1, 1, 1));
		//대연
		mWalkerName = L"KDY";

		//은혜
		mWalkerName = L"JU";
		// 14xxxx
		AddGroundCollider(Vector3(190.0, -30.0, 88.0), Vector3(-30.0, -20.0, 5.0), Vector3(13.0, 1.0, 10.0));	// 0
		AddGroundCollider(Vector3(195.0, -30.0, 86.0), Vector3(-20.0, 32.0, 0.0), Vector3(8.0, 1.0, 13.0));		// 1
		AddGroundCollider(Vector3(186.0, -34.0, 80.0), Vector3(-26.0, 30.0, 0.0), Vector3(24.0, 1.0, 10.0));	// 2
		AddGroundCollider(Vector3(180.0, -36.701, 77.0), Vector3(-20.0, 45.0, -8.0), Vector3(20.0, 1.0, 10.0));	// 3
		AddGroundCollider(Vector3(173.0, -37.201, 66.0), Vector3(0.0, 50.0, 0.0), Vector3(12.0, 1.0, 24.0));	// 4
		AddGroundCollider(Vector3(153.0, -37.201, 55.0), Vector3(0.0, 40.0, 0.0), Vector3(12.0, 1.0, 24.0));	// 5
		AddGroundCollider(Vector3(156.0, -32.0, 65.0), Vector3(13.0, 54.0, 14.0), Vector3(8.0, 1.0, 16.0));		// 6
		AddGroundCollider(Vector3(144.0, -26.50, 59.0), Vector3(4.0, 36.0, 10.0), Vector3(9.0, 1.0, 9.0));		// 7

		// 2xxxxx
		AddGroundCollider(Vector3(153.50, -29.50, 41.50), Vector3(0.0, 39.0, 0.0), Vector3(9.0, 1.0, 7.0));		// 8
		AddGroundCollider(Vector3(138.40, -31.10, 53.0), Vector3(0.0, 41.0, 0.0), Vector3(9.0, 1.0, 10.0));		// 9
		AddGroundCollider(Vector3(145.0, -35.3, 47.0), Vector3(27.0, 40.0, -0.0), Vector3(10.0, 1.0, 9.0));		// 10
		AddGroundCollider(Vector3(138.0, -33.50, 38.0), Vector3(0.0, 39.50, 0.0), Vector3(10.0, 1.0, 15.0));	
		
		AddGroundCollider(Vector3(152.2, -24.40, 33.3), Vector3(0.0, 41.0, 0.0), Vector3(16.0, 1.0, 8.50));
		AddGroundCollider(Vector3(146.60, -29.10, 21.0), Vector3(0.0, 41.0, 0.0), Vector3(8.0, 1.0, 18.0));

		AddGroundCollider(Vector3(143.4, -31.40, 30.0), Vector3(24.0, 36.0, 33.0), Vector3(10.40, 1.0, 7.0));
		AddGroundCollider(Vector3(145.10, -27.3, 20.50), Vector3(28.0, 39.50, 2.0), Vector3(5.60, 1.0, 10.0));
		AddGroundCollider(Vector3(131.0, -24.8, 26.50), Vector3(0.0, 40.0, 0.0), Vector3(24.0, 1.0, 13.0));
		AddGroundCollider(Vector3(140.0, -24.8, 15.0), Vector3(0.0, 40.0, 0.0), Vector3(6.0, 1.0, 7.0));
		AddGroundCollider(Vector3(127.50, -24.8, 41.0), Vector3(0.0, 40.0, 0.0), Vector3(10.0, 1.0, 25.0));

		AddGroundCollider(Vector3(127.50, -24.8, 41.0), Vector3(0.0, 40.0, 0.0), Vector3(10.0, 1.0, 25.0));
		AddGroundCollider(Vector3(127.50, -24.8, 41.0), Vector3(0.0, 40.0, 0.0), Vector3(10.0, 1.0, 25.0));
		AddGroundCollider(Vector3(127.50, -24.8, 41.0), Vector3(0.0, 40.0, 0.0), Vector3(10.0, 1.0, 25.0));
		AddGroundCollider(Vector3(127.50, -24.8, 41.0), Vector3(0.0, 40.0, 0.0), Vector3(10.0, 1.0, 25.0));
		AddGroundCollider(Vector3(127.50, -24.8, 41.0), Vector3(0.0, 40.0, 0.0), Vector3(10.0, 1.0, 25.0));
		
		//AddGroundCollider(Vector3(1, 1, 1));

		// 14xxxx
		AddWallCollider(Vector3(169.000000, -32.374001, 71.000000), Vector3(0.000000, -50.000000, 0.000000), Vector3(12.000000, 13.000000, 1.000000));
		AddWallCollider(Vector3(159.000000, -34.500000, 62.500000), Vector3(0.000000, -33.000000, 0.000000), Vector3(17.000000, 6.000000, 1.000000));
		AddWallCollider(Vector3(148.000000, -31.799999, 56.000000), Vector3(0.000000, -51.000000, 0.000000), Vector3(8.000000, 12.000000, 1.000000));

		// 2xxxxx
		AddWallCollider(Vector3(142.000000, -34.000000, 51.000000), Vector3(0.000000, -49.000000, 0.000000), Vector3(9.000000, 7.000000, 1.000000));
		AddWallCollider(Vector3(150.500000, -33.000000, 45.000000), Vector3(1.000000, -50.000000, -8.000000), Vector3(6.000000, 7.000000, 1.000000));
		AddWallCollider(Vector3(156.000000, -34.000000, 44.500000), Vector3(-8.000000, 39.000000, 0.000000), Vector3(10.000000, 10.000000, 1.000000));
		AddWallCollider(Vector3(157.000000, -33.000000, 39.000000), Vector3(4.000000, -50.000000, 0.000000), Vector3(6.000000, 7.000000, 1.000000));

		AddWallCollider(Vector3(146.000000, -30.000000, 38.500000), Vector3(-10.000000, -49.000000, 0.000000), Vector3(9.000000, 11.000000, 1.000000));
		AddWallCollider(Vector3(156.000000, -30.000000, 37.000000), Vector3(-13.000000, 40.000000, 0.000000), Vector3(16.000000, 12.000000, 1.000000));
		AddWallCollider(Vector3(135.000000, -30.500000, 42.000000), Vector3(6.000000, -49.000000, 0.000000), Vector3(15.000000, 12.000000, 1.000000));
		AddWallCollider(Vector3(137.300003, -29.000000, 30.500000), Vector3(-8.000000, 40.500000, 0.000000), Vector3(18.000000, 10.000000, 1.000000));
		AddWallCollider(Vector3(142.000000, -24.000000, 22.000000), Vector3(0.000000, -50.000000, 0.000000), Vector3(7.000000, 6.000000, 0.800000));
		//AddWallCollider(Vector3(1, 1, 1));


		//상수
		mWalkerName = L"KSS";

	}
	void MapCollider::AddWallCollider(Vector3 transform, Vector3 rotation, Vector3 size)
	{
		GameObject* obj = object::Instantiate<GameObject>(eLayerType::Wall);
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
		GameObject* obj = object::Instantiate<GameObject>(eLayerType::Wall);
		std::wstring num = std::to_wstring(mGroundCount++);
		obj->SetName(mWalkerName + L"_GroundCollider_" + num);
		obj->AddComponent<Collider2D>()->SetType(eColliderType::Box);
		Transform* tr = obj->GetComponent<Transform>();
		tr->SetPosition(transform);
		tr->SetRotation(rotation);
		tr->SetScale(size);
	}
}