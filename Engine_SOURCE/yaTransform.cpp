#include "yaTransform.h"
#include "yaRenderer.h"
#include "yaCamera.h"

namespace ya
{



	Transform::Transform()
		: Component(eComponentType::Transform)
		, mParent()
		, mLocalScale(Vector3::One)
		, mLocalRotation(Vector3::Zero)
		, mLocalPosition(Vector3::Zero)
		, mLocalMatrix(Matrix::Identity)
		, mWorldScale(Vector3::One)

		, mLocalRotationOffset()


		, mWorldRotation()
		, mWorldRotationQuaternion()
		, mWorldMatrix(Matrix::Identity)
		, mWorldForward(Vector3::UnitX)
		, mWorldRight(Vector3::UnitY)
		, mWorldUp(Vector3::UnitZ)
		, mbCameraMode()
	{
	}

	Transform::~Transform()
	{
	}


	void Transform::Start()
	{
		FixedUpdate();
	}

	void Transform::FixedUpdate()
	{
		// 크기 변환 행렬
		mLocalMatrix = Matrix::CreateScale(mLocalScale);

		// 회전 변환 행렬
		Vector3 radian = (mLocalRotationOffset + mLocalRotation) * gDegreeToRadFactor;
		mLocalRotationQuaternion = Quaternion::CreateFromPitchYawRoll(radian);
		mLocalMatrix *= Matrix::CreateFromQuaternion(mLocalRotationQuaternion);

		// 이동 변환 행렬
		mLocalMatrix *= Matrix::CreateTranslation(mLocalPosition);


		if (mParent)
		{
			mWorldScale = mParent->GetWorldScale() * mLocalScale;
			mWorldRotation = mParent->GetWorldRotation() + mLocalRotation;
			mWorldRotationQuaternion = Quaternion::CreateFromPitchYawRoll(mWorldRotation);

			mWorldMatrix = mLocalMatrix * mParent->GetWorldMatrix();

			//Matrix matScale = Matrix::CreateScale(mWorldScale);
			//Matrix matRT = matScale.Invert() * mWorld;
			//matRT._41 = 0;
			//matRT._42 = 0;
			//matRT._43 = 0;

			//mForward = Vector3::TransformNormal(Vector3::Forward, matRT);
			//mRight = Vector3::TransformNormal(Vector3::Right, matRT);
			//mUp = Vector3::TransformNormal(Vector3::Up, matRT);
		}
		
		else
		{
			mWorldScale = mLocalScale;
			mWorldRotation = mLocalRotation;
			mWorldRotationQuaternion = mLocalRotationQuaternion;
			mWorldMatrix = mLocalMatrix;
		}

		if (false == mbCameraMode)
		{
			Matrix WorldRot = Matrix::CreateFromQuaternion(mWorldRotationQuaternion);
			mWorldForward = WorldRot.Forward();
			mWorldRight = WorldRot.Right();
			mWorldUp = WorldRot.Up();
		}
	}

	void Transform::PrevRender()
	{



	}

	void Transform::Render()
	{
	}

	void Transform::SetConstantBuffer()
	{

		renderer::TransformCB trCb = {};
		trCb.world = mWorldMatrix;
		trCb.inverseWorld = mWorldMatrix.Invert();
		trCb.view = Camera::GetGpuViewMatrix();
		trCb.inverseView = trCb.view.Invert();
		trCb.projection = Camera::GetGpuProjectionMatrix();



		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Transform];
		cb->SetData(&trCb);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::HS);
		cb->Bind(eShaderStage::DS);
		cb->Bind(eShaderStage::GS);
		cb->Bind(eShaderStage::PS);
		cb->Bind(eShaderStage::CS);
	}



	void Transform::SetWorldPosition(const Vector3& _pos)
	{
		if (mParent)
		{
			Vector3 local = _pos - mParent->GetWorldPosition();
			SetLocalPosition(local);
		}
		else
		{
			SetLocalPosition(_pos);
		}
	}

	void Transform::SetWorldRotation(const Quaternion& _rot)
	{
		if (mParent)
		{
			Quaternion		 inverse{};
			const Quaternion parentWorldRotation = mParent->GetWorldRotationQuaternion();
			parentWorldRotation.Inverse(inverse);

			SetLocalRotationQuaternion(inverse * _rot);
		}
		else
		{
			SetLocalRotationQuaternion(_rot);
		}
	}

}
