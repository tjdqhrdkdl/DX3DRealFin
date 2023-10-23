//#include "yaCollider3D.h"
//#include "yaTransform.h"
//
//#include "PhysxWrapper.h"
//
////#include "Engine/Base/Types.h"
//#include "yaApplication.h"
//#include "yaResources.h"
//#include "yaGameObject.h"
//#include "yaMath.h"
//#include "MathUtil.h"
//#include "yaConstantBuffer.h"
//
//namespace ya
//{
//	Collider3D::Collider3D()
//		: Component{ eComponentType::Collider }
//		, _id{ _idGenerator++ }
//		, _type{ eColliderType::Sphere }
//		, _offsetScale{ 1.f, 1.f, 1.f }
//		, _collisionCount{ 0 }
//		, _shape{ nullptr }
//		, _isTrigger{ false }
//		, _otherOverlapping{ nullptr }
//		, _freezeRotationFlag{ FreezeRotationFlag::END }
//		, _enableDraw{ true }
//		, _positionBuffer()
//		, _wireFrameBuffer()
//	{
//		_positionBuffer = renderer::constantBuffers[(UINT)eCBType::Transform];
//		//_wireFrameBuffer = renderer::constantBuffers[(UINT)eCBType::Transform];
//		_mesh = Resources::Find<Mesh>(L"SphereMesh");
//		//_shader = ResourceManager::getGraphicShader("WIRE_FRAME");
//	}
//
//	Collider3D::~Collider3D()
//	{
//		if (_shape)
//		{
//			physx::PxActor* actor = _shape->getActor();
//			actor->userData = nullptr;
//			actor->release();
//			_shape = nullptr;
//		}
//	}
//
//	void Collider3D::Initialize()
//	{
//	}
//
//	void Collider3D::Update()
//	{
//		assert(_type != eColliderType::End);
//
//		const Matrix translation = Matrix::CreateTranslation(_offsetPosition);
//		const Matrix rotation = Matrix{};
//		const Matrix scale = Matrix::CreateScale(_offsetScale);
//		_worldMatrix = scale * rotation * translation;
//
//		const Vector3 objectScale = GetOwner()->GetComponent<Transform>()->GetLocalScale();
//		const Matrix  inverseObjectScale = Matrix::CreateScale(objectScale).Invert();
//		const Matrix  objectWorldMatrix = GetOwner()->GetComponent<Transform>()->GetWorldMatrix();
//		// 충돌체 상대행렬 * 오브젝트 월드 크기 역행렬 * 오브젝트 월드 행렬(크기 * 회전 * 이동)
//		_worldMatrix = _worldMatrix * inverseObjectScale * objectWorldMatrix;
//
//		syncPhysics();
//	}
//
//	void Collider3D::FixedUpdate()
//	{
//	}
//
//	void Collider3D::Render()
//	{
//		_positionBufferData.world = _worldMatrix;
//
//		//const Camera& camera = *(renderer::cameras[(UINT)eSceneType::Play]);
//		_positionBufferData.view = Camera::GetGpuViewMatrix();
//		_positionBufferData.projection = Camera::GetGpuProjectionMatrix();
//		_positionBuffer->SetData(&_positionBufferData);
//		_positionBuffer->Bind(eShaderStage::ALL);
//
//		//_wireFrameData.collisionCount = _collisionCount;
//		//_wireFrameBuffer->uploadData(&_wireFrameData, sizeof(cbWireFrame));
//		//_wireFrameBuffer->bindToPipeline(ConstBufferType::cbWireFrame);
//
//		_shader->Binds();
//		_mesh->Render(0u);
//	}
//
//	void Collider3D::setType(eColliderType type, bool isStatic)
//	{
//		_isStatic = isStatic;
//
//		if (_shape == nullptr)
//		{
//			_type = type;
//			if (_type == eColliderType::Box)
//				PhysxWrapper::getInstance().createActorCube(GetOwner(), _offsetScale, &_shape, _isStatic);
//			else if (_type == eColliderType::Sphere)
//				PhysxWrapper::getInstance().createActorSphere(GetOwner(), _offsetScale.x, &_shape, _isStatic);
//		}
//		else if (_shape && _type != type)
//		{
//			_type = type;
//			PhysxWrapper::getInstance().changeGeometry(this, _shape, _type);
//		}
//
//		_mesh = (_type == eColliderType::Box) ? ResourceManager::getMesh("Box") : ResourceManager::getMesh("SphereMesh");
//	}
//
//	void Collider3D::setOffsetScale(Vector3 offset)
//	{
//		assert(_shape);
//
//		_offsetScale = offset;
//		if (_type == eColliderType::Sphere)
//		{
//			_offsetScale.x = _offsetScale.y = _offsetScale.z;
//
//			physx::PxGeometryHolder holder{ _shape->getGeometry() };
//			holder.sphere().radius = _offsetScale.x;
//			_shape->setGeometry(holder.sphere());
//		}
//		else
//		{
//			physx::PxGeometryHolder holder{ _shape->getGeometry() };
//			holder.box().halfExtents = MathUtil::vector3ToPx(_offsetScale);
//			_shape->setGeometry(holder.box());
//		}
//	}
//
//	void Collider3D::onCollisionEnter(Collider3D* other, const Vector3& collisionPosition)
//	{
//		++_collisionCount;
//
//		const auto& scripts = GetOwner()->GetScripts();
//		for (auto& script : scripts)
//		{
//			script->OnCollisionEnter(&other->GetOwner(), collisionPosition);
//		}
//	}
//
//	void Collider3D::onCollision(Collider3D* other, const Vector3& collisionPosition)
//	{
//		const auto& scripts = getOwner().getScripts();
//		for (auto& script : scripts)
//		{
//			script->onCollision(&other->getOwner(), collisionPosition);
//		}
//	}
//
//	void Collider3D::onCollisionExit(Collider3D* other, const Vector3& collisionPosition)
//	{
//		--_collisionCount;
//		const auto& scripts = getOwner().getScripts();
//		for (auto& script : scripts)
//		{
//			script->onCollisionExit(&other->getOwner(), collisionPosition);
//		}
//	}
//
//	void Collider3D::onTriggerEnter(Collider3D* other)
//	{
//		_otherOverlapping = &other->getOwner();
//		const auto& scripts = getOwner().getScripts();
//		for (auto& script : scripts)
//		{
//			script->onTriggerEnter(&other->getOwner());
//		}
//
//		_wireFrameData.isTrigger = true;
//	}
//
//	void Collider3D::onTriggerExit(Collider3D* other)
//	{
//		const auto& scripts = getOwner().getScripts();
//		for (auto& script : scripts)
//		{
//			script->onTriggerExit(&other->getOwner());
//		}
//
//		_otherOverlapping = nullptr;
//		_wireFrameData.isTrigger = false;
//	}
//
//	void Collider3D::setTrigger(bool enable)
//	{
//		_isTrigger = enable;
//		assert(_type != ColliderType::END);
//
//		physx::PxShapeFlags flags = _shape->getFlags();
//
//		if (enable)
//		{
//			flags &= ~physx::PxShapeFlag::eSIMULATION_SHAPE;
//			flags |= physx::PxShapeFlag::eTRIGGER_SHAPE;
//		}
//		else
//		{
//			flags &= ~physx::PxShapeFlag::eTRIGGER_SHAPE;
//			flags |= physx::PxShapeFlag::eSIMULATION_SHAPE;
//		}
//
//		_shape->setFlags(flags);
//	}
//
//	void Collider3D::enableGravity(bool enable)
//	{
//		assert(_shape);
//		_isGravityEnabled = enable;
//
//		physx::PxActor* actor = _shape->getActor();
//
//		auto flags = actor->getActorFlags();
//		if (_isGravityEnabled)
//			flags &= ~physx::PxActorFlag::eDISABLE_GRAVITY;
//		else
//			flags |= physx::PxActorFlag::eDISABLE_GRAVITY;
//
//		actor->setActorFlags(flags);
//	}
//
//	bool Collider3D::isOverlapping(const GameObject* other, Vector3* outDistance) const
//	{
//		physx::PxVec3 direction{};
//		physx::PxF32  depth{};
//
//		const Collider3D* otherCollider = other->getComponent<Collider3D>();
//		const bool		  isPenetrating = physx::PxGeometryQuery::computePenetration(direction, depth,
//			_shape->getGeometry(),
//			_shape->getActor()->getGlobalPose(),
//			otherCollider->getShape()->getGeometry(),
//			otherCollider->getShape()->getActor()->getGlobalPose());
//
//		if (outDistance && isPenetrating)
//		{
//			const physx::PxVec3 result = direction * depth;
//			*outDistance = MathUtil::pxToVector3(result);
//		}
//
//		return isPenetrating;
//	}
//
//	void Collider3D::setFreezeRotation(FreezeRotationFlag flag, bool enable)
//	{
//		physx::PxActor* actor = _shape->getActor();
//		assert(actor);
//
//		physx::PxRigidDynamic* rigidActor = actor->is<physx::PxRigidDynamic>();
//		if (rigidActor == nullptr)
//			return;
//
//		EnumFlags<FreezeRotationFlag, UINT16> enumFlag{ flag };
//
//		if (enable)
//		{
//			_freezeRotationFlag |= enumFlag;
//			if (enumFlag & FreezeRotationFlag::ROTATION_X)
//				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
//			if (enumFlag & FreezeRotationFlag::ROTATION_Y)
//				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
//			if (enumFlag & FreezeRotationFlag::ROTATION_Z)
//				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
//		}
//		else
//		{
//			_freezeRotationFlag &= ~enumFlag;
//			if (enumFlag & FreezeRotationFlag::ROTATION_X)
//				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
//			if (enumFlag & FreezeRotationFlag::ROTATION_Y)
//				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);
//			if (enumFlag & FreezeRotationFlag::ROTATION_Z)
//				rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);
//		}
//
//		rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, true);
//		rigidActor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, true);
//	}
//
//	bool Collider3D::hasFlag(FreezeRotationFlag flag) const
//	{
//		return false;
//	}
//
//	void Collider3D::addForce(const Vector3& force)
//	{
//		physx::PxActor* actor = _shape->getActor();
//		assert(actor);
//		physx::PxRigidBody* rigidActor = actor->is<physx::PxRigidBody>();
//		assert(rigidActor);
//
//		Transform* transform = getOwner().getComponent<Transform>();
//		physx::PxRigidBodyExt::addForceAtPos(*rigidActor, MathUtil::vector3ToPx(force), MathUtil::vector3ToPx(transform->getWorldPosition()));
//	}
//
//	void Collider3D::setMass(float mass)
//	{
//		_shape->getActor()->is<physx::PxRigidBody>()->setMass(mass);
//	}
//
//	float Collider3D::getMass(void) const
//	{
//		return _shape->getActor()->is<physx::PxRigidBody>()->getMass();
//	}
//
//	void Collider3D::syncPhysics()
//	{
//		physx::PxActor* actor = _shape->getActor();
//		assert(actor);
//		if (actor->is<physx::PxRigidActor>())
//		{
//			physx::PxRigidActor* rigidActor = static_cast<physx::PxRigidActor*>(actor);
//			if (rigidActor->is<physx::PxRigidStatic>())
//				return;
//
//			Matrix		   withoutScale = Matrix::CreateScale(getWorldScale()).Invert() * _worldMatrix;
//			physx::PxMat44 matrix{};
//			std::memcpy(&matrix, &withoutScale, sizeof(Matrix));
//			physx::PxTransform transform{ matrix };
//
//			rigidActor->setGlobalPose(transform);
//		}
//	}
//} // namespace pa
