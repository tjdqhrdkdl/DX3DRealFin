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
		//최초의 마우스 위치에 카메라가 영향받지 않도록.
		if (!mbFirstInit)
		{
			SetCursorPos(application.GetWidth() / 2, application.GetHeight() / 2);
			mbFirstInit = true;
		}


		// 마우스의 이동 거리(픽셀) 측정 - 반영 - 마우스 위치 고정 
		else
		{
			XMQuaternionRotationMatrix
			Vector2 mousePos = Input::GetMousePosition();
			POINT center = { application.GetWidth() / 2, application.GetHeight() / 2 };
			ScreenToClient(application.GetHwnd(), &center);

			Vector2 mouseMovement = { mousePos.x - center.x, center.y - mousePos.y };
			Transform* tr = GetOwner()->GetComponent<Transform>();

			//디버깅시에 문제생기는 부분 막음.
			if (Time::DeltaTime() < 0.1f)
			{
				//두번 계산해줄 것이다.
				//플레이어를 원점 기준으로 위치를 이동시키고
				//카메라 오브젝트의 회전을 바꿔준다.
				
				mChildPos += 50 * tr->Right() * mouseMovement.x * Time::DeltaTime();;
				mChildPos.Normalize();
				mChildPos *= mDistFromTarget;

				mChildPos += 50 * tr->Up() * mouseMovement.y * Time::DeltaTime();
				mChildPos.Normalize();
				mChildPos *= mDistFromTarget;
				/*mChildPos += 20 * tr->Up() * mouseMovement.y * Time::DeltaTime();*/
				//마지막으로, 카메라를 회전 시켜서 항상 타겟을 바라보게끔.
				//목표 : forward가 newForward가 되는 것.
				//두 벡터의 외적을 통해서 수직이 되는 벡터를 구한다.
				//두 벡터의 내적을 통해서 두 벡터 사이의 코사인 세타를 구한다.
				//수직이 되는 벡터를 축으로, 세타만큼 회전시킨다.
				
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

					//축 벡터에 기반한 rotation matrix.
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