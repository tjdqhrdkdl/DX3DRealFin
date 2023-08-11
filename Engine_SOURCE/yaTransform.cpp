#include "yaTransform.h"
#include "yaRenderer.h"
#include "yaCamera.h"

namespace ya
{
	Transform::Transform()
		: Component(eComponentType::Transform)
		, mForward(Vector3::Forward)
		, mRight(Vector3::Right)
		, mUp(Vector3::Up)
		, mScale(Vector3::One)
		, mRotation(Vector3::Zero)
		, mPosition(Vector3::One)
		, mParent(nullptr)
		, mSubPosition(Vector3::Zero)
		, mSubRotation(Vector3::Zero)
		, mSubScale(Vector3::One)
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
		Vector3 finalScale = mScale * mSubScale;
		Matrix scale = Matrix::CreateScale(finalScale);
		mMatScale = scale;



		// 회전 변환 행렬
		Matrix rotation;

		Vector3 finalRotation = mRotation + mSubRotation;
		Vector3 radian(finalRotation.x * (XM_PI / 180)
			, finalRotation.y * (XM_PI / 180)
			, finalRotation.z * (XM_PI / 180));

		float theta;

		rotation = Matrix::CreateRotationX(radian.x);
		rotation *= Matrix::CreateRotationY(radian.y);
		rotation *= Matrix::CreateRotationZ(radian.z);

		mMatRotation = rotation;




		// 이동 변환 행렬
		Matrix position;
		Vector3 finalPosition = mPosition + mSubPosition;
		position.Translation(finalPosition);

		mMatTranslation = position;

		mWorld = scale * rotation * position;
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
		}
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
