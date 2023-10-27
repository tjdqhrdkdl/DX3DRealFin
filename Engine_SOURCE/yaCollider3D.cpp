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
		, _otherOverlapping{ nullptr }
		, _freezeRotationFlag{ FreezeRotationFlag::END }
		, _enableDraw{ true }
		, _mass(100.f)
		, _restitution(0.f)
		, _syncScaleToTransform(true)
		//, _positionBuffer()
		//, _wireFrameBuffer()
	{
		//_positionBuffer = renderer::constantBuffers[(UINT)eCBType::Transform];
		//_wireFrameBuffer = renderer::constantBuffers[(UINT)eCBType::WireFrame];
		//_mesh = Resources::Find<Mesh>(strKeys::mesh::CubeMesh);
		//_shader = Resources::Find<Shader>(strKeys::shader::PhysXDebugShader);
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
			_offsetScale = tr->GetWorldScale();
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

		setMass(_mass);
		setRestitution(_restitution);
		EnableGravity(_isGravityEnabled);
		
		//_mesh = (_type == eColliderType::Box) ? Resources::Find<Mesh>(strKeys::mesh::CubeMesh) : Resources::Find<Mesh>(strKeys::mesh::SphereMesh);
	}

	void Collider3D::Update()
	{
		assert(_type != eColliderType::End);

		const Matrix translation = Matrix::CreateTranslation(_offsetPosition);
		const Matrix scale = Matrix::CreateScale(_offsetScale);
		_worldMatrix = scale * translation;

		const Vector3 objectScale = GetOwner()->GetComponent<Transform>()->GetWorldScale();
		const Matrix objectScaleInvMatrix = Matrix::CreateScale(objectScale).Invert();
		const Matrix  objectWorldMatrix = GetOwner()->GetComponent<Transform>()->GetWorldMatrix();
		// 충돌체 상대행렬 * 오브젝트 월드 크기 역행렬 * 오브젝트 월드 행렬(크기 * 회전 * 이동)
		//트랜스폼의 Scale은 반영하지 않는다
		_worldMatrix = _worldMatrix * objectScaleInvMatrix * objectWorldMatrix;

		syncPhysics();

		//Debug Render
		DebugMesh meshAttribute = {};

		meshAttribute.type = _type;

		Transform* tr = GetOwner()->GetComponent<Transform>();
		meshAttribute.scale = _offsetScale;

		meshAttribute.position = Vector3(_worldMatrix.m[3]);
		//meshAttribute.position += _offsetPosition;

		meshAttribute.rotation = tr->GetWorldRotation();

		meshAttribute.parent = nullptr;

		meshAttribute.collisionCount = _collisionCount;
		meshAttribute.isTrigger = _isTrigger;

		renderer::debugMeshes.push_back(meshAttribute);
	}

	void Collider3D::FixedUpdate()
	{
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

	void Collider3D::setFreezeRotation(FreezeRotationFlag flag, bool enable)
	{
		physx::PxActor* actor = _shape->getActor();
		assert(actor);

		physx::PxRigidDynamic* rigidActor = actor->is<physx::PxRigidDynamic>();
		if (rigidActor == nullptr)
			return;

		EnumFlags<FreezeRotationFlag, UINT16> enumFlag{ flag };

		if (enable)
		{
			_freezeRotationFlag |= enumFlag;
			if (enumFlag & FreezeRotationFlag::ROTATION_X)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
			if (enumFlag & FreezeRotationFlag::ROTATION_Y)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
			if (enumFlag & FreezeRotationFlag::ROTATION_Z)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
		}
		else
		{
			_freezeRotationFlag &= ~enumFlag;
			if (enumFlag & FreezeRotationFlag::ROTATION_X)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
			if (enumFlag & FreezeRotationFlag::ROTATION_Y)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);
			if (enumFlag & FreezeRotationFlag::ROTATION_Z)
				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);
		}

		rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, true);
		rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, true);
	}

	bool Collider3D::hasFlag(FreezeRotationFlag flag) const
	{
		return false;
	}

	void Collider3D::addForce(const Vector3& force)
	{
		physx::PxActor* actor = _shape->getActor();
		assert(actor);
		physx::PxRigidBody* rigidActor = actor->is<physx::PxRigidBody>();
		assert(rigidActor);

		Transform* transform = GetOwner()->GetComponent<Transform>();
		physx::PxRigidBodyExt::addForceAtPos(*rigidActor, MathUtil::vector3ToPx(force), MathUtil::vector3ToPx(transform->GetWorldPosition()));
	}

	void Collider3D::setMass(float mass)
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

	float Collider3D::getMass(void) const
	{
		return _shape->getActor()->is<physx::PxRigidBody>()->getMass();
	}

	void Collider3D::setRestitution(float restitution)
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

	void Collider3D::syncPhysics()
	{
		physx::PxActor* actor = _shape->getActor();
		assert(actor);
		if (actor->is<physx::PxRigidActor>())
		{
			physx::PxRigidActor* rigidActor = static_cast<physx::PxRigidActor*>(actor);
			if (rigidActor->is<physx::PxRigidStatic>())
				return;

			//Matrix		   withoutScale = Matrix::CreateScale(getWorldScale()).Invert() * _worldMatrix;
			//physx::PxMat44 matrix{};
			//std::memcpy(&matrix, &withoutScale, sizeof(Matrix));


			physx::PxTransformT<float> transform{};
			memcpy(&transform.p, _worldMatrix.m[3], sizeof(float) * 3);

			Quaternion q = GetOwner()->GetComponent<Transform>()->GetWorldRotationQuaternion();
			transform.q = MathUtil::quaternionToPx(q);
			//memcpy(&transform.q, &q, sizeof(Quaternion));
			
			rigidActor->setGlobalPose(transform);
		}
	}
} // namespace ya
