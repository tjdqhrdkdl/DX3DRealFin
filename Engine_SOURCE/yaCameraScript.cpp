#include "yaCameraScript.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaInput.h"
#include "yaTime.h"


extern ya::Application application;

namespace ya
{
	CameraScript::CameraScript()
		: Script()
		, mChildPos(Vector3(0,0,-40))
		, mThetaAxisY(1.57)
		, mThetaAxisX(1.57)
		, mDistFromTarget(40)
	{
	}

	CameraScript::~CameraScript()
	{
	}

	void CameraScript::Initalize()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
	}

	void CameraScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

		
		Vector3 pos = tr->GetPosition();


		tr->SetPosition(pos);
		MouseMove();
		TrackTarget();
	}
	void CameraScript::Render()
	{

	}
	void CameraScript::TrackTarget()
	{
		if (mTarget != nullptr)
		{
			Vector3 targetPos = mTarget->GetComponent<Transform>()->GetPosition();
			Transform* tr = GetOwner()->GetComponent<Transform>();

			tr->SetPosition(targetPos + mChildPos);
		}
	}
	void CameraScript::MouseMove()
	{
		//������ ���콺 ��ġ�� ī�޶� ������� �ʵ���.
		if (!mbFirstInit)
		{
			SetCursorPos(application.GetWidth() / 2, application.GetHeight() / 2);
			mbFirstInit = true;
		}


		// ���콺�� �̵� �Ÿ�(�ȼ�) ���� - �ݿ� - ���콺 ��ġ ���� 
		else
		{
			XMQuaternionRotationMatrix
			Vector2 mousePos = Input::GetMousePosition();
			POINT center = { application.GetWidth() / 2, application.GetHeight() / 2 };
			ScreenToClient(application.GetHwnd(), &center);

			Vector2 mouseMovement = { mousePos.x - center.x, center.y - mousePos.y };
			Transform* tr = GetOwner()->GetComponent<Transform>();

			//�����ÿ� ��������� �κ� ����.
			if (Time::DeltaTime() < 0.1f)
			{
				//�ι� ������� ���̴�.
				//�÷��̾ ���� �������� ��ġ�� �̵���Ű��
				//ī�޶� ������Ʈ�� ȸ���� �ٲ��ش�.
				
				mChildPos += 50 * tr->Right() * mouseMovement.x * Time::DeltaTime();;
				mChildPos.Normalize();
				mChildPos *= mDistFromTarget;

				mChildPos += 50 * tr->Up() * mouseMovement.y * Time::DeltaTime();
				mChildPos.Normalize();
				mChildPos *= mDistFromTarget;
				/*mChildPos += 20 * tr->Up() * mouseMovement.y * Time::DeltaTime();*/
				//����������, ī�޶� ȸ�� ���Ѽ� �׻� Ÿ���� �ٶ󺸰Բ�.
				//��ǥ : forward�� newForward�� �Ǵ� ��.
				//�� ������ ������ ���ؼ� ������ �Ǵ� ���͸� ���Ѵ�.
				//�� ������ ������ ���ؼ� �� ���� ������ �ڻ��� ��Ÿ�� ���Ѵ�.
				//������ �Ǵ� ���͸� ������, ��Ÿ��ŭ ȸ����Ų��.
				
					Vector3 forward = Vector3::Forward;

					Vector3 pos = tr->GetPosition();
					Vector3 targetPos = mTarget->GetComponent<Transform>()->GetPosition();

					Vector3 newForward = targetPos - pos;
					float dist = Vector3::Distance(pos, targetPos);
					//if (pos == targetPos)
					//	newForward = forward;

					newForward.Normalize();
					Vector3 axis = forward.Cross(newForward);
					axis.Normalize();

					float c = forward.Dot(newForward);
					float vecTheta = acosf(c);
					float s = sinf(vecTheta);

					//�� ���Ϳ� ����� rotation matrix.
					Matrix rotationFromAxis = {
						c + (1 - c) * axis.x * axis.x			, (1 - c) * axis.x * axis.y + s * axis.z , (1 - c) * axis.x * axis.z - s * axis.y	, 0,
						(1 - c) * axis.x * axis.y - s * axis.z  , c + (1 - c) * axis.y * axis.y			 , (1 - c) * axis.y * axis.z + s * axis.x	, 0,
						(1 - c) * axis.x * axis.z + s * axis.y  , (1 - c) * axis.y * axis.z - s * axis.x ,  c + (1 - c) * axis.z * axis.z			, 0,
						0										, 0									     , 0										, 1
					};

					tr->RotateFromAxis(rotationFromAxis);



				
			}

			
			SetCursorPos(application.GetWidth() / 2, application.GetHeight() / 2);
		}
	}
}