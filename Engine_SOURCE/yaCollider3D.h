#pragma once

#include "yaCollider.h"
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

	enum class FreezeRotationFlag : UINT8
	{
		ROTATION_X = (1 << 1),
		ROTATION_Y = (1 << 2),
		ROTATION_Z = (1 << 4),

		END
	};

	// 전방 선언


	class Collider3D final : public Collider
	{
		friend class PhysxWrapper;

	public:
		explicit Collider3D();
		virtual ~Collider3D();

		virtual void Initialize() override;
		virtual void Start() override;

		virtual void CollisionUpdate() override;
		virtual void CollisionLateUpdate() override;
		

		virtual void Render() override;


		void		 SetType(eColliderType type, bool isStatic = false)
		{
			_type = type;
			_isStatic = isStatic;
		}
		eColliderType GetCollider3DType(void) const { return _type; }

		Matrix	getWorldMatrix(void) const { return _worldMatrix; }
		Vector3 getWorldPosition(void) const { return _worldMatrix.Translation(); }
		Vector3 getWorldScale(void) const { return _offsetScale; }

		void	SetCenter(Vector3 offset) { _offsetPosition = offset; }
		Vector3 GetCenter(void) const { return _offsetPosition; }

		void	SetSize(Vector3 offset);
		Vector3 GetSize(void) const { return _offsetScale; }


		void OnCollisionEnter(Collider3D* other, const Vector3& collisionPosition);
		void OnCollision(Collider3D* other, const Vector3& collisionPosition);
		void OnCollisionExit(Collider3D* other, const Vector3& collisionPosition);

		void OnTriggerEnter(Collider3D* other);
		void OnTriggerExit(Collider3D* other);

		bool isTrigger(void) const { return _isTrigger; }
		void setTrigger(bool enable);

		//씬 변경시 dontdestroy에 의해 제거되지 않고 씬만 변경 되었을 경우
		void SceneChanged();
		physx::PxShape* getShape() const { return _shape; }
		void			EnableGravity(bool enable);
		bool			isGravityEnabled(void) const { return _isGravityEnabled; }
		bool			isStatic(void) const { return _isStatic; }

		bool		isOverlapping(GameObject* other, Vector3* outDistance);
		GameObject* getOverlappingObject(void) const { return _otherOverlapping; }

		UINT32 getId(void) const { return _id; }

		void setFreezeRotation(FreezeRotationFlag flag, bool enable);
		bool hasFlag(FreezeRotationFlag flag) const;

		void AddForce(const Vector3& force);
		void ClearForce();

		void setEnableDebugDraw(bool enable) { _enableDraw = enable; }
		bool isDrawDebug(void) const { return _enableDraw; }

		void  SetMass(float mass);
		float GetMass(void) const { return _mass; };

		void SetRestitution(float restitution);
		float GetRestitution() const { return _restitution; }

		void SetVelocity(const Vector3& _velocity);
		Vector3 GetVelocity() const;

		void SetStaticFriction(float staticFriction);
		float GetStaticFriction() const { return _staticFriction; }

		void SetDynamicFriction(float dynamicFriction);
		float GetDynamicFriction() const { return _dynamicFriction; }

		void SetLimitVelocity(float maxVelocity);
		float GetLimitVelocity() const { return _maxVelocity; }

		void SetKinematic(bool enable);

		//Start 호출 되기 전에만 동작함.
		void SetSyncScaleToTransfom(bool enable) { _syncScaleToTransform = enable; }


	private:
		void SyncPhysX();
		void FetchPhysX();
		void destroyShape() { _shape = nullptr; }
		void UpdateMatrix();

	private:
		static UINT32 _idGenerator;

		UINT32		 _id;
		eColliderType _type;
		Vector3		 _offsetPosition;
		Vector3		 _offsetScale;
		UINT32		 _collisionCount;

		Matrix		 _localMatrix;
		Matrix		 _worldMatrix;

		float		 _mass;
		float		 _restitution;
		float		 _staticFriction;
		float		 _dynamicFriction;
		float		 _maxVelocity;
		

		renderer::WireFrameCB						_wireFrameData;

		physx::PxShape* _shape;
		bool			_isTrigger;
		bool			_isGravityEnabled;
		bool			_isStatic;
		bool			_syncScaleToTransform;
		 
		GameObject* _otherOverlapping;

		EnumFlags<FreezeRotationFlag, UINT16> _freezeRotationFlag;

		bool _enableDraw;
	};
} // namespace pa
