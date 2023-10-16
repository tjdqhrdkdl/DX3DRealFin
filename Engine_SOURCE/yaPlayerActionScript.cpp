#include "yaPlayerActionScript.h"

#include "yaInput.h"
#include "yaTime.h"

#include "yaPlayer.h"

#include "yaTransform.h"
#include "yaRigidbody.h"
#include "yaCameraScript.h"
#include "yaPlayerMeshScript.h"
#include "yaBoneCollider.h"

namespace ya
{
	PlayerActionScript::PlayerActionScript()
		: ActionScript()
		, mPlayer(nullptr)
		, mPlayerAnim(nullptr)
		, mbRotate(false)
		, mLastDir(eDirection::Forward)
		, mbDash(false)
		, mDashSpeed(300.0f)
		, mDashTimer(0.0f)
		, mHitTimer(1.0f)
		, mFrontTheta(10.0f)
		, mDashTimerMax(0.2f)
		, mbJumpDouble(false)
	{


	}

	PlayerActionScript::~PlayerActionScript()
	{
	}

	void PlayerActionScript::Initialize()
	{
		ActionScript::Initialize();

		GameObject* owner = GetOwner();
		mPlayer = dynamic_cast<Player*>(GetOwner());
		mPlayerAnim = mPlayer->GetScript<PlayerMeshScript>();

		GetJumpEvent() = [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			PlayerMeshScript* playerAnim = player->GetScript<PlayerMeshScript>();

			playerAnim->Play(L"a000_200000");
			//playerAnim->Play(L"a000_201030");
			player->SetStateFlag(ePlayerState::Jump, true);
		};

		GetGroundEvent() = [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			PlayerMeshScript* playerAnim = player->GetScript<PlayerMeshScript>();
			playerAnim->Play(L"a000_201040");
			player->SetStateFlag(ePlayerState::Jump, false);
		};

		mPlayer->GetStartStateEvent().insert(std::make_pair(ePlayerState::Jump, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			player->SetStateFlag(ePlayerState::Idle, false);
			player->SetStateFlag(ePlayerState::Walk, false);
		}));
		mPlayer->GetEndStateEvent().insert(std::make_pair(ePlayerState::Jump, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			player->SetStateFlag(ePlayerState::Idle, true);
			player->SetStateFlag(ePlayerState::Walk, true);
		}));

		mPlayer->GetStartStateEvent().insert(std::make_pair(ePlayerState::Block, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			/*BoneCollider* waeponCollider = player->GetWeaponCollider();
			Transform* weaponColliderTr = waeponCollider->GetComponent<Transform>();
			Vector3 scale = weaponColliderTr->GetScale();
			waeponCollider->SetScale(Vector3(1.2f, 0.2f, 1.2f));*/

			PlayerActionScript* action = player->GetScript<PlayerActionScript>();
			action->Velocity(30.0f);
			
		}));

		mPlayer->GetEndStateEvent().insert(std::make_pair(ePlayerState::Block, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			/*BoneCollider* waeponCollider = player->GetWeaponCollider();
			waeponCollider->SetScale(Vector3(1.6f, 0.2f, 0.2f));*/

			PlayerActionScript* action = player->GetScript<PlayerActionScript>();
			action->Velocity();
		}));

		mPlayer->GetStartStateEvent().insert(std::make_pair(ePlayerState::Crouch, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			PlayerActionScript* action = player->GetScript<PlayerActionScript>();
			action->Velocity(20.0f);
		}));

		mPlayer->GetEndStateEvent().insert(std::make_pair(ePlayerState::Crouch, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			PlayerActionScript* action = player->GetScript<PlayerActionScript>();
			action->Velocity();
		}));
	}

	void PlayerActionScript::Update()
	{
		if(!mPlayer->IsStateFlag(ePlayerState::Hit))
		{
			Idle();
			Walk();
			Sprint();
			PlayerJump();
			Crouch();
			Walk();
			Hang();
		}

		Hit();

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

	void PlayerActionScript::Idle()
	{


	}

	void PlayerActionScript::Walk()
	{
		if (mDashTimer > 0.0f)
			return;

		if (mPlayer->IsStateFlag(ePlayerState::Jump))
			return;

		Vector3 pos = mTransform->GetPosition();
		Vector3 rot = mTransform->GetRotation();

		GameObject* camera = mPlayer->GetCamera();
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
					Rotate(Vector3(0.0f, 0.1f, 0.0f));
				else
					Rotate(Vector3(0.0f, -0.1f, 0.0f));
			}
		}

		if (Input::GetKeyDown(eKeyCode::W))
		{
			mPlayer->SetStateFlag(ePlayerState::Walk, true);

			if (mPlayer->IsStateFlag(ePlayerState::Block))
			{
				mPlayerAnim->Play(L"a050_002200");
			}
			else if (mPlayer->IsStateFlag(ePlayerState::Hang))
			{
			}
			else if (mPlayer->IsStateFlag(ePlayerState::Crouch))
			{
				mPlayerAnim->Play(L"a000_005100");
			}
			else
			{
				mPlayerAnim->Play(L"a000_000500");
			}
		}
		if (Input::GetKeyDown(eKeyCode::A))
		{
			mPlayer->SetStateFlag(ePlayerState::Walk, true);

			if (mPlayer->IsStateFlag(ePlayerState::Block))
			{
				mPlayerAnim->Play(L"a050_002200");
			}
			else if (mPlayer->IsStateFlag(ePlayerState::Hang))
			{

			}
			else if (mPlayer->IsStateFlag(ePlayerState::Crouch))
			{
				if (bLockOn)
					mPlayerAnim->Play(L"a000_005102");
				else
					mPlayerAnim->Play(L"a000_005100");
			}
			else
			{
				if (bLockOn)
				{
					mPlayerAnim->Play(L"a000_000503");
				}
				else
				{
					if (abs(theta.y) < mFrontTheta)
						mPlayerAnim->Play(L"a000_000500");
					else
						mPlayerAnim->Play(L"a000_000010");
				}
				
			}
		}
		if (Input::GetKeyDown(eKeyCode::S))
		{
			mPlayer->SetStateFlag(ePlayerState::Walk, true);

			if (mPlayer->IsStateFlag(ePlayerState::Block))
			{
				mPlayerAnim->Play(L"a050_002200");
			}
			else if (mPlayer->IsStateFlag(ePlayerState::Hang))
			{

			}
			else if (mPlayer->IsStateFlag(ePlayerState::Crouch))
			{
				if (bLockOn)
					mPlayerAnim->Play(L"a000_005101");
				else
					mPlayerAnim->Play(L"a000_005100");
			}
			else
			{
				if (bLockOn)
				{
					mPlayerAnim->Play(L"a000_000501");
				}
				else
				{
					if (abs(theta.y) < mFrontTheta)
						mPlayerAnim->Play(L"a000_000500");
					else
						mPlayerAnim->Play(L"a000_000011");
				}
			}
		}
		if (Input::GetKeyDown(eKeyCode::D))
		{
			mPlayer->SetStateFlag(ePlayerState::Walk, true);

			if (mPlayer->IsStateFlag(ePlayerState::Block))
			{
				mPlayerAnim->Play(L"a050_002200");
			}
			else if (mPlayer->IsStateFlag(ePlayerState::Hang))
			{

			}
			else if (mPlayer->IsStateFlag(ePlayerState::Crouch))
			{
				if (bLockOn)
					mPlayerAnim->Play(L"a000_005103");
				else
					mPlayerAnim->Play(L"a000_005100");
			}
			else
			{
				if (bLockOn)
				{
					mPlayerAnim->Play(L"a000_000502");
				}
				else
				{
					if (abs(theta.y) < mFrontTheta)
						mPlayerAnim->Play(L"a000_000500");
					else
						mPlayerAnim->Play(L"a000_000012");
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

		if (mPlayer->IsStateFlag(ePlayerState::Walk) &&
			Input::GetKeyNone(eKeyCode::W) && Input::GetKeyNone(eKeyCode::A) && Input::GetKeyNone(eKeyCode::S) && Input::GetKeyNone(eKeyCode::D))
		{
			mPlayer->SetStateFlag(ePlayerState::Walk, false);

			if (mPlayer->IsStateFlag(ePlayerState::Hang))
			{
				mPlayerAnim->Play(L"a000_020000");
			}
			else if (mPlayer->IsStateFlag(ePlayerState::Crouch))
			{
				mPlayerAnim->Play(L"a000_005000");
			}
			else
			{
				if (bLockOn)
				{
					mPlayerAnim->Play(L"a000_000501");
				}
				else
				{
					mPlayerAnim->Play(L"a000_000000");
				}
			}
		}
	}

	void PlayerActionScript::Sprint()
	{
		if (mDashTimer > 0.0f)
		{
			Move(mTransform->Forward(), mDashSpeed);
			mDashTimer -= Time::DeltaTime();
		}

		if (Input::GetKeyDown(eKeyCode::LSHIFT))
		{
			mPlayer->SetStateFlag(ePlayerState::Sprint, true);

			Velocity(70.0f);

			mbDash = true;

			if(mDashTimer <= 0.0f)
				mDashTimer = mDashTimerMax;
			
			if (mPlayer->IsStateFlag(ePlayerState::Walk))
			{
				mPlayerAnim->Play(L"a000_001200");
			}
			else if(mDashTimer == mDashTimerMax)
			{
				//mPlayerAnim->Play(L"a000_001510");
			}
		}

		if (Input::GetKeyUp(eKeyCode::LSHIFT))
		{
			mPlayer->SetStateFlag(ePlayerState::Sprint, false);

			Velocity();

			mbDash = false;
		}
	}

	void PlayerActionScript::Hang()
	{
		// 매달리기 임시
		if (Input::GetKeyDown(eKeyCode::H))
		{
			// TODO: 매달리기 -> 떨어지기, 올라가기 만들어야함
			if (mPlayer->IsStateFlag(ePlayerState::Hang))
			{
				mPlayer->SetStateFlag(ePlayerState::Hang, false);
				mPlayerAnim->Play(L"a000_000000");
			}
			else
			{
				mPlayer->SetStateFlag(ePlayerState::Hang, true);
				mPlayerAnim->Play(L"a000_020000");
			}
		}

		if(mPlayer->IsStateFlag(ePlayerState::Hang))
		{
			if (IsGrounded())
			{
				mPlayer->SetStateFlag(ePlayerState::Hang, false);
				mPlayerAnim->Play(L"a000_000000");
			}
		}
	}

	void PlayerActionScript::Crouch()
	{
		if (Input::GetKeyDown(eKeyCode::Q))
		{
			if (mPlayer->IsStateFlag(ePlayerState::Crouch))
			{
				mPlayer->SetStateFlag(ePlayerState::Crouch, false);
				mPlayerAnim->Play(L"a000_000000");
			}
			else
			{
				mPlayer->SetStateFlag(ePlayerState::Crouch, true);
				mPlayerAnim->Play(L"a000_005000");
			}
		}
	}

	void PlayerActionScript::Hit()
	{
		if(mPlayer->IsStateFlag(ePlayerState::Hit))
		{
			if (mHitTimer > 0.0f)
			{
				mHitTimer -= Time::DeltaTime();
			}
			else
			{
				mPlayer->SetStateFlag(ePlayerState::Hit, false);
				mHitTimer = 1.0f;
			}
		}
	}

	void PlayerActionScript::PlayerJump()
	{
		if (Input::GetKeyDown(eKeyCode::SPACE))
		{
			SetJumping(true);
			Jump();

			if (mbForwardBlocked && !mbJumpDouble)
			{
				mbJumpDouble = true;
			}
		}

		if (mPlayer->IsStateFlag(ePlayerState::Jump))
		{
			if (Input::GetKey(eKeyCode::W) || Input::GetKey(eKeyCode::A) || Input::GetKey(eKeyCode::S) || Input::GetKey(eKeyCode::D))
			{
				Move(mTransform->Forward(), 200.0f);
			}
		}
	}
}
