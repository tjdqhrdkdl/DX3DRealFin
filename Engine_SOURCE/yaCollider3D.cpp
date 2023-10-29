#include "yaCollider3D.h"
#include "yaTransform.h"

#include "PhysxWrapper.h"

//#include "Engine/Base/Types.h"
#include "yaApplication.h"
#include "yaResources.h"
#include "yaGameObject.h"
#include "yaMath.h"
#include "MathUtil.h"
#include "yaConstantBuffer.h"


namespace ya
{
	UINT32 Collider3D::_idGenerator = 0u;

	Collider3D::Collider3D()
		: Collider{ eComponentType::Collider }
		, _id{ _idGenerator++ }
		, _type{ eColliderType::Sphere }
		, _offsetScale{ 1.f, 1.f, 1.f }
		, _collisionCount{ 0 }
		, _shape{ nullptr }
		, _isTrigger{ false }
		, _isGravityEnabled(true)
		, _otherOverlapping{ nullptr }
		, _freezeRotationFlag{ FreezeRotationFlag::END }
		, _enableDraw{ true }
		, _mass(0.6f)
		, _restitution(0.f)
		, _syncScaleToTransform(true)
		, _staticFriction(180.f)
		, _dynamicFriction(180.f)
		, _maxVelocity(40.f)
		, _isStatic(false)
	{
	}

	Collider3D::~Collider3D()
	{
		if (_shape)
		{
			physx::PxActor* actor = _shape->getActor();
			actor->userData = nullptr;
			actor->release();
			_shape = nullptr;
		}
	}

	void Collider3D::Initialize()
	{
	}

	void Collider3D::Start()
	{
		if (_syncScaleToTransform)
		{
			Transform* tr = GetOwner()->GetComponent<Transform>();
			_offsetScale *= tr->GetWorldScale();
		}
		Vector3 realScale = _offsetScale * 0.5f;
		

		if (_shape == nullptr)
		{
			if (_type == eColliderType::Box)
				PhysxWrapper::getInstance().createActorCube(GetOwner(), realScale, &_shape, _isStatic);
			else if (_type == eColliderType::Sphere)
				PhysxWrapper::getInstance().createActorSphere(GetOwner(), realScale.x, &_shape, _isStatic);
		}
		else if (_shape)
		{
			PhysxWrapper::getInstance().changeGeometry(this, _shape, _type);
		}


		EnableGravity(_isGravityEnabled);
		SetMass(_mass);
		SetRestitution(_restitution);
		EnableGravity(_isGravityEnabled);
		SetStaticFriction(_staticFriction);
		SetDynamicFriction(_dynamicFriction);
		SetLimitVelocity(_maxVelocity);
		setFreezeRotation(_freezeRotationFlag.Get());

		if (_isStatic)
		{
			UpdateMatrix();
			SyncPhysX();
		}
	}


	void Collider3D::Render()
	{
		//_positionBufferData.world = _worldMatrix;

		////const Camera& camera = *(renderer::cameras[(UINT)eSceneType::Play]);
		//_positionBufferData.view = Camera::GetGpuViewMatrix();
		//_positionBufferData.projection = Camera::GetGpuProjectionMatrix();
		//_positionBuffer->SetData(&_positionBufferData);
		//_positionBuffer->Bind(eShaderStage::ALL);

		//_wireFrameData.collisionCount = _collisionCount;
		//_wireFrameBuffer->SetData(&_wireFrameData);
		//_wireFrameBuffer->Bind(eShaderStage::PS);

		//_shader->Binds();
		//_mesh->Render(0u);

	}

	void Collider3D::CollisionUpdate()
	{
		assert(_type != eColliderType::End);

		//static일 경우 scale 설정은 최초 한번만
		if (false == _isStatic)
		{
			UpdateMatrix();
			SyncPhysX();
		}
	}

	void Collider3D::CollisionLateUpdate()
	{
		FetchPhysX();
	}



	void Collider3D::SetSize(Vector3 offset)
	{
		_offsetScale = offset;
		
		if (_type == eColliderType::Sphere)
		{
			_offsetScale.x = _offsetScale.y = _offsetScale.z;

			if (_shape)
			{
				Vector3 realScale = GetOwner()->GetComponent<Transform>()->GetWorldScale() * _offsetScale * 0.5f;
				physx::PxGeometryHolder holder{ _shape->getGeometry() };
				holder.sphere().radius = realScale.x;
				_shape->setGeometry(holder.sphere());
			}
		}
		else
		{
			if (_shape)
			{
				Vector3 realScale = GetOwner()->GetComponent<Transform>()->GetWorldScale() * _offsetScale * 0.5f;
				physx::PxGeometryHolder holder{ _shape->getGeometry() };
				holder.box().halfExtents = MathUtil::vector3ToPx(realScale);
				_shape->setGeometry(holder.box());
			}
		}
	}

	void Collider3D::OnCollisionEnter(Collider3D* other, const Vector3& collisionPosition)
	{
		++_collisionCount;

		const auto& scripts = GetOwner()->GetScripts();
		for (auto& script : scripts)
		{
			script->OnCollisionEnter(other->GetOwner(), collisionPosition);
		}
	}

	void Collider3D::OnCollision(Collider3D* other, const Vector3& collisionPosition)
	{
		const auto& scripts = GetOwner()->GetScripts();
		for (auto& script : scripts)
		{
			script->OnCollisionStay(other->GetOwner(), collisionPosition);
		}
	}

	void Collider3D::OnCollisionExit(Collider3D* other, const Vector3& collisionPosition)
	{
		--_collisionCount;
		const auto& scripts = GetOwner()->GetScripts();
		for (auto& script : scripts)
		{
			script->OnCollisionExit(other->GetOwner(), collisionPosition);
		}
	}

	void Collider3D::OnTriggerEnter(Collider3D* other)
	{
		_otherOverlapping = other->GetOwner();
		const auto& scripts = GetOwner()->GetScripts();
		for (auto& script : scripts)
		{
			script->OnTriggerEnter(other->GetOwner());
		}

		_wireFrameData.isTrigger = true;
	}

	void Collider3D::OnTriggerExit(Collider3D* other)
	{
		const auto& scripts = GetOwner()->GetScripts();
		for (auto& script : scripts)
		{
			script->OnTriggerExit(other->GetOwner());
		}

		_otherOverlapping = nullptr;
		_wireFrameData.isTrigger = false;
	}

	void Collider3D::setTrigger(bool enable)
	{
		_isTrigger = enable;
		assert(_type != eColliderType::End);

		physx::PxShapeFlags flags = _shape->getFlags();

		if (enable)
		{
			flags &= ~physx::PxShapeFlag::eSIMULATION_SHAPE;
			flags |= physx::PxShapeFlag::eTRIGGER_SHAPE;
		}
		else
		{
			flags &= ~physx::PxShapeFlag::eTRIGGER_SHAPE;
			flags |= physx::PxShapeFlag::eSIMULATION_SHAPE;
		}

		_shape->setFlags(flags);
	}

	void Collider3D::SceneChanged()
	{
		if (_shape)
		{
			physx::PxActor* actor = _shape->getActor();
			actor->userData = nullptr;
			actor->release();
			_shape = nullptr;
		}
		SetType(GetCollider3DType(), _isStatic);
	}

	void Collider3D::EnableGravity(bool enable)
	{
		_isGravityEnabled = enable;

		if (_shape)
		{
			physx::PxActor* actor = _shape->getActor();

			auto flags = actor->getActorFlags();
			if (_isGravityEnabled)
				flags &= ~physx::PxActorFlag::eDISABLE_GRAVITY;
			else
				flags |= physx::PxActorFlag::eDISABLE_GRAVITY;

			actor->setActorFlags(flags);
		}
	}

	bool Collider3D::isOverlapping(GameObject* other, Vector3* outDistance)
	{
		physx::PxVec3 direction{};
		physx::PxF32  depth{};

		Collider3D* otherCollider = other->GetComponent<Collider3D>();
		const bool		  isPenetrating = physx::PxGeometryQuery::computePenetration(direction, depth,
			_shape->getGeometry(),
			_shape->getActor()->getGlobalPose(),
			otherCollider->getShape()->getGeometry(),
			otherCollider->getShape()->getActor()->getGlobalPose());

		if (outDistance && isPenetrating)
		{
			const physx::PxVec3 result = direction * depth;
			*outDistance = MathUtil::pxToVector3(result);
		}

		return isPenetrating;
	}

	void Collider3D::setFreezeRotation(EnumFlags<FreezeRotationFlag> flag)
	{
		_freezeRotationFlag = flag;

		if (_shape)
		{
			physx::PxActor* actor = _shape->getActor();
			assert(actor);

			physx::PxRigidDynamic* rigidActor = actor->is<physx::PxRigidDynamic>();
			if (rigidActor == nullptr)
				return;

			physx::PxRigidDynamicLockFlags pxFlag{};

			if (_freezeRotationFlag & FreezeRotationFlag::ROTATION_X)
				pxFlag |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X;
			if (_freezeRotationFlag & FreezeRotationFlag::ROTATION_Y)
				pxFlag |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y;
			if (_freezeRotationFlag & FreezeRotationFlag::ROTATION_Z)
				pxFlag |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;


			rigidActor->setRigidDynamicLockFlags(pxFlag);

			rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, true);
			rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, true);
		}


	}

	bool Collider3D::hasFlag(FreezeRotationFlag flag) const
	{
		return false;
	}

	void Collider3D::AddForce(const Vector3& force)
	{
		if (_shape)
		{
			physx::PxActor* actor = _shape->getActor();
			if (actor)
			{
				physx::PxRigidBody* rigidActor = actor->is<physx::PxRigidBody>();
				if (rigidActor)
				{
					rigidActor->addForce(MathUtil::vector3ToPx(force), physx::PxForceMode::eACCELERATION);
					//Transform* transform = GetOwner()->GetComponent<Transform>();
					//physx::PxRigidBodyExt::addForceAtPos(*rigidActor, MathUtil::vector3ToPx(force), MathUtil::vector3ToPx(transform->GetWorldPosition()));
				}
			}
		}
	}

	void Collider3D::ClearForce()
	{
		if (_shape)
		{
			physx::PxActor* actor = _shape->getActor();
			if (actor)
			{
				physx::PxRigidBody* rigidActor = actor->is<physx::PxRigidBody>();
				if (rigidActor)
				{
					rigidActor->clearForce();
				}
			}
		}
	}


	void Collider3D::SetMass(float mass)
	{
		_mass = mass;
		if (_shape)
		{
			physx::PxRigidBody* rigidBody = _shape->getActor()->is<physx::PxRigidBody>();
			if (rigidBody)
			{
				rigidBody->setMass(_mass);
			}
		}
	}

	void Collider3D::SetRestitution(float restitution)
	{
		_restitution = restitution;
		if (_shape)
		{
			physx::PxMaterial* mtrl{};
			_shape->getMaterials(&mtrl, 1u, 0u);
			if (mtrl)
			{
				mtrl->setRestitution(_restitution);
			}
		}
	}

	void Collider3D::SetVelocity(const Vector3& _velocity)
	{
		if (_shape)
		{
			physx::PxRigidDynamic* dynamicRigid = _shape->getActor()->is<physx::PxRigidDynamic>();
			if (dynamicRigid)
			{
				dynamicRigid->setLinearVelocity(MathUtil::vector3ToPx(_velocity));
			}
		}
	}

	Vector3 Collider3D::GetVelocity() const
	{
		Vector3 retVec{};

		if (_shape)
		{
			physx::PxRigidDynamic* dynamicRigid = _shape->getActor()->is<physx::PxRigidDynamic>();
			if (dynamicRigid)
			{
				retVec = MathUtil::pxToVector3(dynamicRigid->getLinearVelocity());
			}
		}

		return retVec;
	}

	void Collider3D::SetStaticFriction(float staticFriction)
	{
		_staticFriction = staticFriction;
		if (_shape)
		{
			physx::PxMaterial* mtrl{};
			_shape->getMaterials(&mtrl, 1u, 0u);
			if (mtrl)
			{
				mtrl->setStaticFriction(_staticFriction);
			}
		}
	}


	void Collider3D::SetDynamicFriction(float dynamicFriction)
	{
		_dynamicFriction = dynamicFriction;
		if (_shape)
		{
			physx::PxMaterial* mtrl{};
			_shape->getMaterials(&mtrl, 1u, 0u);
			if (mtrl)
			{
				mtrl->setDynamicFriction(_dynamicFriction);
			}
		}
	}

	void Collider3D::SetLimitVelocity(float maxVelocity)
	{
		_maxVelocity = maxVelocity;
		if (_shape)
		{
			physx::PxRigidDynamic* dynamicRigid = _shape->getActor()->is<physx::PxRigidDynamic>();
			if (dynamicRigid)
			{
				dynamicRigid->setMaxLinearVelocity(maxVelocity);
			}
		}
	}

	void Collider3D::SetKinematic(bool enable)
	{
		if (_shape)
		{
			physx::PxRigidBody* rigid = _shape->getActor()->is<physx::PxRigidBody>();
			if (rigid)
			{
				rigid->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, enable);
			}
		}
	}



	void Collider3D::SyncPhysX()
	{
		if (GetOwner()->GetName() == L"childObj")
		{
			int a = 3;
		}

		physx::PxActor* actor = _shape->getActor();
		assert(actor);
		physx::PxRigidActor* rigidActor = actor->is<physx::PxRigidActor>();
		assert(rigidActor);

		physx::PxTransformT<float> transform{};
		memcpy(&transform.p, _worldMatrix.m[3], sizeof(float) * 3);

		Quaternion q = GetOwner()->GetComponent<Transform>()->GetWorldRotationQuaternion();
		transform.q = MathUtil::quaternionToPx(q);
			
		rigidActor->setGlobalPose(transform);
	}
	void Collider3D::FetchPhysX()
	{
		physx::PxActor* actor = _shape->getActor();
		assert(actor);

		physx::PxRigidActor* rigidActor = actor->is<physx::PxRigidActor>();
		assert(rigidActor);

		physx::PxTransform worldTransform = rigidActor->getGlobalPose();

		if (false == _isStatic)
		{
			Matrix before = Matrix::CreateScale(_offsetScale).Invert() * _worldMatrix;
			Matrix after = MathUtil::pxToMatrix(physx::PxMat44(worldTransform));
			Matrix diff = before.Invert() * after;

			Quaternion diffQuat = Quaternion::CreateFromRotationMatrix(diff);

			Vector3 diffPos = diff.Translation();

			if (GetOwner()->GetName() == L"childObj")
			{
				int a = 3;
			}

			GetOwner()->GetComponent<Transform>()->FetchPhysX(diffQuat, diffPos);
		}

		//Debug Render
		DebugMesh meshAttribute = {};

		meshAttribute.type = _type;

		meshAttribute.scale = _offsetScale;

		meshAttribute.position = MathUtil::pxToVector3(worldTransform.p);
		//meshAttribute.position += _offsetPosition;

		meshAttribute.rotation = MathUtil::pxToQuaternion(worldTransform.q).ToEulerXYZOrder() * gRadianToDegreeFactor;
			
		meshAttribute.parent = nullptr;

		meshAttribute.collisionCount = _collisionCount;
		meshAttribute.isTrigger = _isTrigger;

		renderer::debugMeshes.push_back(meshAttribute);
	}
	void Collider3D::UpdateMatrix()
	{
		if (GetOwner()->GetName() == L"childObj")
		{
			int a = 3;
		}

		//이동은 크기에 영향을 받으므로 우선 크기 정보를 반영해서 Matrix를 만들어줘야 한다.
		_localMatrix = Matrix::CreateScale(_offsetScale);
		_localMatrix *= Matrix::CreateTranslation(_offsetPosition);

		//트랜스폼의 크기 정보는 제거
		const Vector3 objectScale = GetOwner()->GetComponent<Transform>()->GetWorldScale();
		const Matrix objectScaleInvMatrix = Matrix::CreateScale(objectScale).Invert();

		const Matrix  objectWorldMatrix = GetOwner()->GetComponent<Transform>()->GetWorldMatrix();
		// 충돌체 상대행렬 * 오브젝트 월드 크기 역행렬 * 오브젝트 월드 행렬(크기 * 회전 * 이동)
		//트랜스폼의 Scale은 반영하지 않는다
		_worldMatrix = _localMatrix * objectScaleInvMatrix * objectWorldMatrix;
	}
} // namespace ya
