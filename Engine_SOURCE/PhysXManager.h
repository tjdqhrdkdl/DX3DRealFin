#pragma once
#include "CommonInclude.h"
#include "yaMath.h"

namespace ya
{
	using namespace math;
	//전방 선언
	struct RaycastHit;
	class Scene;

	class PhysicsManager final
	{
	public:
		static bool initialize(void);
		static void update(float deltaTime);

		static void enableRaycast(UINT32 leftLayerIndex, UINT32 rightLayerIndex, bool enable);
		static void enableCollision(UINT32 leftLayerIndex, UINT32 rightLayerIndex, bool enable);
		static void enableGravity(bool enable, Scene* pScene, Vector3 gravity = Vector3(0.f, -9.8f, 0.f));

		static bool raycast(UINT32 layerIndex, const Vector3& origin, const Vector3& direction, float maxDistance, RaycastHit* outHit);
		static void drawRaycast(const Vector3& origin, const Vector3& direction, float maxDistance, const Vector3& color = Vector3{ 1.f, 0.f, 0.f });

		static void createPhysicScene(Scene* scene);
		static void changePhysicScene(Scene* scene);

		static void enablePhysics(bool enable) { _enable = enable; }
		static bool isPhysicsEnabled(void) { return _enable; }

	private:
		inline static bool _enable = true;
	};
} //namespace pa
