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
	{
		
	}

	Transform::~Transform()
	{
	}

	void Transform::Initalize()
	{
		//cos(180);
	}

	void Transform::Update()
	{
		//���� ������ ĳ�����̵� ó��
	}

	void Transform::FixedUpdate()
	{
		//�������� ���� ��ġ������ ������Ʈ

		// ���� ��� ����
		

		// ũ�� ��ȯ ���
		Matrix scale = Matrix::CreateScale(mScale);
		mMatScale = scale;



		// ȸ�� ��ȯ ���
		Matrix rotation;

		if (mbRotateFromAxis)
		{
			rotation = mRotationFromAxis;
		}
		else if (mbUseQuaternion)
		{
			Matrix qM = XMMatrixRotationQuaternion(mRotationQuaternion);
			rotation = qM;
		}
		else
		{
			Vector3 radian(mRotation.x * (XM_PI / 180)
				, mRotation.y * (XM_PI / 180)
				, mRotation.z * (XM_PI / 180));

			float theta;

			rotation = Matrix::CreateRotationX(radian.x);
			rotation *= Matrix::CreateRotationY(radian.y);
			rotation *= Matrix::CreateRotationZ(radian.z);
		}
		mMatRotation = rotation;



		// �̵� ��ȯ ���
		Matrix position;
		position.Translation(mPosition);

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
		// ī�޶� ������Ʈ���� �������ش�
		// ����� ����
		// �������� ��� ����

	
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
		trCb.view = Camera::GetGpuViewMatrix();
		trCb.projection = Camera::GetGpuProjectionMatrix();

		if (mbUseQuaternion)
			int a = 0;


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
