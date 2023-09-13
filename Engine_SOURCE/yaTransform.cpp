#include "yaTransform.h"
#include "yaRenderer.h"
#include "yaCamera.h"

namespace ya
{
	constexpr float gDegreeToRadFactor = XM_PI / 180.f;
	constexpr float gRadToDegreeFactor = 180.f / XM_PI;


	Transform::Transform()
		: Component(eComponentType::Transform)
		, mParent(nullptr)
		, mForward(0.f, 0.f, 1.f)
		, mRight(1.f, 0.f, 0.f)
		, mUp(0.f, 1.f, 0.f)
		, mPosition()
		, mRotation()
		, mScale(Vector3::One)
		, mFinalScale(Vector3::One)
		, mbCamera()
		, mbUpdated(true)
		, mMatRelativeWorld(Matrix::Identity)
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
		if (mbUpdated)
		{
			//크기
			mMatRelativeWorld = Matrix::CreateScale(mScale);

			//회전
			Vector3 radian = mRotation * gDegreeToRadFactor;
			Matrix matRot = Matrix::CreateRotationX(radian.x);
			matRot *=		Matrix::CreateRotationY(radian.y);
			matRot *=		Matrix::CreateRotationZ(radian.z);

			//이동
			mMatRelativeWorld *= Matrix::CreateTranslation(mPosition);
		}

		// 최종 월드 행렬 생성
		if (mParent)
		{
			mFinalScale = mParent->GetFinalScale() * mScale;
			mMatFinalWorld = mParent->GetWorldMatrix() * mMatRelativeWorld;
		}
		else
		{
			mFinalScale = mScale;
			mMatFinalWorld = mMatRelativeWorld;
		}


		//방향 설정
		if (false == mbCamera)
		{
			mForward = mMatFinalWorld.Forward();
			mForward.Normalize();
			mRight = mMatFinalWorld.Right();
			mRight.Normalize();
			mUp =  mMatFinalWorld.Up();
			mUp.Normalize();
		}
		
		mbUpdated = false;
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
		trCb.world = mMatRelativeWorld;
		trCb.inverseWorld = mMatRelativeWorld.Invert();
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
