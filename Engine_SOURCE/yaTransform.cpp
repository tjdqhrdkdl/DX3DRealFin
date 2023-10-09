#include "yaTransform.h"
#include "yaRenderer.h"
#include "yaCamera.h"

namespace ya
{
	constexpr float gDegreeToRadFactor = XM_PI / 180.f;
	constexpr float gRadToDegreeFactor = 180.f / XM_PI;


	Transform::Transform()
		: Component(eComponentType::Transform)
		, mForward(Vector3::Forward)
		, mRight(Vector3::Right)
		, mUp(Vector3::Up)
		, mScale(Vector3::One)
		, mRotation(Vector3::Zero)
		, mPosition(Vector3::Zero)
		, mRotationOffset(Vector3::Zero)
		, mParent(nullptr)
	{
	}

	Transform::~Transform()
	{
	}

	void Transform::Initialize()
	{
		//cos(180);
	}

	void Transform::Update()
	{
		//실제 로직상 캐릭터이동 처리
	}

	void Transform::FixedUpdate()
	{
		//렌더링에 사용될 위치값들을 업데이트

		// 월드 행렬 생성


		// 크기 변환 행렬
		Vector3 finalScale = mScale;
		Matrix scale = Matrix::CreateScale(finalScale);
		mMatScale = scale;



		// 회전 변환 행렬
		Matrix rotation = Matrix::Identity;

		Vector3 radian = mRotation * gDegreeToRadFactor;
		rotation = Matrix::CreateRotationX(radian.x);
		rotation *= Matrix::CreateRotationY(radian.y);
		rotation *= Matrix::CreateRotationZ(radian.z);

		mMatRotation = rotation;


		// 이동 변환 행렬
		Matrix position = Matrix::Identity;
		position.Translation(mPosition);
		mMatTranslation = position;

		Matrix rotationOffset;
		rotationOffset.Translation(mRotationOffset);
		mMatRotationOffset = rotationOffset;

		mWorld = scale * rotationOffset * rotation * position;


		if (mbCamera)
		{
		}
		else
		{
			mForward = Vector3::TransformNormal(Vector3::Forward, rotation);
			mRight = Vector3::TransformNormal(Vector3::Right, rotation);
			mUp = Vector3::TransformNormal(Vector3::Up, rotation);
		}
		// 카메라 컴포넌트에서 세팅해준다
		// 뷰행렬 세팅
		// 프로젝션 행렬 세팅


		if (mParent)
		{
			mWorld *= mParent->mWorld;
			mFinalScale = mScale * mParent->mFinalScale;

			Matrix matScale = Matrix::CreateScale(mFinalScale);
			Matrix matRT = matScale.Invert() * mWorld;
			matRT._41 = 0;
			matRT._42 = 0;
			matRT._43 = 0;

			mForward = Vector3::TransformNormal(Vector3::Forward, matRT);
			mRight = Vector3::TransformNormal(Vector3::Right, matRT);
			mUp = Vector3::TransformNormal(Vector3::Up, matRT);
		}
		
		else
		{
			mFinalScale = mScale;
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
		trCb.world = mWorld;
		trCb.inverseWorld = mWorld.Invert();
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

}
