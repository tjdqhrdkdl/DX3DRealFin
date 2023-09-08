#include "yaPlayerActionScript.h"

#include "yaInput.h"
#include "yaTime.h"

#include "yaPlayer.h"

#include "yaTransform.h"
#include "yaRigidbody.h"
#include "yaCameraScript.h"
#include "yaPlayerMeshScript.h"

namespace ya
{
	PlayerActionScript::PlayerActionScript()
		: ActionScript()
		, mbRotate(false)
		, mLastDir(eDirection::Forward)
		, mbDash(false)
		, mDashSpeed(300.0f)
		, mDashTimer(0.0f)
		, mFrontTheta(4.0f)
		, mDashTimerMax(0.2f)
	{
	}

	PlayerActionScript::~PlayerActionScript()
	{
	}

	void PlayerActionScript::Initialize()
	{
		ActionScript::Initialize();
	}

	void PlayerActionScript::Update()
	{
		Player* player = dynamic_cast<Player*>(GetOwner());

		PlayerMeshScript* playerAnim = player->GetScript<PlayerMeshScript>();
    
    CheckGround();
		Walk();
		Run();

		// 매달리기 임시
		if (Input::GetKeyDown(eKeyCode::H))
		{
			// TODO: 매달리기 -> 떨어지기, 올라가기 만들어야함
			if (player->IsStateFlag(ePlayerState::Hang))
			{
				player->SetStateFlag(ePlayerState::Hang, false);
				playerAnim->Play(L"a000_000000");
			}
			else
			{
				player->SetStateFlag(ePlayerState::Hang, true);
				playerAnim->Play(L"a000_020000");
			}
		}

		if (Input::GetKeyDown(eKeyCode::SPACE))
		{
			playerAnim->Play(L"a000_200000");
		}

		if (Input::GetKey(eKeyCode::SPACE))
		{
			player->SetStateFlag(ePlayerState::Jump, true);
			Jump();
		}

		if (Input::GetKeyDown(eKeyCode::Q))
		{
			if (player->IsStateFlag(ePlayerState::Crouch))
			{
				player->SetStateFlag(ePlayerState::Crouch, false);
				playerAnim->Play(L"a000_000000");
			}
			else
			{
				player->SetStateFlag(ePlayerState::Crouch, true);
				playerAnim->Play(L"a000_005000");
			}
		}

		ActionScript::Update();
	}

	void PlayerActionScript::FixedUpdate()
	{
		ActionScript::FixedUpdate();
	}


	void PlayerActionScript::Render()
	{
		ActionScript::Render();
	}

	void PlayerActionScript::Walk()
	{
		if (mDashTimer > 0.0f)
			return;

		Player* player = dynamic_cast<Player*>(GetOwner());
		PlayerMeshScript* playerAnim = player->GetScript<PlayerMeshScript>();

		Vector3 pos = mTransform->GetPosition();
		Vector3 rot = mTransform->GetRotation();

		GameObject* camera = player->GetCamera();
		CameraScript* cameraScript = camera->GetScript<CameraScript>();
		Transform* cameraTr = camera->GetComponent<Transform>();
		Vector3 cameraPos = cameraTr->GetPosition();
		bool bLockOn = cameraScript->IsLockOn();

		Vector3 theta = Vector3::Zero;
		if (bLockOn)
		{	// lockon 상태인 경우 항상 lockon 타겟을 바라본다.
			GameObject* lockOnTarget = cameraScript->GetLockOnTarget();
			Vector3 lockOnTargetPos = lockOnTarget->GetComponent<Transform>()->GetPosition();

			Quaternion quater = Quaternion::FromToRotation(mTransform->Forward(), Vector3(lockOnTargetPos.x - pos.x, pos.y, lockOnTargetPos.z - pos.z));
			Vector3 quaterToEuler = quater.ToEuler();
			theta = quaterToEuler * 180.0f / XM_PI;

			mbRotate = true;
		}
		else
		{	// lockon 상태가 아닌 경우 카메라를 등진다.
			Vector3 dir = -mTransform->Forward();
			if (mLastDir == eDirection::Forward)
				dir = -mTransform->Forward();
			if (mLastDir == eDirection::Back)
				dir = mTransform->Forward();
			if (mLastDir == eDirection::Right)
				dir = mTransform->Right();
			if (mLastDir == eDirection::Left)
				dir = -mTransform->Right();

			// player가 바라볼 방향과 camera 사이의 각도를 구한다.
			Quaternion quater = Quaternion::FromToRotation(dir, Vector3(cameraPos.x - pos.x, pos.y, cameraPos.z - pos.z));
			Vector3 quaterToEuler = quater.ToEuler();
			theta = quaterToEuler * 180.0f / XM_PI;
		}

		if (mbRotate)
		{
			if (abs(theta.y) < mFrontTheta)
			{	// 회전 종료. 진행하려는 방향과 player의 forward가 비슷해지면 회전이 끝난다.
				mbRotate = false;
				mTransform->SetRotation(Vector3(0.0f, rot.y + theta.y, 0.0f));
			}
			else
			{	// 진행하려는 방향과 player의 forward가 비슷해질 때 까지 회전한다. theta 각에 따라 회전 방향을 결정한다.
				if (theta.y > 0.0f)
					Rotate(Vector3(0.0f, 1.0f, 0.0f));
				else
					Rotate(Vector3(0.0f, -1.0f, 0.0f));
			}
		}

		if (Input::GetKeyDown(eKeyCode::W))
		{
			player->SetStateFlag(ePlayerState::Walk, true);

			if (player->IsStateFlag(ePlayerState::Hang))
			{
			}
			else if (player->IsStateFlag(ePlayerState::Crouch))
			{
			}
			else if (player->IsStateFlag(ePlayerState::Block))
			{
			}
			else
			{
				playerAnim->Play(L"a000_000500");
			}
		}
		if (Input::GetKeyDown(eKeyCode::A))
		{
			player->SetStateFlag(ePlayerState::Walk, true);

			if (player->IsStateFlag(ePlayerState::Hang))
			{

			}
			else if (player->IsStateFlag(ePlayerState::Crouch))
			{
				if (bLockOn)
					playerAnim->Play(L"a000_005102");
				else
					playerAnim->Play(L"a000_005100");
			}
			else if (player->IsStateFlag(ePlayerState::Block))
			{

			}
			else
			{
				if (bLockOn)
				{
					playerAnim->Play(L"a000_000503");
				}
				else
				{
					if (abs(theta.y) < mFrontTheta)
						playerAnim->Play(L"a000_000500");
					else
						playerAnim->Play(L"a000_000010");
				}
				
			}
		}
		if (Input::GetKeyDown(eKeyCode::S))
		{
			player->SetStateFlag(ePlayerState::Walk, true);

			if (player->IsStateFlag(ePlayerState::Hang))
			{

			}
			else if (player->IsStateFlag(ePlayerState::Crouch))
			{
				if (bLockOn)
					playerAnim->Play(L"a000_005101");
				else
					playerAnim->Play(L"a000_005100");
			}
			else if (player->IsStateFlag(ePlayerState::Block))
			{

			}
			else
			{
				if (bLockOn)
				{
					playerAnim->Play(L"a000_000501");
				}
				else
				{
					if (abs(theta.y) < mFrontTheta)
						playerAnim->Play(L"a000_000500");
					else
						playerAnim->Play(L"a000_000011");
				}
			}
		}
		if (Input::GetKeyDown(eKeyCode::D))
		{
			player->SetStateFlag(ePlayerState::Walk, true);

			if (player->IsStateFlag(ePlayerState::Hang))
			{

			}
			else if (player->IsStateFlag(ePlayerState::Crouch))
			{
				if (bLockOn)
					playerAnim->Play(L"a000_005103");
				else
					playerAnim->Play(L"a000_005100");
			}
			else if (player->IsStateFlag(ePlayerState::Block))
			{

			}
			else
			{
				if (bLockOn)
				{
					playerAnim->Play(L"a000_000502");
				}
				else
				{
					if (abs(theta.y) < mFrontTheta)
						playerAnim->Play(L"a000_000500");
					else
						playerAnim->Play(L"a000_000012");
				}
			}
		}

		if (Input::GetKey(eKeyCode::W))
		{
			if (bLockOn)
			{
				Move(mTransform->Forward());
			}
			else
			{
				if (abs(theta.y) > mFrontTheta)
				{	// 진행하려는 방향과 각도 차이가 날때 회전시킨다.
					mbRotate = true;
				}
				else
				{	// 진행하려는 방향과 각도 차이가 없으면 player의 forward 방향으로 이동시킨다.
					if (mbDash)
						Move(mTransform->Forward(), mDashSpeed);
					else
						Move(mTransform->Forward());
				}

				mLastDir = eDirection::Forward;
			}
		}

		if (Input::GetKey(eKeyCode::S))
		{
			if (bLockOn)
			{
				Move(-mTransform->Forward());
			}
			else
			{
				if (abs(theta.y) > mFrontTheta)
				{
					mbRotate = true;
				}
				else
				{
					if (mbDash)
						Move(mTransform->Forward(), mDashSpeed);
					else
						Move(mTransform->Forward());
				}

				mLastDir = eDirection::Back;
			}
		}

		if (Input::GetKey(eKeyCode::D))
		{
			if (bLockOn)
			{
				Move(mTransform->Right());
			}
			else
			{
				mLastDir = eDirection::Right;

				if (abs(theta.y) > mFrontTheta)
				{
					mbRotate = true;
				}
				else
				{
					if (mbDash)
						Move(mTransform->Forward(), mDashSpeed);
					else
						Move(mTransform->Forward());
				}
			}
		}
		if (Input::GetKey(eKeyCode::A))
		{
			if (bLockOn)
			{
				Move(-mTransform->Right());
			}
			else
			{
				mLastDir = eDirection::Left;

				if (abs(theta.y) > mFrontTheta)
				{
					mbRotate = true;
				}
				else
				{
					if (mbDash)
						Move(mTransform->Forward(), mDashSpeed);
					else
						Move(mTransform->Forward());
				}
			}
		}

		if (Input::GetKeyUp(eKeyCode::W) || Input::GetKeyUp(eKeyCode::W) || Input::GetKeyUp(eKeyCode::S) || Input::GetKeyUp(eKeyCode::D))
		{
			player->SetStateFlag(ePlayerState::Walk, false);

			if (player->IsStateFlag(ePlayerState::Hang))
			{
				playerAnim->Play(L"a000_020000");
			}
			else if (player->IsStateFlag(ePlayerState::Crouch))
			{
				playerAnim->Play(L"a000_005000");
			}
			else if (player->IsStateFlag(ePlayerState::Block))
			{
				//playerAnim->Play(L"a050_002000");
			}
			else
			{
				if (bLockOn)
				{
					playerAnim->Play(L"a000_000501");
				}
				else
				{
					playerAnim->Play(L"a000_000000");
				}
			}
		}
	}

	void PlayerActionScript::Run()
	{
		PlayerMeshScript* playerAnim = GetOwner()->GetScript<PlayerMeshScript>();

		if (Input::GetKey(eKeyCode::LSHIFT))
		{
			playerAnim->Play(L"a000_001151");
		}

		if (mDashTimer > 0.0f)
		{
			Move(mTransform->Forward(), mDashSpeed);
			mDashTimer -= Time::DeltaTime();
		}

		if (Input::GetKeyDown(eKeyCode::LSHIFT))
		{
			playerAnim->Play(L"a000_001151");

			Vector3 limitVelocity = mRigidbody->GetLimitVelocity();
			mRigidbody->SetLimitVelocity(Vector3(50.0f, limitVelocity.y, 50.0f));

			mbDash = true;

			if (mDashTimer <= 0.0f)
			{
				mDashTimer = mDashTimerMax;
			}
		}

		if (Input::GetKeyUp(eKeyCode::LSHIFT))
		{
			Vector3 limitVelocity = mRigidbody->GetLimitVelocity();
			mRigidbody->SetLimitVelocity(Vector3(40.0f, limitVelocity.y, 40.0f));

			mbDash = false;
		}
	}
}
