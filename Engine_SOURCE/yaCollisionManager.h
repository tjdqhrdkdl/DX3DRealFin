#pragma once
#include "CommonInclude.h"
#include "yaCollider2D.h"


namespace ya
{
	union ColliderID
	{
		struct
		{
			UINT32 left;
			UINT32 right;
		};
		UINT64 id;
	};

	struct Ray
	{
		Vector3 position;
		Vector3 direction;
	};

	struct RayHit
	{
		bool isHit;
		GameObject* hitObj;
		Vector3 contact;
	};

	struct DistAndObj
	{
		float dist;

		GameObject* obj;
	};

	//	class Scene;
	class CollisionManager
	{
	public:
		static void Initialize();
		static void Update();
		static void FixedUpdate();
		static void Render();

		static void CollisionLayerCheck(eLayerType left, eLayerType right, bool enable = true);
		static void LayerCollision(class Scene* scene, eLayerType left, eLayerType right);
		static void ColliderCollision(Collider2D* left, Collider2D* right);
		static bool Intersect(Collider2D* left, Collider2D* right);

		static RayHit RayCast(GameObject* owner, Vector3 direction);
		
	private:
		static DistAndObj LayerRayCollision(class Scene* scene, eLayerType objType, ya::Ray ray, GameObject* owner);
		static float RayIntersect(ya::Ray ray, Matrix worldMatrix);

	private:
		static std::bitset<(UINT)eLayerType::End> mLayerCollisionMatrix[(UINT)eLayerType::End];
		static std::map<UINT64, bool> mCollisionMap;
	};
}
