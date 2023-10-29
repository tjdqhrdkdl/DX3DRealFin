#include "PhysXWrapper.h"

#include "yaGameObject.h"
#include "yaCollider3D.h"
#include "yaTransform.h"
#include "yaScene.h"
#include "yaMath.h"
#include "MathUtil.h"
#include "StrConverter.h"
#include "yaTime.h"

#ifdef _DEBUG
#pragma comment(lib, "Physx/Library/Debug/PhysX_64.lib")
#pragma comment(lib, "Physx/Library/Debug/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "Physx/Library/Debug/PhysXCommon_64.lib")
#pragma comment(lib, "Physx/Library/Debug/PhysXCooking_64.lib")
#pragma comment(lib, "Physx/Library/Debug/PhysXExtensions_static_64.lib")
#pragma comment(lib, "Physx/Library/Debug/PhysXFoundation_64.lib")
#pragma comment(lib, "Physx/Library/Debug/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "Physx/Library/Debug/PhysXTask_static_64.lib")
#pragma comment(lib, "Physx/Library/Debug/PhysXVehicle_static_64.lib")
#pragma comment(lib, "Physx/Library/Debug/PhysXVehicle2_static_64.lib")
#pragma comment(lib, "Physx/Library/Debug/PVDRuntime_64.lib")
#pragma comment(lib, "Physx/Library/Debug/SceneQuery_static_64.lib")
#pragma comment(lib, "Physx/Library/Debug/SimulationController_static_64.lib")
#pragma comment(lib, "Physx/Library/Debug/LowLevel_static_64.lib")
#pragma comment(lib, "Physx/Library/Debug/LowLevelAABB_static_64.lib")
#pragma comment(lib, "Physx/Library/Debug/LowLevelDynamics_static_64.lib")



#else 

#pragma comment(lib, "Physx/Library/Release/PhysX_64.lib")
#pragma comment(lib, "Physx/Library/Release/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "Physx/Library/Release/PhysXCommon_64.lib")
#pragma comment(lib, "Physx/Library/Release/PhysXCooking_64.lib")
#pragma comment(lib, "Physx/Library/Release/PhysXExtensions_static_64.lib")
#pragma comment(lib, "Physx/Library/Release/PhysXFoundation_64.lib")
#pragma comment(lib, "Physx/Library/Release/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "Physx/Library/Release/PhysXTask_static_64.lib")
#pragma comment(lib, "Physx/Library/Release/PhysXVehicle_static_64.lib")
#pragma comment(lib, "Physx/Library/Release/PhysXVehicle2_static_64.lib")
#pragma comment(lib, "Physx/Library/Release/PVDRuntime_64.lib")
#pragma comment(lib, "Physx/Library/Release/SceneQuery_static_64.lib")
#pragma comment(lib, "Physx/Library/Release/SimulationController_static_64.lib")
#pragma comment(lib, "Physx/Library/Release/LowLevel_static_64.lib")
#pragma comment(lib, "Physx/Library/Release/LowLevelAABB_static_64.lib")
#pragma comment(lib, "Physx/Library/Release/LowLevelDynamics_static_64.lib")

#endif //_DEBUG

using namespace physx;

namespace ya
{
	PhysxWrapper::PhysxWrapper()
		: _allocator{},
		_errorCallback{},
		_foundation{ nullptr },
		_physics{ nullptr },
		_dispatcher{ nullptr },
		_currentScene{ nullptr },
		_material{ nullptr },
		_currentInterval{ UpdateInterval::Frame_60 }
	{
		_intervals[static_cast<UINT8>(UpdateInterval::Frame_30)] = 1.f / 30.f;
		_intervals[static_cast<UINT8>(UpdateInterval::Frame_60)] = 1.f / 60.f;
		_intervals[static_cast<UINT8>(UpdateInterval::Frame_120)] = 1.f / 120.f;
		_intervals[static_cast<UINT8>(UpdateInterval::Frame_144)] = 1.f / 144.f;
		_intervals[static_cast<UINT8>(UpdateInterval::Frame_240)] = 1.f / 240.f;
	}

	PhysxWrapper::~PhysxWrapper(void)
	{
		for (auto& scene : _scenes)
		{
			const PxU32 actorCount = scene.second->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
			if (actorCount > 0)
			{
				std::vector<PxRigidActor*> actors(actorCount);
				scene.second->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(actors.data()), actorCount);
				for (const auto& actor : actors)
				{
					GameObject* gameObject = static_cast<GameObject*>(actor->userData);
					if (gameObject)
						gameObject->GetComponent<Collider3D>()->destroyShape();
				}
			}
			PX_RELEASE(scene.second);
		}
		PX_RELEASE(_dispatcher);
		PX_RELEASE(_physics);
#ifdef _DEBUG
		PxCloseExtensions();
		if (_pvd)
		{
			PxPvdTransport* transport = _pvd->getTransport();
			PX_RELEASE(_pvd);
			PX_RELEASE(transport);
		}
#endif
		PX_RELEASE(_foundation);
	}

	PhysxWrapper& PhysxWrapper::getInstance(void)
	{
		static PhysxWrapper __instance{};

		return __instance;
	}

	void PhysxWrapper::Initialize(void)
	{
		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

		bool enable = false;
#ifdef _DEBUG
		enable = true;
		_pvd = PxCreatePvd(*_foundation);
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		_pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
#endif
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, PxTolerancesScale{}, enable, _pvd);

#ifdef _DEBUG
		PxInitExtensions(*_physics, _pvd);
#endif
		constexpr UINT32 kThreadCount = 2;
		_dispatcher = PxDefaultCpuDispatcherCreate(kThreadCount);
		_material = _physics->createMaterial(0.5f, 0.5f, 0.6f);

	}

	void PhysxWrapper::CollisionUpdate()
	{
		if (_currentScene == nullptr)
			return;

		//_currentScene->simulate(_intervals[static_cast<UINT8>(_currentInterval)]);
		_currentScene->simulate(Time::DeltaTime());
		_currentScene->fetchResults(true);

		//SyncGameScene();
	}


	void PhysxWrapper::SyncGameScene(void) const
	{
		const PxU32 actorCount = _currentScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
		if (actorCount == 0)
			return;

		std::vector<PxRigidActor*> actors(actorCount);
		_currentScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(actors.data()), actorCount);

		for (const auto& actor : actors)
		{
			const PxTransform worldTransform = actor->getGlobalPose();

			GameObject* gameObject = static_cast<GameObject*>(actor->userData);
			if (gameObject == nullptr || gameObject->IsDead())
				continue;
			// 충돌체에 대해서 일어난 변화이므로, 이를 Transform에도 반영해야 한다.
			const Collider3D* collider = gameObject->GetComponent<Collider3D>();
			if (collider->isTrigger())
				continue;

			//원래 위치 정보에서 크기정보 제거
			const Matrix from = Matrix::CreateScale(collider->getWorldScale()).Invert() * collider->getWorldMatrix();

			//피직스에서 받아온 트랜스폼 정보를 가져온다
			const Matrix to = MathUtil::pxToMatrix(PxMat44{ worldTransform });

			//차이를 구해준다
			const Matrix diff = from.Invert() * to;

			// 이동은 회전에 영향을 받으므로
			const Quaternion rotation = Quaternion::CreateFromRotationMatrix(diff);
			const Vector3	 position = to.Translation() - Vector3::Transform(from.Translation(), rotation);

			Transform* transform = gameObject->GetComponent<Transform>();
			transform->SetWorldPosition(Vector3::Transform(transform->GetWorldPosition(), rotation) + position);
			transform->SetWorldRotation(transform->GetWorldRotation() * rotation);
		}
	}

	void PhysxWrapper::onTrigger(PxTriggerPair* pairs, PxU32 count)
	{
		for (PxU32 i = 0; i < count; ++i)
		{
			const PxTriggerPair& contactpair = pairs[i];

			if (contactpair.flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
				continue;

			// if these actors do not exist in the scene anymore due to deallocation, do not process
			if (contactpair.triggerActor->userData == nullptr || contactpair.otherActor->userData == nullptr)
				continue;

			GameObject* triggerObject = static_cast<GameObject*>(contactpair.triggerActor->userData);
			assert(triggerObject);
			GameObject* otherObject = static_cast<GameObject*>(contactpair.otherActor->userData);
			assert(otherObject);
			if (triggerObject->IsDead() || otherObject->IsDead())
				continue;

			Collider3D* triggerCollider = triggerObject->GetComponent<Collider3D>();
			Collider3D* otherCollider = otherObject->GetComponent<Collider3D>();

			// process events
			if (contactpair.status & (PxPairFlag::eNOTIFY_TOUCH_FOUND))
			{
				triggerCollider->OnTriggerEnter(otherCollider);
				otherCollider->OnTriggerEnter(triggerCollider);
			}
			else if (contactpair.status & (PxPairFlag::eNOTIFY_TOUCH_LOST))
			{
				triggerCollider->OnTriggerExit(otherCollider);
				otherCollider->OnTriggerExit(triggerCollider);
			}
		}
	}

	void PhysxWrapper::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
	{
		for (PxU32 i = 0; i < nbPairs; ++i)
		{
			const PxContactPair& contactpair = pairs[i];

			// if these actors do not exist in the scene anymore due to deallocation, do not process
			if (pairHeader.actors[0]->userData == nullptr || pairHeader.actors[1]->userData == nullptr)
				continue;

			GameObject* leftGameObject = static_cast<GameObject*>(pairHeader.actors[0]->userData);
			GameObject* rightGameObject = static_cast<GameObject*>(pairHeader.actors[1]->userData);
			if (leftGameObject->IsDead() || rightGameObject->IsDead())
				continue;
			else if (leftGameObject == rightGameObject)
				continue;

			Collider3D* leftCollider = leftGameObject->GetComponent<Collider3D>();
			Collider3D* rightCollider = rightGameObject->GetComponent<Collider3D>();

			PxContactPairPoint collisionPoint{};
			const PxU32		   count = contactpair.extractContacts(&collisionPoint, 1);
			const Vector3	   collisionPosition = MathUtil::pxToVector3(collisionPoint.position);

			// invoke events
			if (contactpair.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				leftCollider->OnCollisionEnter(rightCollider, collisionPosition);
				rightCollider->OnCollisionEnter(leftCollider, collisionPosition);
			}
			else if (contactpair.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			{
				leftCollider->OnCollision(rightCollider, collisionPosition);
				rightCollider->OnCollision(leftCollider, collisionPosition);
			}
			else if (contactpair.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				leftCollider->OnCollisionExit(rightCollider, collisionPosition);
				rightCollider->OnCollisionExit(leftCollider, collisionPosition);
			}
		}
	}

	void PhysxWrapper::EnableRaycast(UINT32 leftLayerIndex, UINT32 rightLayerIndex, bool enable)
	{
		_raycastMask[leftLayerIndex][rightLayerIndex] = enable;
		_raycastMask[rightLayerIndex][leftLayerIndex] = enable;
	}

	void PhysxWrapper::EnableCollision(UINT32 leftLayerIndex, UINT32 rightLayerIndex, bool enable)
	{
		_collisionMask[leftLayerIndex][rightLayerIndex] = enable;
		_collisionMask[rightLayerIndex][leftLayerIndex] = enable;
	}

	void PhysxWrapper::EnableGravity(bool enable, Scene* scene, const Vector3& gravity) const
	{
		//assert(_currentScene);

		auto iter = _scenes.find(scene);
		if (iter != _scenes.end())
		{
			iter->second->setGravity(MathUtil::vector3ToPx(gravity));
		}
	}

	void PhysxWrapper::changeGeometry(Collider3D* collider, physx::PxShape* shape, eColliderType type)
	{
		const Vector3 scale = collider->getWorldScale();
		switch (type)
		{
		case eColliderType::Box:
			shape->setGeometry(PxSphereGeometry{ scale.x });
			break;
		case eColliderType::Sphere:
			shape->setGeometry(PxBoxGeometry{ scale.x, scale.y, scale.z });
			break;
		}
	}

	bool PhysxWrapper::Raycast(UINT32 srcLayerIndex, const Vector3& origin, const Vector3& direction, float maxDistance, RaycastHit* outHit) const
	{

		std::bitset<32> layer{};
		layer[srcLayerIndex] = true;

		const PxHitFlags  hitFlag = PxHitFlag::eDEFAULT;
		PxQueryFilterData filter{};
		filter.data.word0 = _raycastMask[srcLayerIndex].to_ulong();

		PxRaycastBuffer hit{};
		const bool		result = _currentScene->raycast(MathUtil::vector3ToPx(origin), MathUtil::vector3ToPx(direction), maxDistance, hit, hitFlag, filter);

		outHit->gameObject = (hit.hasBlock) ? static_cast<GameObject*>(hit.block.actor->userData) : nullptr;
		outHit->hasBlocking = hit.hasBlock;
		outHit->hitDistance = hit.block.distance;
		outHit->hitPosition = MathUtil::pxToVector3(hit.block.position);
		outHit->hitNormal = MathUtil::pxToVector3(hit.block.normal);

		return result;
	}

	void PhysxWrapper::createScene(Scene* scene)
	{
		assert(scene);
		const auto iter = _scenes.find(scene);
			
			//std::find_if(_scenes.begin(), _scenes.end(), [scene](const PxScene* pxScene) { return pxScene->getName() == scene->GetNameChar(); });

		//이미 생성된 씬이 있을 경우 return
		if (iter != _scenes.end())
		{
			return;
		}

		//assert(iter == _scenes.end());

		PxSceneDesc sceneDescription = PxSceneDesc{ _physics->getTolerancesScale() };
		sceneDescription.gravity = PxVec3{ 0.f, -0.5f, 0.f };
		sceneDescription.cpuDispatcher = _dispatcher;
		sceneDescription.filterShader = &PhysxWrapper::FilterShader;
		sceneDescription.simulationEventCallback = this;

		PxScene* newScene = _physics->createScene(sceneDescription);

		
		newScene->setName(scene->GetNameChar().c_str());

		_scenes.insert(std::make_pair(scene, newScene));
		//_scenes.push_back(newScene);
	}

	void PhysxWrapper::changeScene(Scene* scene)
	{
		const auto iter = _scenes.find(scene);
			//std::find_if(_scenes.begin(), _scenes.end(), [scene](const PxScene* pxScene) {
			//return pxScene->getName() == scene->GetNameChar();
			//});
		//assert(iter != _scenes.end());

		physx::PxScene* pxScene = nullptr;

		if (iter != _scenes.end())
		{
			pxScene = iter->second;
		}
		
		_currentScene = pxScene;
	}


	void PhysxWrapper::createActorSphere(GameObject* gameObject, float radius, PxShape** outShape, bool isStatic)
	{
		Transform* component = gameObject->GetComponent<Transform>();

		auto iter = _scenes.find(gameObject->GetScene());
		assert(_scenes.end() != iter);

		PxTransform t{};
		t.p = MathUtil::vector3ToPx(component->GetWorldPosition());
		t.q = MathUtil::quaternionToPx(component->GetWorldRotationQuaternion());

		if (isStatic)
		{
			PxRigidStatic* staticObject = PxCreateStatic(*_physics, t, PxSphereGeometry{ radius }, *_material);
			//_currentScene->addActor(*staticObject);
			iter->second->addActor(*staticObject);

			PxShape* shapes{};
			const PxU32 count = staticObject->getShapes(&shapes, 1);
			assert(count);

			staticObject->userData = component->GetOwner();
			*outShape = &shapes[0];
			setupFiltering(*outShape, static_cast<UINT32>(gameObject->GetLayerType()));
		}
		else
		{
			PxRigidDynamic* dynamic = PxCreateDynamic(*_physics, t, PxSphereGeometry{ radius }, *_material, _kDefaultDensity);

			iter->second->addActor(*dynamic);
			//currentScene->addActor(*dynamic);
			dynamic->setAngularDamping(100.f);
			PxShape* shapes{};
			const PxU32 count = dynamic->getShapes(&shapes, 1);
			assert(count);
			dynamic->userData = component->GetOwner();
			*outShape = &shapes[0];
			setupFiltering(*outShape, static_cast<UINT32>(gameObject->GetLayerType()));
		}
	}

	void PhysxWrapper::createActorCapsule(GameObject* gameObject, float radius, float height, physx::PxShape** outShape, bool isStatic)
	{
		Transform* component =gameObject->GetComponent<Transform>();

		auto iter = _scenes.find(gameObject->GetScene());
		assert(_scenes.end() != iter);

		PxTransform t{};
		t.p = *outShape ? MathUtil::vector3ToPx(component->GetWorldPosition()) : MathUtil::vector3ToPx(component->GetLocalPosition());
		t.q = *outShape ? MathUtil::quaternionToPx(component->GetWorldRotationQuaternion()) : MathUtil::quaternionToPx(component->GetLocalRotationQuaternion());

		if (isStatic)
		{
			PxRigidStatic* staticObject = PxCreateStatic(*_physics, t, PxCapsuleGeometry{ radius, height }, *_material);
			//_currentScene->addActor(*staticObject);
			iter->second->addActor(*staticObject);

			PxShape* shapes{};
			const PxU32 count = staticObject->getShapes(&shapes, 1);
			assert(count);
			staticObject->userData = component->GetOwner();
			*outShape = &shapes[0];
			setupFiltering(*outShape, static_cast<UINT32>(gameObject->GetLayerType()));
		}
		else
		{
			PxRigidDynamic* dynamic = PxCreateDynamic(*_physics, t, PxCapsuleGeometry{ radius, height }, *_material, _kDefaultDensity);
			dynamic->setAngularDamping(100.f);

			iter->second->addActor(*dynamic);
			//_currentScene->addActor(*dynamic);

			PxShape* shapes{};
			const PxU32 count = dynamic->getShapes(&shapes, 1);
			assert(count);

			dynamic->userData = component->GetOwner();
			*outShape = &shapes[0];
			setupFiltering(*outShape, static_cast<UINT32>(gameObject->GetLayerType()));
		}
	}

	void PhysxWrapper::createActorCube(GameObject* gameObject, const Vector3& halfExtents, PxShape** outShape, bool isStatic)
	{
		Transform* component = gameObject->GetComponent<Transform>();

		auto iter = _scenes.find(gameObject->GetScene());
		assert(_scenes.end() != iter);



		PxTransform t{};
		t.p = *outShape ? 
			MathUtil::vector3ToPx(component->GetWorldPosition()) : MathUtil::vector3ToPx(component->GetLocalPosition());
		t.q = *outShape ? MathUtil::quaternionToPx(component->GetWorldRotationQuaternion()) : MathUtil::quaternionToPx(component->GetLocalRotationQuaternion());

		if (isStatic)
		{
			PxRigidStatic* staticObject = PxCreateStatic(*_physics, t, PxBoxGeometry{ halfExtents.x, halfExtents.y, halfExtents.z }, *_material);
			//targetScene->addActor(*staticObject);
			iter->second->addActor(*staticObject);

			PxShape* shapes{};
			const PxU32 count = staticObject->getShapes(&shapes, 1);
			assert(count);
			staticObject->userData = component->GetOwner();
			*outShape = &shapes[0];
			setupFiltering(*outShape, static_cast<UINT32>(gameObject->GetLayerType()));
		}
		else
		{
			PxRigidDynamic* dynamic = PxCreateDynamic(*_physics, t, PxBoxGeometry{ halfExtents.x, halfExtents.y, halfExtents.z }, *_material, _kDefaultDensity);
			//_currentScene->addActor(*dynamic);
			iter->second->addActor(*dynamic);

			PxShape* shapes{};
			const PxU32 count = dynamic->getShapes(&shapes, 1);
			assert(count);

			dynamic->userData = component->GetOwner();
			*outShape = &shapes[0];
			setupFiltering(*outShape, static_cast<UINT32>(gameObject->GetLayerType()));
		}
	}

	PxFilterFlags PhysxWrapper::FilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags,
		const void* constantBlock, physx::PxU32 constantBlockSize)
	{
		// let triggers through
		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1))
				pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			else
				return PxFilterFlag::eKILL;

			return PxFilterFlag::eDEFAULT;
		}
		// generate contacts for all that were not filtered above
		// trigger the contact callback for pairs (A,B) where
		// the filtermask of A contains the ID of B or vice versa.
		if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1))
		{
			pairFlags = PxPairFlag::eCONTACT_DEFAULT;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_TOUCH_LOST | PxPairFlag::eNOTIFY_CONTACT_POINTS;
			return PxFilterFlag::eDEFAULT;
		}

		return PxFilterFlag::eKILL;
	}

	void PhysxWrapper::setupFiltering(PxShape* shape, UINT32 layerIndex) const
	{
		std::bitset<32> layer{};

		layer[layerIndex] = true;

		PxFilterData filterData{};
		filterData.word0 = layer.to_ulong();					  // word0 = own ID
		filterData.word1 = _collisionMask[layerIndex].to_ulong(); // word1 = ID mask to filter pairs that trigger a contact callback
		shape->setSimulationFilterData(filterData);

		PxFilterData queryFilterData{};
		queryFilterData.word0 = layer.to_ulong();					   // word0 = own ID
		queryFilterData.word1 = _raycastMask[layerIndex].to_ulong(); // word1 = ID mask to filter pairs that trigger a contact callback
		shape->setQueryFilterData(queryFilterData);
	}

#pragma region NOT IMPLEMENTED
	void PhysxWrapper::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
	{
	}

	void PhysxWrapper::onWake(PxActor** actors, PxU32 count)
	{
	}

	void PhysxWrapper::onSleep(PxActor** actors, PxU32 count)
	{
	}

	void PhysxWrapper::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
	{
	}
#pragma endregion
} // namespace pa 