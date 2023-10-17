#include "yaCameraScript.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaInput.h"
#include "yaTime.h"
#include "yaCollisionManager.h"
#include "yaApplication.h"
#include "yaSceneManager.h"

extern ya::Application application;

namespace ya
{
	const float lockOnDistanceMax = 80.0f;

	CameraScript::CameraScript()
		: Script()
		, mChildPos(Vector3(0, 0, -40))
		, mThetaAxisY(1.57)
		, mThetaAxisX(1.57)
		, mDistFromTarget(10)
		, mDelayTime(0.2f)
		, mDelayTimeChecker(0)
		, mbFirstInit(false)
		, mbMouseMove(true)
		, mbSelfCameraMoveMode(false)
	{
	}

	CameraScript::~CameraScript()
	{
	}

	void CameraScript::Initialize()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

	}

	void CameraScript::Update()
	{
		if (Input::GetKey(eKeyCode::N_9))
			mDistFromTarget += Time::DeltaTime() * 10;
		if (Input::GetKey(eKeyCode::N_0))
			mDistFromTarget -= Time::DeltaTime() * 10;


		if (Input::GetKeyDown(eKeyCode::ESC))
			mbMouseMove = !mbMouseMove;

		if (Input::GetKeyDown(eKeyCode::T))
			mbSelfCameraMoveMode = !mbSelfCameraMoveMode;
		
		if (mbSelfCameraMoveMode)
		{
			Transform* tr = GetOwner()->GetComponent<Transform>();

			Vector3 pos = tr->GetPosition();

			if (Input::GetKeyState(eKeyCode::D) == eKeyState::PRESSED)
			{
				pos += 100.0f * tr->Right() * Time::DeltaTime();
			}
			else if (Input::GetKeyState(eKeyCode::A) == eKeyState::PRESSED)
			{
				pos += 100.0f * -tr->Right() * Time::DeltaTime();
			}
			else if (Input::GetKeyState(eKeyCode::W) == eKeyState::PRESSED)
			{
				pos += 100.0f * tr->Forward() * Time::DeltaTime();
			}
			else if (Input::GetKeyState(eKeyCode::S) == eKeyState::PRESSED)
			{
				pos += 100.0f * -tr->Forward() * Time::DeltaTime();
			}
			else if (Input::GetKeyState(eKeyCode::Q) == eKeyState::PRESSED)
			{
				pos += 100.0f * tr->Up() * Time::DeltaTime();
			}
			else if (Input::GetKeyState(eKeyCode::E) == eKeyState::PRESSED)
			{
				pos += 100.0f * -tr->Up() * Time::DeltaTime();
			}

			if (Input::GetKeyState(eKeyCode::MBTN) == eKeyState::PRESSED)
			{
				Vector2 mousePos = Input::GetMousePosition();
				POINT center = { application.GetWidth() / 2, application.GetHeight() / 2 };
				ScreenToClient(application.GetHwnd(), &center);

				Vector2 mouseMovement = { mousePos.x - center.x, center.y - mousePos.y };
				Transform* tr = GetOwner()->GetComponent<Transform>();
				//디버깅시에 문제생기는 부분 막음.
				if (Time::DeltaTime() < 0.1f)
				{
					Vector3 forward = tr->Forward();
					Vector3 right = tr->Right();
					Vector3 up = tr->Up();
					forward += right * mouseMovement.x * Time::DeltaTime();
					forward += up * mouseMovement.y * Time::DeltaTime();

					forward.Normalize();
					Vector3 UpVector = Vector3(0.0, 1.0, 0.0);

					 right = UpVector.Cross(forward);
					right.Normalize();

					 up = forward.Cross(right);
					up.Normalize();

					tr->IsCamera(true);
					tr->SetForward(forward);
					tr->SetUp(up);
					tr->SetRight(right);
				}

				//회전


				SetCursorPos(application.GetWidth() / 2, application.GetHeight() / 2);
			}
			tr->SetPosition(pos);
		}

		else
		{
			if (mPlayerTarget != nullptr)
			{
				TrackTarget();
				MouseMove();
				LockOn();
				MoveToDestination();
				ObstacleDetection();

			}
		}
	}
	void CameraScript::Render()
	{

	}
	void CameraScript::TrackTarget()
	{

		Vector3 targetPos = mPlayerTarget->GetComponent<Transform>()->GetPosition();
		mQueDelayedTargetPos.push(targetPos);
		Transform* tr = GetOwner()->GetComponent<Transform>();


		if (mDelayTimeChecker < mDelayTime && mbFirstInit)
			mDelayTimeChecker += Time::DeltaTime();
		else
		{
			//카메라 이동 로직
			//카메라가 바라보는 타겟이 카메라의 right 방향으로 움직이면 카메라는 움직이지 않아야 한다.
			Vector3 delayedPos = mQueDelayedTargetPos.front();
			mQueDelayedTargetPos.pop();
			//if (delayedPos != mDelayedTargetPos)
			//	int a = 0;

			//Vector3 change = delayedPos - mDelayedTargetPos;
			//
			//
			//Vector3 scala = change.x * tr->Right() + change.y * tr->Up() + change.z * tr->Forward();
			//delayedPos -= scala.x * tr->Right();
			mDelayedTargetPos = delayedPos;

			tr->SetPosition(mDelayedTargetPos + mChildPos);

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

	}
	void CameraScript::MouseMove()
	{
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
					if (Time::DeltaTime() < 0.1f && !mbLockOn)
					{
						//두번 계산해줄 것이다.
						//카메라를 원점(플레이어) 기준으로 먼저 위치를 이동시키고
						//카메라 오브젝트의 회전을 바꿔준다.

						//구 이동
						mChildPos -= 2 * tr->Right() * mouseMovement.x * Time::DeltaTime();;
						mChildPos.Normalize();
						mChildPos *= mDistFromTarget;


						mChildPos -= 1 * tr->Up() * mouseMovement.y * Time::DeltaTime();
						mChildPos.Normalize();
						mChildPos *= mDistFromTarget;

						//y축 이동 한계 지정
						if (mChildPos.y < -mDistFromTarget + mDistFromTarget / 1.2)
							mChildPos.y = -mDistFromTarget + mDistFromTarget / 1.2;
						if (mChildPos.y > mDistFromTarget - mDistFromTarget / 5)
							mChildPos.y = mDistFromTarget - mDistFromTarget / 5;







					}
				
				//회전


				SetCursorPos(application.GetWidth() / 2, application.GetHeight() / 2);
			}
		}
	}

	void CameraScript::ObstacleDetection()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 direction =  mChildPos;
		direction.Normalize();
		std::vector<eLayerType> layers = {};
		layers.push_back(eLayerType::Ground);
		//layers.push_back(eLayerType::Wall);
		RayHit hit = CollisionManager::RayCast(mPlayerTarget, mDelayedTargetPos, direction, layers);
		if (hit.isHit)
		{
			if(hit.length < mDistFromTarget)
				mChildPos = direction * hit.length - direction;
		}
	}

	void CameraScript::LockOn()
	{
		if (Input::GetKeyDown(eKeyCode::MBTN))
			if (mbLockOn && !(mLockOnTarget->IsDead()))
				mLockOnTarget = nullptr, mbLockOn = false;
			else
				SetLockOnTarget();
		if (mLockOnTarget && mLockOnTarget->IsDead())
			mbLockOn = false, mLockOnTarget = nullptr;
		if (mbLockOn)
		{
			Transform* monTr = mLockOnTarget->GetComponent<Transform>();
			if (monTr == nullptr)
			{
				mbLockOn = false;
				return;
			}

			Vector3 monPos = monTr->GetPosition();
			Vector3 dir = mDelayedTargetPos - monPos;
			dir.Normalize();
			dir.y = 0.3;
			dir.Normalize();
			Vector3 dest = dir * mDistFromTarget;
			Vector3 gap = dest - mChildPos;
			Vector3 gapNormal = dest - mChildPos;
			gapNormal.Normalize();
			Vector3 move = 200 * gapNormal * Time::DeltaTime();
			if (gap.Length() < move.Length())
				return;
			mChildPos += 10 * gap.Length() * gapNormal * Time::DeltaTime();
			mChildPos.Normalize();
			mChildPos *= mDistFromTarget;



			Vector3 monPlDiff = monPos - mPlayerTarget->GetComponent<Transform>()->GetPosition();
			float monPlDist = monPlDiff.Length();
			if(monPlDist > lockOnDistanceMax)
				mLockOnTarget = nullptr, mbLockOn = false;
			
		}


	}
	void CameraScript::SetLockOnTarget()
	{
		//락온 타겟을 정해줘야함.
		//mbtn 클릭하면 락온 대상 탐색
		//탐색 성공시 락온 타겟으로 설정, 락온상태 설정 
		//탐색 실패시 락온상태 off , 락온 타겟 null
		//락온 탐색에 고려해야하는 부분 -> 카메라 포워드와 목표물의 각도 / 거리 / 장애물.
		//장애물을 고려할때, 사실 일부분만 보여도 락온이 되던데. 레이를 여러방 쏴야하나??
		//락온 대상이 장애물 등에 완전히 가려지거나 너무 멀어지면 락온이 풀려야한다.

		//Scene* scene =  SceneManager::GetActiveScene();
		Scene* scene = GetOwner()->GetScene();
		std::vector<GameObject*> mons =  scene->GetGameObjects(eLayerType::Monster);
		float minDist = 10000000;
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
		Transform* plTr = mPlayerTarget->GetComponent<Transform>();
		Vector3 plPos = plTr->GetPosition();
		for (GameObject* mon : mons)
		{
			Transform* monTr = mon->GetComponent<Transform>();
			Vector3 monPos = monTr->GetPosition();
			Vector3 monCamDiff = monPos - pos;
			Vector3 monPlDiff = monPos - plPos;
			float dist = monPlDiff.Length();
			
			// 락온이 가능한 최대거리를 벗어나는지 체크
			if (dist > lockOnDistanceMax)
				continue;

			// 카메라 포워드와 각도 체크  (이 각도가 문제임. 각도의 적정수준이 필요할듯)
			Quaternion rot = Quaternion::FromToRotation(tr->Forward(), monCamDiff);
			Vector3 theta = rot.ToEuler();
			if (fabsf(theta.x) > XM_PIDIV2 * 1/ 2)
				continue;
			if (fabsf(theta.y) > XM_PIDIV2 * 1/ 2)
				continue;
			//if (fabsf(theta.z) > XM_PI * 2 / 3)
			//	continue;
			
			// 카메라와 몬스터 사이에 장애물(시야에 가려지는지) 체크   (아마 레이를 몇개 더쏴서 확인해야할듯 ?)
			//Vector3 dir = -monCamDiff;
			//dir.Normalize();
			//std::vector<eLayerType> layers = {};
			//layers.push_back(eLayerType::Ground);
			//RayHit hit = CollisionManager::RayCast(mon, dir, layers);
			//if (hit.isHit)
			//	continue;

			// 위 조건을 만족하면서, 더 적절한 오브젝트가 있는지 체크   (현재는 더 가까운 것이 더 적절하다.)

			Vector3 plMonDistVec = monPos - plPos;
			float plMonDist = plMonDistVec.Length();
			if (plMonDist < minDist)
			{
				minDist = plMonDist;
				mLockOnTarget = mon;
				mbLockOn = true;
			}
		}

		if (!mbLockOn)
		{
			Vector3 dest = -(plTr->Forward()) + Vector3(0, 0.5, 0);
			dest.Normalize();
			dest *= mDistFromTarget;
			SetDestination(dest);
		}

	}
	void CameraScript::MoveToDestination()
	{
		if (mbDestination)
		{
			Vector3 gap = mDestination - mChildPos;
			Vector3 gapNormal = mDestination - mChildPos;
			gapNormal.Normalize();
			Vector3 move = 200 * gapNormal * Time::DeltaTime();
			if (gap.Length() < move.Length())
			{
				mbDestination = false;
				return;
			}
			mChildPos += 10 * gap.Length() * gapNormal * Time::DeltaTime();
			mChildPos.Normalize();
			mChildPos *= mDistFromTarget;
		}
	}
}