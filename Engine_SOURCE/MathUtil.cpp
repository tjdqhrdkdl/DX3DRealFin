#include "MathUtil.h"
#include "yaEnums.h"

namespace ya
{
	using namespace enums;
	Quaternion MathUtil::makeRotationByASF(float x, float y, float z, RotationOrder order)
	{
		const Quaternion xAxis = Quaternion::CreateFromAxisAngle({ 1.f, 0.f, 0.f }, x);
		const Quaternion yAxis = Quaternion::CreateFromAxisAngle({ 0.f, 1.f, 0.f }, y);
		const Quaternion zAxis = Quaternion::CreateFromAxisAngle({ 0.f, 0.f, 1.f }, z);

		switch (order)
		{
		case RotationOrder::XYZ:
			return xAxis * yAxis * zAxis;
		case RotationOrder::XZY:
			return xAxis * zAxis * yAxis;
		case RotationOrder::YXZ:
			return yAxis * xAxis * zAxis;
		case RotationOrder::YZX:
			return yAxis * zAxis * xAxis;
		case RotationOrder::ZXY:
			return zAxis * xAxis * yAxis;
		case RotationOrder::ZYX:
			return zAxis * yAxis * xAxis;
		default:
			assert(false && "WRONG INPUT");
		}
		return yAxis * xAxis * zAxis;
	}
	Quaternion MathUtil::pxToQuaternion(const physx::PxQuat& data)
	{
		return Quaternion{ data.x, data.y, data.z, data.w };
	}
	physx::PxQuat MathUtil::quaternionToPx(const Quaternion& quaternion)
	{
		return physx::PxQuat{ quaternion.x, quaternion.y, quaternion.z, quaternion.w };
	}
	Vector3 MathUtil::pxToVector3(const physx::PxVec3& data)
	{
		return Vector3{ data.x, data.y, data.z };
	}
	physx::PxVec3 MathUtil::vector3ToPx(const Vector3& vector)
	{
		return physx::PxVec3{ vector.x, vector.y, vector.z };
	}
	Matrix MathUtil::pxToMatrix(const physx::PxMat44& data)
	{
		Matrix result{};
		std::memcpy(&result, &data, sizeof(Matrix));

		return result;
	}
	physx::PxMat44 MathUtil::matrixToPx(const Matrix& matrix)
	{
		physx::PxMat44 result{};
		std::memcpy(&result, &matrix, sizeof(physx::PxMat44));

		return result;
	}
	Quaternion MathUtil::getQuaternionInverse(const Quaternion& quaternion)
	{
		Quaternion result{};
		quaternion.Inverse(result);
		return result;
	}
} //namespace pa
