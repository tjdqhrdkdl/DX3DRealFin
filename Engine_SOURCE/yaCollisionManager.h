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
		static bool Initialize(void);
		static void Update(float deltaTime);

		static inline void EnableRaycast(eLayerType leftLayerIndex, eLayerType rightLayerIndex, bool enable);
		static inline void EnableCollision(eLayerType leftLayerIndex, eLayerType rightLayerIndex, bool enable);
		static inline void EnableGravity(bool enable, Scene* pScene, Vector3 gravity = Vector3(0.f, -9.8f, 0.f));
		static inline bool Raycast(eLayerType srcLayerIndex, const Vector3& origin, const Vector3& direction, float maxDistance, RaycastHit* outHit);

		static void drawRaycast(const Vector3& origin, const Vector3& direction, float maxDistance, const Vector3& color = Vector3{ 1.f, 0.f, 0.f });

		static void createPhysicScene(Scene* scene);
		static void changePhysicScene(Scene* scene);

		static void enablePhysics(bool enable) { _enable = enable; }
		static bool isPhysicsEnabled(void) { return _enable; }

	private:
		inline static bool _enable = true;
	};


	inline void CollisionManager::EnableRaycast(eLayerType leftLayerIndex, eLayerType rightLayerIndex, bool enable)
	{
		PhysxWrapper::getInstance().EnableRaycast((UINT32)leftLayerIndex, (UINT32)rightLayerIndex, enable);
	}

	inline void CollisionManager::EnableCollision(eLayerType leftLayerIndex, eLayerType rightLayerIndex, bool enable)
	{
		PhysxWrapper::getInstance().EnableCollision((UINT32)leftLayerIndex, (UINT32)rightLayerIndex, enable);
	}

	inline void CollisionManager::EnableGravity(bool enable, Scene* pScene, Vector3 gravity)
	{
		gravity = (enable) ? gravity : Vector3{ 0.f, 0.f, 0.f };
		PhysxWrapper::getInstance().EnableGravity(enable, pScene, gravity);
	}


	inline bool CollisionManager::Raycast(eLayerType srcLayerIndex, const Vector3& origin, const Vector3& direction, float maxDistance, RaycastHit* outHit)
	{
		Vector3 normalized{};
		direction.Normalize(normalized);

		drawRaycast(origin, direction, maxDistance);

		return PhysxWrapper::getInstance().Raycast((UINT32)srcLayerIndex, origin, normalized, maxDistance, outHit);
	}

} //namespace pa
