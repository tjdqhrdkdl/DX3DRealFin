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
		if (mTarget != nullptr)
		{
			MouseMove();
			TrackTarget();
		}
	}
	void CameraScript::Render()
	{

	}
	void CameraScript::TrackTarget()
	{
		
			Vector3 targetPos = mTarget->GetComponent<Transform>()->GetPosition();
			Transform* tr = GetOwner()->GetComponent<Transform>();

			tr->SetPosition(targetPos + mChildPos);
		
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
			Vector2 mousePos = Input::GetMousePosition();
			POINT center = { application.GetWidth() / 2, application.GetHeight() / 2 };
			ScreenToClient(application.GetHwnd(), &center);

			Vector2 mouseMovement = { mousePos.x - center.x, center.y - mousePos.y };
			Transform* tr = GetOwner()->GetComponent<Transform>();

			//�����ÿ� ��������� �κ� ����.
			if (Time::DeltaTime() < 0.1f)
			{
				//�ι� ������� ���̴�.
				//ī�޶� ����(�÷��̾�) �������� ���� ��ġ�� �̵���Ű��
				//ī�޶� ������Ʈ�� ȸ���� �ٲ��ش�.
				
				mChildPos += 60 * tr->Right() * mouseMovement.x * Time::DeltaTime();;
				mChildPos.Normalize();
				mChildPos *= mDistFromTarget;

				
				mChildPos += 60 * tr->Up() * mouseMovement.y * Time::DeltaTime();
				mChildPos.Normalize();
				mChildPos *= mDistFromTarget;
				if (mChildPos.y < -mDistFromTarget + mDistFromTarget/5)
					mChildPos.y = -mDistFromTarget + mDistFromTarget / 5;
				if (mChildPos.y > mDistFromTarget - mDistFromTarget / 5)
					mChildPos.y = mDistFromTarget - mDistFromTarget / 5;
				////��ġ �̵� ���� ī�޶� ȸ�� ���Ѽ� �׻� Ÿ���� �ٶ󺸰Բ�.
				////��ǥ : forward�� newForward�� �Ǵ� ��.
				////�� ������ ������ ���ؼ� ������ �Ǵ� ���͸� ���Ѵ�.
				////�� ������ ������ ���ؼ� �� ���� ������ �ڻ��� ��Ÿ�� ���Ѵ�.
				////������ �Ǵ� ���͸� ������, ��Ÿ��ŭ ȸ����Ų��.
				//
				//Vector3 forward = Vector3::Forward;

				//Vector3 pos = tr->GetPosition();
				//Vector3 targetPos = mTarget->GetComponent<Transform>()->GetPosition();

				//Vector3 newForward = targetPos - pos;


				////newForward.Normalize();
				////Vector3 axis = forward.Cross(newForward);
				////axis.Normalize();

				////float c = forward.Dot(newForward);
				////float vecTheta = acosf(c);
				////float s = sinf(vecTheta);

				//////�� ���Ϳ� ����� rotation matrix.
				////Matrix rotationFromAxis = {
				////	c + (1 - c) * axis.x * axis.x			, (1 - c) * axis.x * axis.y + s * axis.z , (1 - c) * axis.x * axis.z - s * axis.y	, 0,
				////	(1 - c) * axis.x * axis.y - s * axis.z  , c + (1 - c) * axis.y * axis.y			 , (1 - c) * axis.y * axis.z + s * axis.x	, 0,
				////	(1 - c) * axis.x * axis.z + s * axis.y  , (1 - c) * axis.y * axis.z - s * axis.x ,  c + (1 - c) * axis.z * axis.z			, 0,
				////	0										, 0									     , 0										, 1
				////};

				////tr->RotateFromAxis(rotationFromAxis);
				//
				//
				//tr->SetQuaternion(Quaternion::FromToRotation(forward, newForward));
				
				
				
			}

			
			SetCursorPos(application.GetWidth() / 2, application.GetHeight() / 2);
		}
	}
}