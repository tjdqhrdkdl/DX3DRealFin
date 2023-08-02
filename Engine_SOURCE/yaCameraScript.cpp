#include "yaCameraScript.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaInput.h"
#include "yaTime.h"
#include "yaCollisionManager.h"

extern ya::Application application;

namespace ya
{
	CameraScript::CameraScript()
		: Script()
		, mChildPos(Vector3(0,0,-40))
		, mThetaAxisY(1.57)
		, mThetaAxisX(1.57)
		, mDistFromTarget(40)
		, mDelayTime(0.1f)
		, mDelayTimeChecker(0)
		, mbFirstInit(false)
		, mbMouseMove(true)
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
			TrackTarget();
			MouseMove();
			ObstacleDetection();
		}
	}
	void CameraScript::Render()
	{

	}
	void CameraScript::TrackTarget()
	{
		
			Vector3 targetPos = mTarget->GetComponent<Transform>()->GetPosition();
			mQueDelayedTargetPos.push(targetPos);
			Transform* tr = GetOwner()->GetComponent<Transform>();


			if (mDelayTimeChecker < mDelayTime && mbFirstInit)
				mDelayTimeChecker += Time::DeltaTime();
			else
			{
				Vector3 delayedPos = mQueDelayedTargetPos.front();
				mQueDelayedTargetPos.pop();
				mDelayedTargetPos = delayedPos;
				tr->SetPosition(delayedPos + mChildPos);
			}
		
	}
	void CameraScript::MouseMove()
	{
		if (Input::GetKeyDown(eKeyCode::ESC))
		{
			mbMouseMove = !mbMouseMove;
		}
		if (mbMouseMove)
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
				Vector2 mousePos = Input::GetMousePosition();
				POINT center = { application.GetWidth() / 2, application.GetHeight() / 2 };
				ScreenToClient(application.GetHwnd(), &center);

				Vector2 mouseMovement = { mousePos.x - center.x, center.y - mousePos.y };
				Transform* tr = GetOwner()->GetComponent<Transform>();

				//디버깅시에 문제생기는 부분 막음.
				if (Time::DeltaTime() < 0.1f)
				{
					//두번 계산해줄 것이다.
					//카메라를 원점(플레이어) 기준으로 먼저 위치를 이동시키고
					//카메라 오브젝트의 회전을 바꿔준다.

					//구 이동
					mChildPos += 60 * tr->Right() * mouseMovement.x * Time::DeltaTime();;
					mChildPos.Normalize();
					mChildPos *= mDistFromTarget;


					mChildPos += 60 * tr->Up() * mouseMovement.y * Time::DeltaTime();
					mChildPos.Normalize();
					mChildPos *= mDistFromTarget;

					//y축 이동 한계 지정
					if (mChildPos.y < -mDistFromTarget + mDistFromTarget / 10)
						mChildPos.y = -mDistFromTarget + mDistFromTarget / 10;
					if (mChildPos.y > mDistFromTarget - mDistFromTarget / 10)
						mChildPos.y = mDistFromTarget - mDistFromTarget / 10;


					//회전
					Vector3 pos = tr->GetPosition();
					Vector3 UpVector = Vector3(0.0, 1.0, 0.0);

					Vector3 targetPos = mDelayedTargetPos;
					Vector3 newForward = targetPos - pos;
					newForward.Normalize();

					Vector3 forward = newForward;

					Vector3 right = UpVector.Cross(forward);
					right.Normalize();

					Vector3 up = forward.Cross(right);
					up.Normalize();

					tr->IsCamera(true);
					tr->SetForward(forward);
					tr->SetUp(up);
					tr->SetRight(right);




				}


				SetCursorPos(application.GetWidth() / 2, application.GetHeight() / 2);
			}
		}
	}
	void CameraScript::ObstacleDetection()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 direction = tr->GetPosition() - mDelayedTargetPos;
		direction.Normalize();
		std::vector<eLayerType> layers = {};
		layers.push_back(eLayerType::Ground);
		RayHit hit = CollisionManager::RayCast(mTarget, direction, layers);
		if (hit.isHit)
		{
			int a = 0;
			tr->SetPosition(hit.contact);
		}
	}
}