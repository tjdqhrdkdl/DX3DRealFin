#include "yaMapCollider.h"
#include "yaObject.h"
#include "yaWallScript.h"
#include "yaCollider2D.h"
namespace ya
{
	MapCollider::MapCollider()
		: mGroundCount(0)
		, mWallCount(0)
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

		AddGroundCollider(Vector3(141.7, -20.9, 21.8), Vector3(0.0, 40.0, 0.0), Vector3(1.5, 0.2, 7.0));
		
		// 14xxxx
		AddWallCollider(Vector3(169.0, -32.37401, 71.0), Vector3(0.0, -50.0, 0.0), Vector3(12.0, 13.0, 1.0));
		AddWallCollider(Vector3(159.0, -34.50, 62.50), Vector3(0.0, -33.0, 0.0), Vector3(17.0, 6.0, 1.0));
		AddWallCollider(Vector3(148.0, -31.799999, 56.0), Vector3(0.0, -51.0, 0.0), Vector3(8.0, 12.0, 1.0));

		// 2xxxxx
		AddWallCollider(Vector3(142.0, -34.0, 51.0), Vector3(0.0, -49.0, 0.0), Vector3(9.0, 7.0, 1.0));
		AddWallCollider(Vector3(150.50, -33.0, 45.0), Vector3(1.0, -50.0, -8.0), Vector3(6.0, 7.0, 1.0));
		AddWallCollider(Vector3(156.0, -34.0, 44.50), Vector3(-8.0, 39.0, 0.0), Vector3(10.0, 10.0, 1.0));
		AddWallCollider(Vector3(157.0, -33.0, 39.0), Vector3(4.0, -50.0, 0.0), Vector3(6.0, 7.0, 1.0));

		AddWallCollider(Vector3(146.0, -30.0, 38.50), Vector3(-10.0, -49.0, 0.0), Vector3(9.0, 11.0, 1.0));
		AddWallCollider(Vector3(156.0, -30.0, 37.0), Vector3(-13.0, 40.0, 0.0), Vector3(16.0, 12.0, 1.0));
		AddWallCollider(Vector3(135.0, -30.50, 42.0), Vector3(6.0, -49.0, 0.0), Vector3(15.0, 12.0, 1.0));
		AddWallCollider(Vector3(137.303, -29.0, 30.50), Vector3(-8.0, 40.50, 0.0), Vector3(18.0, 10.0, 1.0));
		AddWallCollider(Vector3(142.0, -24.0, 22.0), Vector3(0.0, -50.0, 0.0), Vector3(7.0, 6.0, 0.80));


		//상수
		mWalkerName = L"KSS";
		AddWallCollider(Vector3(97.50, -22.0, -14.4), Vector3(0.0, -5.0, 0.0), Vector3(0.50, 7.0, 5.0));
		AddWallCollider(Vector3(96.40, -22.0, -4.0), Vector3(0.0, -5.0, 0.0), Vector3(0.50, 7.0, 5.0));
		AddWallCollider(Vector3(95.0, -20.0, -6.30), Vector3(0.0, -5.0, 0.0), Vector3(3.80, 10.0, 0.30));
		AddWallCollider(Vector3(96.0, -20.0, -12.6), Vector3(0.0, -5.0, 0.0), Vector3(3.80, 10.0, 0.30));
		AddWallCollider(Vector3(96.40, -22.0, -4.0), Vector3(0.0, -5.0, 0.0), Vector3(0.50, 7.0, 5.0));
		AddWallCollider(Vector3(96.40, -22.0, -4.0), Vector3(0.0, -5.0, 0.0), Vector3(0.50, 7.0, 5.0));
		AddWallCollider(Vector3(96.0, -22.0, 1.0), Vector3(0.0, -5.0, 0.0), Vector3(0.50, 4.0, 6.0));
		AddWallCollider(Vector3(96.0, -22.0, 1.0), Vector3(0.0, -5.0, 0.0), Vector3(0.50, 4.0, 6.0)); //
		AddWallCollider(Vector3(91.0, -22.90, 8.0), Vector3(10.0, -7.0, 0.0), Vector3(8.0, 10.0, 1));
		AddWallCollider(Vector3(79.0, -22.90, 3.30), Vector3(7.0, -7.0, 0.0), Vector3(8.0, 10.0, 1));
		AddWallCollider(Vector3(82.40, -23.40, 9.0), Vector3(0.0, -7.0, 10.0), Vector3(1, 10.0, 11.0));
		AddWallCollider(Vector3(87.0, -23.0, 9.0), Vector3(0.0, -7.0, -4.0), Vector3(1.0, 10.0, 4.0));
		AddWallCollider(Vector3(94.70, -22.0, 14.0), Vector3(0.0, -5.0, 10.0), Vector3(1, 8.0, 12.0));
		AddWallCollider(Vector3(78.40, -16.50, 4.20), Vector3(0.0, -5.0, 0.0), Vector3(8.0, 3.0, 0.70));
		AddWallCollider(Vector3(83.0, -17.0, 18.0), Vector3(0.0, -5.0, 0.0), Vector3(24.0, 4.0, 1));
		AddWallCollider(Vector3(68.0, -16.0, 1.0), Vector3(0.0, -23.0, 0.0), Vector3(17.0, 16.0, 1));
		AddWallCollider(Vector3(74.0, -15.0, 9.0), Vector3(0.0, -14.0, 0.0), Vector3(1, 16.0, 11.0));
		AddWallCollider(Vector3(55.50, -18.0, -19.0), Vector3(0.0, -5.0, 10.0), Vector3(1, 14.0, 9.0));
		AddWallCollider(Vector3(53.0, -10.0, -22.0), Vector3(-10.0, -5.0, 0.0), Vector3(17.0, 10.0, 1));
		AddWallCollider(Vector3(43.0, -6.40, -24.0), Vector3(-10.0, -5.0, 0.0), Vector3(7.0, 10.0, 1));
		AddWallCollider(Vector3(41.50, -9.40, -13.50), Vector3(0.0, -5.0, 0.0), Vector3(1, 1, 19.0));
		AddWallCollider(Vector3(39.0, -6.60, -3.70), Vector3(10.0, -5.0, 0.0), Vector3(3.0, 10.0, 1));
		AddWallCollider(Vector3(40.30, -6.60, -2.0), Vector3(0.0, -5.0, 10.0), Vector3(1, 10.0, 3.0));


		AddGroundCollider(Vector3(97.90, -15.30, -9.10), Vector3(3.80, -5.0, -34.0), Vector3(2.20, 0.30, 10.60));
		AddGroundCollider(Vector3(96.20, -15.30, -9.10), Vector3(-3.0, -5.0, 30.0), Vector3(1.80, 0.30, 10.60));
		AddGroundCollider(Vector3(79.40, -15.0, 4.0), Vector3(0, -6.0, 0), Vector3(6.30, 0.30, 1.40));

		AddGroundCollider(Vector3(84.0, -20.30, 6.0), Vector3(-26.0, -2.0, 2.0), Vector3(6.0, -0.50, 10.0));
		AddGroundCollider(Vector3(90.60, -18.20, 13.20), Vector3(0, -6.0, 0), Vector3(8.0, 1.0, 10.0));
		AddGroundCollider(Vector3(84.0, -18.20, 15.0), Vector3(0, -6.0, 0), Vector3(10.0, 1, 10.0));
		AddGroundCollider(Vector3(77.70, -18.20, 11.0), Vector3(0, -6.0, 0), Vector3(10, 1, 10));
		AddGroundCollider(Vector3(85.0, -15.30, 24.0), Vector3(0, -5.0, 2.0), Vector3(22.0, 1.0, 10.0));
		AddGroundCollider(Vector3(72.0, -15.0, 24.0), Vector3(0, -30.0, 0), Vector3(8.0, 1.0, 10.0));

		AddGroundCollider(Vector3(62.0, -15.0, -9.50), Vector3(0, -4.0, -22.0), Vector3(22.0, 1.0, 8.50));
		AddGroundCollider(Vector3(49.70, -11.0, -12.50), Vector3(0, -5.0, 0), Vector3(11.0, 1.0, 24.0));
		AddGroundCollider(Vector3(49.0, -11.0, 4.50), Vector3(0, -6.0, 0), Vector3(15.0, 1.0, 15.0));
		AddGroundCollider(Vector3(58.0, -8.50, 6.0), Vector3(0, -20.0, 15.0), Vector3(18.0, 1.0, 10.0));
		AddGroundCollider(Vector3(68.0, -6.50, 8.0), Vector3(-4.0, -20.0, 10.0), Vector3(10.0, 1.0, 10.0));
		
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