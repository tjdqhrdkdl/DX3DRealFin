#include "yaTransform.h"
#include "yaRenderer.h"
#include "yaCamera.h"

namespace ya
{



	Transform::Transform()
		: Component(eComponentType::Transform)
		, mParent()
		, mScaleLocal(Vector3::One)
		, mRotLocal(Vector3::Zero)
		, mRotOffset(Vector3::Zero)
		, mPosLocal(Vector3::Zero)
		//, mMatLocal(Matrix::Identity)

		, mScaleWorld(Vector3::One)
		, mRotWorld()
		, mQuatWorld()
		, mMatWorld()

		, mWorldForward(Vector3::UnitX)
		, mWorldRight(Vector3::UnitY)
		, mWorldUp(Vector3::UnitZ)
		, mbNeedMyUpdate(true)
		, mbCameraMode()
	{
	}

	Transform::~Transform()
	{
	}


	void Transform::Start()
	{
		CollisionUpdate();
	}

	void Transform::CollisionUpdate()
	{
		if (true == mbNeedMyUpdate)
		{
			//자신의 트랜스폼 업데이트를 진행할 경우 - 두개 다 업데이트 해줘야함.
			UpdateMyTransform();
		}

		
		//// 크기 변환 행렬
		//mLocalMatrix = Matrix::CreateScale(mLocalScale);

		//// 회전 변환 행렬
		//Vector3 radian = (mLocalRotationOffset + mLocalRotation) * gDegreeToRadFactor;
		//mLocalRotationQuaternion = Quaternion::CreateFromPitchYawRoll(radian);
		//mLocalMatrix *= Matrix::CreateFromQuaternion(mLocalRotationQuaternion);

		//// 이동 변환 행렬
		//mLocalMatrix *= Matrix::CreateTranslation(mLocalPosition);


		//if (mParent)
		//{
		//	mWorldScale = mParent->GetWorldScale() * mLocalScale;
		//	mWorldRotation = mParent->GetWorldRotation() + mLocalRotation;
		//	mWorldRotationQuaternion = Quaternion::CreateFromPitchYawRoll(mWorldRotation);

		//	mWorldMatrix = mLocalMatrix * mParent->GetWorldMatrix();
		//}

		//else
		//{
		//	mWorldScale = mLocalScale;
		//	mWorldRotation = mLocalRotation;
		//	mWorldRotationQuaternion = mLocalRotationQuaternion;
		//	mWorldMatrix = mLocalMatrix;
		//}


	}

	void Transform::FetchPhysX(const Quaternion& quatWorld, const Vector3& posWorld)
	{
		SetWorldRotation(quatWorld);
		mQuatWorld = quatWorld;

		SetWorldPosition(posWorld);

		mMatWorld = Matrix::CreateScale(mScaleWorld);
		mMatWorld *= Matrix::CreateFromQuaternion(quatWorld);
		mMatWorld *= Matrix::CreateTranslation(posWorld);


		if (false == mbCameraMode)
		{
			Matrix WorldRot = Matrix::CreateFromQuaternion(mQuatWorld);
			mWorldForward = WorldRot.Forward();
			mWorldRight = WorldRot.Right();
			mWorldUp = WorldRot.Up();
		}
	}


	void Transform::FixedUpdate()
	{

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
		trCb.world = mMatWorld;
		trCb.inverseWorld = mMatWorld.Invert();
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



	void Transform::SetWorldPosition(const Vector3& worldPos)
	{
		if (mParent)
		{
			Vector3 local = worldPos - mParent->GetWorldPosition();
			SetLocalPosition(local);
		}
		else
		{
			SetLocalPosition(worldPos);
		}
	}

	void Transform::SetWorldRotation(const Quaternion& _rot)
	{
		if (mParent)
		{
			//내 월드 회전 정보와 차이를 구한뒤 local에 반영
			Quaternion		 inverse{};
			mParent->GetWorldRotationQuaternion().Inverse(inverse);

			SetLocalRotationQuaternion(inverse * _rot);
		}
		else
		{
			SetLocalRotationQuaternion(_rot);
		}
	}

	void Transform::UpdateMyTransform()
	{
		//1. 크기행렬
		mMatWorld = Matrix::CreateScale(mScaleLocal);

		//2. 회전행렬
		Vector3 radian = mRotLocal * gDegreeToRadianFactor;
		mQuatLocal = Quaternion::CreateFromPitchYawRoll(radian);
		mMatWorld *= Matrix::CreateFromQuaternion(mQuatLocal);

		//3. 이동행렬
		mMatWorld *= Matrix::CreateTranslation(mPosLocal);

		//부모 트랜스폼이 갱신되었는지 확인하고, 갱신되었을 경우 자신의 행렬도 갱신
		if (mParent)
		{
			mMatWorld *= mParent->GetWorldMatrix();
			Vector3 posWorld{};
			mMatWorld.Decompose(mScaleWorld, mQuatWorld, posWorld);
			mRotWorld = mQuatWorld.ToEulerXYZOrder();
		}
		else
		{
			mScaleWorld = mScaleLocal;
			mRotWorld = mRotLocal;
		}


		if (false == mbCameraMode)
		{
			Matrix WorldRot = Matrix::CreateFromQuaternion(mQuatWorld);
			mWorldForward = WorldRot.Forward();
			mWorldRight = WorldRot.Right();
			mWorldUp = WorldRot.Up();
		}
	}
}
