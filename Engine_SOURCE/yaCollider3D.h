#pragma once

#include "yaComponent.h"
#include "yaEnums.h"
#include "yaRenderer.h"
#include "EnumFlags.h"

// 전방 선언
namespace physx
{
	class PxShape;
	class PxActor;
}


namespace ya
{
	//Forward Declaration
	namespace graphics
	{
		class ConstantBuffer;
	}
	class Mesh;
	class Shader;

	enum class FreezeRotationFlag : uint8
	{
		ROTATION_X = (1 << 1),
		ROTATION_Y = (1 << 2),
		ROTATION_Z = (1 << 4),

		END
	};

	// 전방 선언


	class Collider3D final : public Component
	{
		friend class PhysxWrapper;

	public:
		explicit Collider3D();
		virtual ~Collider3D();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void		 setType(eColliderType type, bool isStatic = false);
		eColliderType getCollider3DType(void) const { return _type; }

		Matrix	getWorldMatrix(void) const { return _worldMatrix; }
		Vector3 getWorldPosition(void) const { return _worldMatrix.Translation(); }
		Vector3 getWorldScale(void) const { return _offsetScale; }

		void	setOffsetPosition(Vector3 offset) { _offsetPosition = offset; }
		Vector3 getOffsetPosition(void) const { return _offsetPosition; }

		void	setOffsetScale(Vector3 offset);
		Vector3 getOffsetScale(void) const { return _offsetScale; }

		void onCollisionEnter(Collider3D* other, const Vector3& collisionPosition);
		void onCollision(Collider3D* other, const Vector3& collisionPosition);
		void onCollisionExit(Collider3D* other, const Vector3& collisionPosition);

		void onTriggerEnter(Collider3D* other);
		void onTriggerExit(Collider3D* other);

		bool isTrigger(void) const { return _isTrigger; }
		void setTrigger(bool enable);

		physx::PxShape* getShape() const { return _shape; }
		void			enableGravity(bool enable);
		bool			isGravityEnabled(void) const { return _isGravityEnabled; }
		bool			isStatic(void) const { return _isStatic; }

		bool		isOverlapping(const GameObject* other, Vector3* outDistance) const;
		GameObject* getOverlappingObject(void) const { return _otherOverlapping; }

		uint32 getId(void) const { return _id; }

		void setFreezeRotation(FreezeRotationFlag flag, bool enable);
		bool hasFlag(FreezeRotationFlag flag) const;

		void addForce(const Vector3& force);

		void setEnableDebugDraw(bool enable) { _enableDraw = enable; }
		bool isDrawDebug(void) const { return _enableDraw; }

		void  setMass(float mass);
		float getMass(void) const;

	private:
		void syncPhysics();
		void destroyShape() { _shape = nullptr; }

	private:
		static uint32 _idGenerator;

		uint32		 _id;
		eColliderType _type;
		Vector3		 _offsetPosition;
		Vector3		 _offsetScale;
		uint32		 _collisionCount;
		Matrix		 _worldMatrix;

		std::shared_ptr<Mesh>		   _mesh;
		std::shared_ptr<Shader> _shader;

		ConstantBuffer* _positionBuffer;
		renderer::TransformCB						_positionBufferData;

		ConstantBuffer* _wireFrameBuffer;
		renderer::TransformCB						_wireFrameData;

		physx::PxShape* _shape;
		bool			_isTrigger;
		bool			_isGravityEnabled;
		bool			_isStatic;

		GameObject* _otherOverlapping;

		EnumFlags<FreezeRotationFlag, uint16> _freezeRotationFlag;

		bool _enableDraw;
	};
} // namespace pa
