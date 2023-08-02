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

					//�� �̵�
					mChildPos += 60 * tr->Right() * mouseMovement.x * Time::DeltaTime();;
					mChildPos.Normalize();
					mChildPos *= mDistFromTarget;


					mChildPos += 60 * tr->Up() * mouseMovement.y * Time::DeltaTime();
					mChildPos.Normalize();
					mChildPos *= mDistFromTarget;

					//y�� �̵� �Ѱ� ����
					if (mChildPos.y < -mDistFromTarget + mDistFromTarget / 10)
						mChildPos.y = -mDistFromTarget + mDistFromTarget / 10;
					if (mChildPos.y > mDistFromTarget - mDistFromTarget / 10)
						mChildPos.y = mDistFromTarget - mDistFromTarget / 10;


					//ȸ��
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