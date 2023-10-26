#pragma once
#include "CommonInclude.h"
#include "yaMath.h"
#include "PhysXWrapper.h"

namespace ya
{
	using namespace math;
	using namespace enums;
	//전방 선언
	struct RaycastHit;
	class Scene;

	class CollisionManager final
	{
	public:
		static bool initialize(void);
		static void update(float deltaTime);

		static void enableRaycast(UINT32 leftLayerIndex, UINT32 rightLayerIndex, bool enable);
		static void enableCollision(UINT32 leftLayerIndex, UINT32 rightLayerIndex, bool enable);
		static void enableGravity(bool enable, Scene* pScene, Vector3 gravity = Vector3(0.f, -9.8f, 0.f));
		static bool raycast(UINT32 layerIndex, const Vector3& origin, const Vector3& direction, float maxDistance, RaycastHit* outHit);

		static inline void enableRaycast(eLayerType leftLayerIndex, eLayerType rightLayerIndex, bool enable);
		static inline void enableCollision(eLayerType leftLayerIndex, eLayerType rightLayerIndex, bool enable);
		static inline bool raycast(eLayerType layerIndex, const Vector3& origin, const Vector3& direction, float maxDistance, RaycastHit* outHit);

		static void drawRaycast(const Vector3& origin, const Vector3& direction, float maxDistance, const Vector3& color = Vector3{ 1.f, 0.f, 0.f });

		static void createPhysicScene(Scene* scene);
		static void changePhysicScene(Scene* scene);

		static void enablePhysics(bool enable) { _enable = enable; }
		static bool isPhysicsEnabled(void) { return _enable; }

	private:
		inline static bool _enable = true;
	};



	inline void CollisionManager::enableRaycast(eLayerType leftLayerIndex, eLayerType rightLayerIndex, bool enable)
	{
		enableRaycast((UINT32)leftLayerIndex, (UINT32)rightLayerIndex, enable);
	}

	inline void CollisionManager::enableCollision(eLayerType leftLayerIndex, eLayerType rightLayerIndex, bool enable)
	{
		enableCollision((UINT32)leftLayerIndex, (UINT32)rightLayerIndex, enable);
	}
	inline bool CollisionManager::raycast(eLayerType layerIndex, const Vector3& origin, const Vector3& direction, float maxDistance, RaycastHit* outHit)
	{
		return raycast((UINT32)layerIndex, origin, direction, maxDistance, outHit);
	}

} //namespace pa
