#pragma once
#include "CommonInclude.h"

#pragma warning(disable : 26495)
#pragma warning(disable : 33010)
#pragma warning(disable : 26812)
#pragma warning(disable : 26451)
#pragma warning(disable : 6297)
//#define PX_PHYSX_STATIC_LIB
#include <PxPhysicsAPI.h>
#pragma warning(default : 26495)
#pragma warning(default : 33010)
#pragma warning(default : 26812)
#pragma warning(default : 26451)
#pragma warning(default : 6297)



namespace ya
{
	// 전방 선언
	using namespace math;
	using namespace enums;
	class GameObject;
	class Scene;
	class GameObject;
	class Collider3D;

	struct RaycastHit
	{
		GameObject* gameObject;
		bool		hasBlocking;
		Vector3		hitPosition;
		Vector3		hitNormal;
		float		hitDistance;
	};

	enum class UpdateInterval : UINT8
	{
		Frame_30,
		Frame_60,
		Frame_120,
		Frame_144,
		Frame_240,

		END
	};



	class PhysxWrapper final : public physx::PxSimulationEventCallback
	{
	public:
		static PhysxWrapper& getInstance(void);
		virtual ~PhysxWrapper(void);
		void Initialize(void);
		void CollisionUpdate();

		void EnableRaycast(UINT32 leftLayerIndex, UINT32 rightLayerIndex, bool enable);
		void EnableCollision(UINT32 leftLayerIndex, UINT32 rightLayerIndex, bool enable);
		void EnableGravity(bool enable, Scene* scene, const Vector3& gravity) const;

		void createActorCube(GameObject* gameObject, const Vector3& halfExtents, physx::PxShape** outShape, bool isStatic);
		void createActorSphere(GameObject* gameObject, float radius, physx::PxShape** outShape, bool isStatic);
		void createActorCapsule(GameObject* gameObject, float radius, float height, physx::PxShape** outShape, bool isStatic);
		void changeGeometry(Collider3D* collider, physx::PxShape* shape, eColliderType type);

		bool Raycast(UINT32 srcLayerIndex, const Vector3& origin, const Vector3& direction, float maxDistance, RaycastHit* outHit) const;

		void createScene(Scene* scene);
		void changeScene(Scene* scene);

		void Test();

	private:
		explicit PhysxWrapper(void);
		void SyncGameScene(void) const;
		void setupFiltering(physx::PxShape* shape, UINT32 layerIndex) const;

		static physx::PxFilterFlags FilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
			physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
			physx::PxPairFlags& pairFlags,
			const void* constantBlock, physx::PxU32 constantBlockSize);

		void															  setUpdateInterval(UpdateInterval interval) { _currentInterval = interval; }
		float															  getUpdateInterval(void) const { return _intervals[static_cast<UINT8>(_currentInterval)]; }
		const std::array<float, static_cast<UINT8>(UpdateInterval::END)>& getIntervalList(void) const { return _intervals; }

		// PxSimulationEventCallback을(를) 통해 상속됨
		void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
		void onWake(physx::PxActor** actors, physx::PxU32 count) override;
		void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
		void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;

		

	private:
		physx::PxDefaultAllocator		_allocator;
		physx::PxDefaultErrorCallback	_errorCallback;
		physx::PxFoundation* _foundation;
		physx::PxPhysics* _physics;
		physx::PxDefaultCpuDispatcher* _dispatcher;
		physx::PxScene* _currentScene;
		physx::PxMaterial* _material;
		std::array<std::bitset<32>, 32> _collisionMask;
		std::array<std::bitset<32>, 32> _raycastMask;

		std::array<float, static_cast<UINT8>(UpdateInterval::END)> _intervals;
		UpdateInterval											   _currentInterval;

		std::unordered_map<Scene*, physx::PxScene*>  _scenes;
		inline static constexpr float _kDefaultDensity = 10.f;
#ifdef _DEBUG
		physx::PxPvd* _pvd{};
#endif
	};
} // namespace pa