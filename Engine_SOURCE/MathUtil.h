#pragma once
#include <PxPhysicsAPI.h>
#include "yaMath.h"

namespace ya
{
	using namespace math;
	using namespace enums;
	class MathUtil final
	{
	public:
		static Quaternion makeRotationByASF(float x, float y, float z, RotationOrder order);

		static Quaternion	 pxToQuaternion(const physx::PxQuat& data);
		static physx::PxQuat quaternionToPx(const Quaternion& quaternion);

		static Vector3		 pxToVector3(const physx::PxVec3& data);
		static physx::PxVec3 vector3ToPx(const Vector3& vector);

		static Matrix		  pxToMatrix(const physx::PxMat44& data);
		static physx::PxMat44 matrixToPx(const Matrix& matrix);

		static Quaternion getQuaternionInverse(const Quaternion& quaternion);
	};
} //namespace ya

