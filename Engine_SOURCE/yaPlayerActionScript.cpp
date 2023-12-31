#include "yaPlayerActionScript.h"

#include "yaInput.h"
#include "yaTime.h"
#include "yaResources.h"

#include "yaPlayer.h"

#include "yaTransform.h"
#include "yaRigidbody.h"
#include "yaCameraScript.h"
#include "yaPlayerMeshScript.h"
#include "yaBoneCollider.h"
#include "yaAudioClip.h"

#include "yaSceneManager.h"
#include "yaScene.h"
#include "yaPlayScene.h"
#include "yaLoadingScene.h"

namespace ya
{
	PlayerActionScript::PlayerActionScript()
		: ActionScript()
		, mPlayer(nullptr)
		, mPlayerAnim(nullptr)
		, mbRotate(false)
		, mLastDir(eDirection::Forward)
		, mbDash(false)
		, mDashSpeed(200.0f)
		, mDashTimer(0.0f)
		, mDashTimerMax(0.4f)
		, mDashDirection(eDirection::Forward)
		, mHitTimer(1.0f)
		, mHitTimerMax(1.0f)
		, mbTurn(false)
		, mTurnTimer(0.0f)
		, mTurnTimerMax(0.4f)
		, mGroggyTimer(0.0f)
		, mFrontTheta(5.0f)
		, mInvincibleTimer(true)
		, mBGMVolume(5)
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

		mRigidbody->GetJumpEvent() = [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			PlayerMeshScript* playerAnim = player->GetScript<PlayerMeshScript>();

			//playerAnim->Play(L"a000_200000");
			player->SetStateFlag(ePlayerState::Jump, true);
		};

		mRigidbody->GetGroundEvent() = [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			PlayerMeshScript* playerAnim = player->GetScript<PlayerMeshScript>();
			player->SetStateFlag(ePlayerState::Jump, false);
			player->SetStateFlag(ePlayerState::Hook, false);

			if (player->IsStateFlag(ePlayerState::Walk))
				playerAnim->Play(L"a000_000500");
			else
				playerAnim->Play(L"a000_201040");
		};

		mPlayer->GetState()->GetDeathEvent() = [this, owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			player->SetStateFlag(ePlayerState::Death, true);

			PlayerMeshScript* playerAnim = player->GetScript<PlayerMeshScript>();
			playerAnim->Play(L"a000_100300");

			player->OnDeathUI(true);
			player->SetControl(false, 3.0f);

			Resources::Find<AudioClip>(L"voice-m-dead")->Play();
		};

		mPlayer->GetState()->GetGameOverEvent() = [this, owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			player->SetStateFlag(ePlayerState::Death, true);

			PlayerMeshScript* playerAnim = player->GetScript<PlayerMeshScript>();
			playerAnim->Play(L"a000_100300");

			player->OnGameOverUI(true);
			player->SetControl(false, 5.0f);

			Resources::Find<AudioClip>(L"voice-m-dead")->Play();
		};

		mPlayer->GetState()->GetStunEvent() = [this, owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			player->SetStateFlag(ePlayerState::Groggy, true);

			PlayerMeshScript* playerAnim = player->GetScript<PlayerMeshScript>();
			playerAnim->Play(L"a000_110000");

			player->SetControl(false, 1.8f);
			mGroggyTimer = 1.8f;
		};

		mPlayer->GetState()->GetRessurctionEvent() = [this, owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			PlayerMeshScript* playerAnim = player->GetScript<PlayerMeshScript>();
			playerAnim->Play(L"a000_100320");
			Invincible(2.5f);

			player->OnDeathUI(false);
		};

		mPlayer->GetStartStateEvent().insert(std::make_pair(ePlayerState::Walk, [owner]() {
			Resources::Find<AudioClip>(L"foot-soil-w1")->Play();
		}));

		mPlayer->GetEndStateEvent().insert(std::make_pair(ePlayerState::Walk, [owner]() {
			Resources::Find<AudioClip>(L"foot-soil-w1")->Stop();
		}));

		mPlayer->GetStartStateEvent().insert(std::make_pair(ePlayerState::Sprint, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			PlayerActionScript* action = player->GetScript<PlayerActionScript>();
			action->Velocity(13.0f);
			player->SetStateFlag(ePlayerState::Walk, false);
			Resources::Find<AudioClip>(L"foot-soil-w1")->Play();
		}));

		mPlayer->GetEndStateEvent().insert(std::make_pair(ePlayerState::Sprint, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			PlayerActionScript* action = player->GetScript<PlayerActionScript>();
			action->Velocity();
			Resources::Find<AudioClip>(L"foot-soil-w1")->Stop();
			}));

		mPlayer->GetStartStateEvent().insert(std::make_pair(ePlayerState::Crouch, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			PlayerActionScript* action = player->GetScript<PlayerActionScript>();
			action->Velocity(8.0f);
			}));

		mPlayer->GetEndStateEvent().insert(std::make_pair(ePlayerState::Crouch, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			PlayerActionScript* action = player->GetScript<PlayerActionScript>();
			action->Velocity();
			}));

		mPlayer->GetStartStateEvent().insert(std::make_pair(ePlayerState::Hang, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			PlayerActionScript* action = player->GetScript<PlayerActionScript>();
			action->Velocity(5.0f);
			}));

		mPlayer->GetEndStateEvent().insert(std::make_pair(ePlayerState::Hang, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			PlayerActionScript* action = player->GetScript<PlayerActionScript>();
			action->Velocity();
			}));

		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\body-lobe-1.wav");
			Resources::Insert<AudioClip>(L"body-lobe-1", clip);
		}
		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\foot-soil-w1.wav");
			clip->SetLoop(true);
			Resources::Insert<AudioClip>(L"foot-soil-w1", clip);
			mAudioClips.push_back(clip);
		}

		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\voice-m-dead.wav");
			Resources::Insert<AudioClip>(L"voice-m-dead", clip);
		}

		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\bgm\\17. Approaching Forces.mp3");
			Resources::Insert<AudioClip>(L"bgm-boss", clip);
			mAudioClips.push_back(clip);
			clip->SetLoop(true);
			clip->SetVolume(0);

		}
		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\bgm\\11. Ashina Outskirts.mp3");
			Resources::Insert<AudioClip>(L"bgm-usual", clip);
			mAudioClips.push_back(clip);

			clip->SetLoop(true);
		}
		{
			std::shared_ptr<AudioClip> clip = Resources::Load<AudioClip>(L"perilous_attack", L"..\\Resources\\Sound\\Monster\\perilous_attack.mp3");
			mAudioClips.push_back(clip);
		}
		{
			std::shared_ptr<AudioClip> clip = Resources::Load<AudioClip>(L"recognize_sound", L"..\\Resources\\Sound\\Monster\\recognize_sound.mp3");
			mAudioClips.push_back(clip);
		}
		
	}

	void PlayerActionScript::Update()
	{
		if (!mPlayer->IsControl())
			return;

		if (mPlayer->IsStateFlag(ePlayerState::Death))
		{
			Death();
			return;
		}

		if (mPlayer->IsStateFlag(ePlayerState::Groggy))
		{
			Groggy();
			return;
		}

		if (mPlayer->IsStateFlag(ePlayerState::Invincible))
		{
			if (mInvincibleTimer > 0.0f)
			{
				mInvincibleTimer -= Time::DeltaTime();
			}
			else
			{
				mPlayer->SetStateFlag(ePlayerState::Invincible, false);
				mInvincibleTimer = 2.0f;
			}
		}

		if (mPlayer->IsStateFlag(ePlayerState::DeathBlow))
			return;

		if (!mPlayer->IsStateFlag(ePlayerState::Hook))
		{
			if (!mPlayer->IsStateFlag(ePlayerState::Hit))
			{
				Idle();
				Walk();

				if (!mPlayer->IsStateFlag(ePlayerState::Attack))
				{
					Sprint();
					PlayerJump();
					Crouch();
					Hang();
				}
			}

			Hit();
		}

		if (Input::GetKeyDown(eKeyCode::I))
		{
			mPlayer->SetStateFlag(ePlayerState::Invincible, !mPlayer->IsStateFlag(ePlayerState::Invincible));
		}

		ActionScript::Update();
	}

	void PlayerActionScript::FixedUpdate()
	{
		std::shared_ptr<AudioClip> bgmClip = Resources::Find<AudioClip>(L"bgm-usual");
		for (std::shared_ptr<AudioClip> clip : mAudioClips)
		{
			if (bgmClip == clip)
			{
				clip->Set3DAttributes(mTransform->GetPosition() + Vector3(0, 10 - mBGMVolume, 0), mPlayer->GetComponent<Rigidbody>()->GetVelocity());
			}
			else
				clip->Set3DAttributes(mTransform->GetPosition(), mPlayer->GetComponent<Rigidbody>()->GetVelocity());

		}
		if (Input::GetKeyDown(eKeyCode::I))
			mBGMVolume -= 1;
		if (Input::GetKeyDown(eKeyCode::O))
			mBGMVolume += 1;


		ActionScript::FixedUpdate();
	}


	void PlayerActionScript::Render()
	{
		ActionScript::Render();
	}

	void PlayerActionScript::AdjustState()
	{
		if (Input::GetKey(eKeyCode::W) || Input::GetKey(eKeyCode::A) || Input::GetKey(eKeyCode::S) || Input::GetKey(eKeyCode::D))
			mPlayer->SetStateFlag(ePlayerState::Walk, true);

		GameObject* camera = mPlayer->GetCamera();
		CameraScript* cameraScript = camera->GetScript<CameraScript>();
		bool bLockOn = cameraScript->IsLockOn();

		if (mPlayer->IsStateFlag(ePlayerState::Walk))
		{
			if (mPlayer->IsStateFlag(ePlayerState::Crouch))
			{
				if (bLockOn)
				{
					if (Input::GetKey(eKeyCode::W))
						mPlayerAnim->Play(L"a000_005100");
					else if (Input::GetKey(eKeyCode::S))
						mPlayerAnim->Play(L"a000_005101");
					else if (Input::GetKey(eKeyCode::D))
						mPlayerAnim->Play(L"a000_005103");
					else if (Input::GetKey(eKeyCode::A))
						mPlayerAnim->Play(L"a000_005102");
				}
				else
					mPlayerAnim->Play(L"a000_005100");
			}
			else if (mPlayer->IsStateFlag(ePlayerState::Block))
			{
				if (bLockOn)
				{
					if (Input::GetKey(eKeyCode::W))
						mPlayerAnim->Play(L"a050_002200");
					else if (Input::GetKey(eKeyCode::S))
						mPlayerAnim->Play(L"a050_002201");
					else if (Input::GetKey(eKeyCode::D))
						mPlayerAnim->Play(L"a050_002202");
					else if (Input::GetKey(eKeyCode::A))
						mPlayerAnim->Play(L"a050_002203");
				}
				else
					mPlayerAnim->Play(L"a050_002200");
			}
			else
			{
				if (bLockOn)
				{
					if (Input::GetKey(eKeyCode::W))
						mPlayerAnim->Play(L"a000_000400");
					else if (Input::GetKey(eKeyCode::S))
						mPlayerAnim->Play(L"a000_000401");
					else if (Input::GetKey(eKeyCode::D))
						mPlayerAnim->Play(L"a000_000402");
					else if (Input::GetKey(eKeyCode::A))
						mPlayerAnim->Play(L"a000_000403");
				}
				else
					mPlayerAnim->Play(L"a000_000400");
			}
		}
		else
		{
			if (mPlayer->IsStateFlag(ePlayerState::Crouch))
			{
				mPlayerAnim->Play(L"a000_005000");
			}
			else if (mPlayer->IsStateFlag(ePlayerState::Block))
			{
				mPlayerAnim->Play(L"a050_002000");
			}
			else
			{
				mPlayerAnim->Play(L"a000_000000");
			}
		}
	}

	void PlayerActionScript::Invincible(float time)
	{
		mPlayer->SetStateFlag(ePlayerState::Invincible, true); mInvincibleTimer = time;
	}

	void PlayerActionScript::Idle()
	{
	}

	void PlayerActionScript::Walk()
	{
		if (mDashTimer > 0.0f)
			return;

		if (mPlayer->IsStateFlag(ePlayerState::Attack))
			return;

		Vector3 pos = mTransform->GetPosition();
		Vector3 rot = mTransform->GetRotation();

		GameObject* camera = mPlayer->GetCamera();
		CameraScript* cameraScript = camera->GetScript<CameraScript>();
		Transform* cameraTr = camera->GetComponent<Transform>();
		Vector3 cameraPos = cameraTr->GetPosition();
		Vector3 cameraForward = cameraTr->Forward();
		Vector3 cameraRight = cameraTr->Right();
		bool bLockOn = cameraScript->IsLockOn();

		float theta = 0.0;
		Vector3 cross = Vector3::Zero;
		if (bLockOn)
		{	// lockon 상태인 경우 항상 lockon 타겟을 바라본다.
			GameObject* lockOnTarget = cameraScript->GetLockOnTarget();
			Vector3 lockOnTargetPos = lockOnTarget->GetComponent<Transform>()->GetPosition();

			Quaternion quater = Quaternion::FromToRotation(mTransform->Forward(), Vector3(lockOnTargetPos.x - pos.x, 0.0f, lockOnTargetPos.z - pos.z));
			Vector3 quaterToEuler = quater.ToEuler();
			Vector3 quaterTheta = quaterToEuler * 180.0f / XM_PI;
			
			theta = quaterTheta.y;

			mbRotate = true;
		}
		else
		{	// lockon 상태가 아닌 경우 카메라를 등진다.
			Vector3 dir = mTransform->Forward();
			if (mLastDir == eDirection::Forward)
				dir = mTransform->Forward();
			if (mLastDir == eDirection::Back)
				dir = -mTransform->Forward();
			if (mLastDir == eDirection::Right)
				dir = -mTransform->Right();
			if (mLastDir == eDirection::Left)
				dir = mTransform->Right();

			// player가 바라볼 방향과 camera 사이의 각도를 구한다.
			Vector3 cameraDir = Vector3(cameraForward.x, 0.0f, cameraForward.z);
			cameraDir.Normalize();
			theta = dir.Dot(cameraDir);
			cross = dir.Cross(cameraDir);
			theta = acos(theta);
			theta *= 180.0f / XM_PI;
		}

		if (mbRotate)
		{
			if (theta < mFrontTheta)
			{	// 회전 종료. 진행하려는 방향과 player의 forward가 비슷해지면 회전이 끝난다.
				mbRotate = false;
				if (cross.y < 0.0f)
					mTransform->SetRotation(Vector3(0.0f, rot.y - theta, 0.0f));
				else
					mTransform->SetRotation(Vector3(0.0f, rot.y + theta, 0.0f));
			}
			else
			{	// 진행하려는 방향과 player의 forward가 비슷해질 때 까지 회전한다. theta 각에 따라 회전 방향을 결정한다.
				if(cross.y < 0.0f && theta > 5.0f)
					Rotate(Vector3(0.0f, -(0.05f + 2.95f / 180.0f * theta), 0.0f));
				else 	
					Rotate(Vector3(0.0f, (0.05f + 2.95f / 180.0f * theta), 0.0f));
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
				Vector3 cameraDir = Vector3(cameraForward.x, 0.0f, cameraForward.z);
				cameraDir.Normalize();
				float faceTheta = mTransform->Forward().Dot(cameraDir);
				faceTheta = acos(faceTheta);
				faceTheta *= 180.0f / XM_PI;

				if (faceTheta > 90.0f)
					mPlayerAnim->Play(L"a000_000012");
				else if (faceTheta < -90.0f)
					mPlayerAnim->Play(L"a000_000013");
				else if (faceTheta > 30.0f)
					mPlayerAnim->Play(L"a000_000010");
				else if (faceTheta < -30.0f)
					mPlayerAnim->Play(L"a000_000011");
				else
					mPlayerAnim->Play(L"a000_000400");

				if (abs(faceTheta) > 30.0f)
					mbTurn = true;
			}
		}
		if (Input::GetKeyDown(eKeyCode::A))
		{
			mPlayer->SetStateFlag(ePlayerState::Walk, true);

			if (mPlayer->IsStateFlag(ePlayerState::Block))
			{
				if (bLockOn)
					mPlayerAnim->Play(L"a050_002203");
				else
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
					mPlayerAnim->Play(L"a000_000403");
				}
				else
				{
					Vector3 cameraDir = Vector3(-cameraRight.x, 0.0f, -cameraRight.z);
					cameraDir.Normalize();
					float faceTheta = mTransform->Forward().Dot(cameraDir);
					faceTheta = acos(faceTheta);
					faceTheta *= 180.0f / XM_PI;

					if (faceTheta > 90.0f)
						mPlayerAnim->Play(L"a000_000012");
					else if (faceTheta < -90.0f)
						mPlayerAnim->Play(L"a000_000013");
					else if (faceTheta > 30.0f)
						mPlayerAnim->Play(L"a000_000010");
					else if (faceTheta < -30.0f)
						mPlayerAnim->Play(L"a000_000011");
					else
						mPlayerAnim->Play(L"a000_000400");

					if (abs(faceTheta) > 30.0f)
						mbTurn = true;
				}
			}
		}
		if (Input::GetKeyDown(eKeyCode::S))
		{
			mPlayer->SetStateFlag(ePlayerState::Walk, true);

			if (mPlayer->IsStateFlag(ePlayerState::Block))
			{
				if (bLockOn)
					mPlayerAnim->Play(L"a050_002200");
				else
					mPlayerAnim->Play(L"a050_002201");
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
					mPlayerAnim->Play(L"a000_000401");
				}
				else
				{
					Vector3 cameraDir = Vector3(cameraForward.x, 0.0f, cameraForward.z);
					cameraDir.Normalize();
					float faceTheta = mTransform->Forward().Dot(cameraDir);
					faceTheta = acos(faceTheta);
					faceTheta *= 180.0f / XM_PI;

					if (faceTheta > 90.0f)
						mPlayerAnim->Play(L"a000_000012");
					else if (faceTheta < -90.0f)
						mPlayerAnim->Play(L"a000_000013");
					else if (faceTheta > 30.0f)
						mPlayerAnim->Play(L"a000_000010");
					else if (faceTheta < -30.0f)
						mPlayerAnim->Play(L"a000_000011");
					else
						mPlayerAnim->Play(L"a000_000400");

					if (abs(faceTheta) > 30.0f)
						mbTurn = true;
				}
			}
		}
		if (Input::GetKeyDown(eKeyCode::D))
		{
			mPlayer->SetStateFlag(ePlayerState::Walk, true);

			if (mPlayer->IsStateFlag(ePlayerState::Block))
			{
				if (bLockOn)
					mPlayerAnim->Play(L"a050_002202");
				else
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
					mPlayerAnim->Play(L"a000_000402");
				}
				else
				{
					Vector3 cameraDir = Vector3(cameraRight.x, 0.0f, cameraRight.z);
					cameraDir.Normalize();
					float faceTheta = mTransform->Forward().Dot(cameraDir);
					faceTheta = acos(faceTheta);
					faceTheta *= 180.0f / XM_PI;

					if (faceTheta > 90.0f)
						mPlayerAnim->Play(L"a000_000012");
					else if (faceTheta < -90.0f)
						mPlayerAnim->Play(L"a000_000013");
					else if (faceTheta > 30.0f)
						mPlayerAnim->Play(L"a000_000010");
					else if (faceTheta < -30.0f)
						mPlayerAnim->Play(L"a000_000011");
					else
						mPlayerAnim->Play(L"a000_000400");

					if (abs(faceTheta) > 30.0f)
						mbTurn = true;
				}
			}
		}

		if (!mPlayer->IsStateFlag(ePlayerState::Attack))
		{
			if (Input::GetKey(eKeyCode::W))
			{
				if (bLockOn)
				{
					Move(mTransform->Forward());
				}
				else
				{
					if (abs(theta) > mFrontTheta) // 진행하려는 방향과 각도 차이가 날때 회전시킨다.
						mbRotate = true;

					if (mbDash)
						Move(cameraForward, mDashSpeed);
					else
						Move(cameraForward);

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
					if (abs(theta) > mFrontTheta)
						mbRotate = true;

					if (mbDash)
						Move(-cameraForward, mDashSpeed);
					else
						Move(-cameraForward);

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
					if (abs(theta) > mFrontTheta)
						mbRotate = true;

					if (mbDash)
						Move(cameraRight, mDashSpeed);
					else
						Move(cameraRight);

					mLastDir = eDirection::Right;
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
					if (abs(theta) > mFrontTheta)
						mbRotate = true;

					if (mbDash)
						Move(-cameraRight, mDashSpeed);
					else
						Move(-cameraRight);

					mLastDir = eDirection::Left;
				}
			}
		}

		if (Input::GetKeyUp(eKeyCode::W))
		{
			if (mPlayer->IsStateFlag(ePlayerState::Crouch))
			{
				mPlayerAnim->Play(L"a000_005300");
			}
			else if (mPlayer->IsStateFlag(ePlayerState::Sprint))
			{
				mPlayerAnim->Play(L"a000_001510");
			}
			else if (mPlayer->IsStateFlag(ePlayerState::Walk))
			{
				mPlayerAnim->Play(L"a000_000600");
			}
			else
			{
				mPlayerAnim->Play(L"a000_000000");
			}
		}
		if (Input::GetKeyUp(eKeyCode::A))
		{
			if (mPlayer->IsStateFlag(ePlayerState::Crouch))
			{
				if (bLockOn)
					mPlayerAnim->Play(L"a000_005302");
				else
					mPlayerAnim->Play(L"a000_005300");
			}
			else if (mPlayer->IsStateFlag(ePlayerState::Sprint))
			{
				if (bLockOn)
					mPlayerAnim->Play(L"a000_001511");
				else
					mPlayerAnim->Play(L"a000_001510");
			}
			else if (mPlayer->IsStateFlag(ePlayerState::Walk))
			{
				if (bLockOn)
					mPlayerAnim->Play(L"a000_000602");
				else
					mPlayerAnim->Play(L"a000_000600");
			}
			else
			{
				mPlayerAnim->Play(L"a000_000000");
			}
		}
		if (Input::GetKeyUp(eKeyCode::S))
		{
			if (mPlayer->IsStateFlag(ePlayerState::Crouch))
			{
				if (bLockOn)
					mPlayerAnim->Play(L"a000_005301");
				else
					mPlayerAnim->Play(L"a000_005300");
			}
			else if (mPlayer->IsStateFlag(ePlayerState::Sprint))
			{
				if (bLockOn)
					mPlayerAnim->Play(L"a000_001510");
				else
					mPlayerAnim->Play(L"a000_001510");
			}
			else if (mPlayer->IsStateFlag(ePlayerState::Walk))
			{
				if (bLockOn)
					mPlayerAnim->Play(L"a000_000601");
				else
					mPlayerAnim->Play(L"a000_000600");
			}
			else
			{
				mPlayerAnim->Play(L"a000_000000");
			}
		}
		if (Input::GetKeyUp(eKeyCode::D))
		{
			if (mPlayer->IsStateFlag(ePlayerState::Crouch))
			{
				if (bLockOn)
					mPlayerAnim->Play(L"a000_005303");
				else
					mPlayerAnim->Play(L"a000_005300");
			}
			else if (mPlayer->IsStateFlag(ePlayerState::Sprint))
			{
				if (bLockOn)
					mPlayerAnim->Play(L"a000_001512");
				else
					mPlayerAnim->Play(L"a000_001510");
			}
			else if (mPlayer->IsStateFlag(ePlayerState::Walk))
			{
				if (bLockOn)
					mPlayerAnim->Play(L"a000_000603");
				else
					mPlayerAnim->Play(L"a000_000600");
			}
			else
			{
				mPlayerAnim->Play(L"a000_000000");
			}
		}

		if (mbTurn)
		{
			if (mTurnTimer > 0.0f)
			{
				mTurnTimer -= Time::DeltaTime();
			}
			else
			{
				mbTurn = false;
				mTurnTimer = mTurnTimerMax;

				if (mPlayer->IsStateFlag(ePlayerState::Walk))
					mPlayerAnim->Play(L"a000_000500");
				else if (mPlayer->IsStateFlag(ePlayerState::Idle))
					mPlayerAnim->Play(L"a000_000000");
			}
		}

		if (mPlayer->IsStateFlag(ePlayerState::Walk) &&
			Input::GetKeyNone(eKeyCode::W) && Input::GetKeyNone(eKeyCode::A) && Input::GetKeyNone(eKeyCode::S) && Input::GetKeyNone(eKeyCode::D))
		{
			mPlayer->SetStateFlag(ePlayerState::Walk, false);

			//if (mPlayer->IsStateFlag(ePlayerState::Hang))
			//{
			//	mPlayerAnim->Play(L"a000_020000");
			//}
			//else if (mPlayer->IsStateFlag(ePlayerState::Crouch))
			//{
			//	mPlayerAnim->Play(L"a000_005000");
			//}
			//else
			//{
			//	/*if (bLockOn)
			//	{
			//		mPlayerAnim->Play(L"a000_000501");
			//	}
			//	else
			//	{
			//		mPlayerAnim->Play(L"a000_000000");
			//	}*/
			//}
		}
	}

	void PlayerActionScript::Sprint()
	{
		if (mPlayer->IsStateFlag(ePlayerState::Attack) || mPlayer->IsStateFlag(ePlayerState::Jump) || mPlayer->IsStateFlag(ePlayerState::Block))
			return;

		if (mbDash)
		{
			GameObject* camera = mPlayer->GetCamera();
			CameraScript* cameraScript = camera->GetScript<CameraScript>();
			bool bLockOn = cameraScript->IsLockOn();

			if (mDashTimer > 0.0f)
			{
				if(bLockOn)
				{
					if (mDashDirection == eDirection::Left)
						Move(-mTransform->Right(), mDashSpeed);
					else if (mDashDirection == eDirection::Right)
						Move(mTransform->Right(), mDashSpeed);
					else if (mDashDirection == eDirection::Back)
						Move(-mTransform->Forward(), mDashSpeed);
					else
						Move(mTransform->Forward(), mDashSpeed);
				}
				else
				{
					Move(mTransform->Forward(), mDashSpeed);
				}

				mDashTimer -= Time::DeltaTime();
			}
			else
			{
				if (Input::GetKey(eKeyCode::LSHIFT)
					&& (Input::GetKey(eKeyCode::W) || Input::GetKey(eKeyCode::S) || Input::GetKey(eKeyCode::A) || Input::GetKey(eKeyCode::D)))
				{
					//mPlayerAnim->Play(L"a000_001200");
				}
				else
				{
					mbDash = false;
					Velocity();

					if (Input::GetKey(eKeyCode::W) || Input::GetKey(eKeyCode::S) || Input::GetKey(eKeyCode::A) || Input::GetKey(eKeyCode::D))
					{
						if (bLockOn)
						{
							if (Input::GetKey(eKeyCode::W))
								mPlayerAnim->Play(L"a000_000500");
							else if (Input::GetKey(eKeyCode::S))
								mPlayerAnim->Play(L"a000_000501");
							else if (Input::GetKey(eKeyCode::A))
								mPlayerAnim->Play(L"a000_000502");
							else if (Input::GetKey(eKeyCode::D))
								mPlayerAnim->Play(L"a000_000503");
							else
								mPlayerAnim->Play(L"a000_000500");
						}
						else
						{
							mPlayerAnim->Play(L"a000_000500");
						}
					}
					else
					{
						if (bLockOn)
						{
							if (mDashDirection == eDirection::Left)
								mPlayerAnim->Play(L"a000_001511");
							else if (mDashDirection == eDirection::Right)
								mPlayerAnim->Play(L"a000_001512");
							else
								mPlayerAnim->Play(L"a000_001510");
						}
						else
						{
							mPlayerAnim->Play(L"a000_001510");
						}
					}
				}
			}
		}

		if (Input::GetKeyDown(eKeyCode::LSHIFT))
		{
			mPlayer->SetStateFlag(ePlayerState::Sprint, true);

			mbDash = true;

			if (mDashTimer <= 0.0f)
				mDashTimer = mDashTimerMax;

			if (Input::GetKey(eKeyCode::A))
			{
				mDashDirection = eDirection::Left;
				//mPlayerAnim->Play(L"a000_001153");
			}
			else if (Input::GetKey(eKeyCode::D))
			{
				mDashDirection = eDirection::Right;
				//mPlayerAnim->Play(L"a000_001152");
			}
			else if (Input::GetKey(eKeyCode::S))
			{
				mDashDirection = eDirection::Back;
				//mPlayerAnim->Play(L"a000_001154");
			}
			else
			{
				mDashDirection = eDirection::Forward;
				//mPlayerAnim->Play(L"a000_001151");
			}
			mPlayerAnim->Play(L"a000_001151");

		}

		if (Input::GetKey(eKeyCode::LSHIFT))
		{
			mPlayer->SetStateFlag(ePlayerState::Sprint, true);
		}

		if (Input::GetKeyUp(eKeyCode::LSHIFT))
		{
			mPlayer->SetStateFlag(ePlayerState::Sprint, false);

			if (Input::GetKey(eKeyCode::W) || Input::GetKey(eKeyCode::S) || Input::GetKey(eKeyCode::A) || Input::GetKey(eKeyCode::D))
			{
				mPlayer->SetStateFlag(ePlayerState::Walk, true);
				//mPlayerAnim->Play(L"a000_000500");
			}
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
				mPlayerAnim->Play(L"a000_217500");
			}
			else
			{
				mPlayer->SetStateFlag(ePlayerState::Hang, true);
				mPlayerAnim->Play(L"a000_217103");
			}
		}

		if (mPlayer->IsStateFlag(ePlayerState::Hang))
		{
			/*if (Input::GetKeyDown(eKeyCode::E))
			{
				mPlayer->SetStateFlag(ePlayerState::Hang, false);
				mPlayerAnim->Play(L"a000_217500_");
			}
			else */if(Input::GetKeyDown(eKeyCode::LSHIFT))
			{
				mPlayer->SetStateFlag(ePlayerState::Hang, false);
				mPlayerAnim->Play(L"a000_217620");

				mRigidbody->SetGrounded(false);
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
				AdjustState();
			}
			else
			{
				mPlayer->SetStateFlag(ePlayerState::Crouch, true);
				mPlayerAnim->Play(L"a000_005000");
				AdjustState();
			}
		}
	}

	void PlayerActionScript::Hit()
	{
		if (mPlayer->IsStateFlag(ePlayerState::Hit))
		{
			if (mHitTimer > 0.0f)
			{
				mHitTimer -= Time::DeltaTime();
			}
			else
			{
				mPlayer->SetStateFlag(ePlayerState::Hit, false);
				mHitTimer = 1.0f;

				AdjustState();
			}
		}
	}

	void PlayerActionScript::Groggy()
	{
		if (mGroggyTimer > 0.0f)
		{
			mGroggyTimer -= Time::DeltaTime();
		}
		else
		{
			mPlayer->GetState()->SetPosture(0.0f);
			mPlayer->SetStateFlag(ePlayerState::Groggy, false);
		}
	}

	void PlayerActionScript::Death()
	{
		if (Input::GetKeyDown(eKeyCode::LBTN))
		{
			if (mPlayer->GetState()->GetResurrectionCount() > 0)
			{	// 회생한다
				mPlayer->GetState()->Resurrection();
				mPlayer->SetStateFlag(ePlayerState::Death, false);
			}
			else
			{ // 회생할 수 없으므로 게임 리셋
				PlayScene* playScene = dynamic_cast<PlayScene*>(SceneManager::GetScene(eSceneType::Play));
				playScene->Reset();

				LoadingScene* loadingScene = dynamic_cast<LoadingScene*>(SceneManager::GetScene(eSceneType::Loading));
				loadingScene->SetLoading(2.f);

				SceneManager::LoadScene(eSceneType::Loading);
			}
		}
		else if (Input::GetKeyDown(eKeyCode::RBTN))
		{
			if (mPlayer->GetState()->GetResurrectionCount() > 0)
			{ // 회생하지 않고 포기한다(게임 리셋)
				PlayScene* playScene = dynamic_cast<PlayScene*>(SceneManager::GetScene(eSceneType::Play));
				playScene->Reset();

				LoadingScene* loadingScene = dynamic_cast<LoadingScene*>(SceneManager::GetScene(eSceneType::Loading));
				loadingScene->SetLoading(2.f);

				SceneManager::LoadScene(eSceneType::Loading);
			}
			else
			{
				PlayScene* scene = dynamic_cast<PlayScene*>(SceneManager::GetScene(eSceneType::Play));
				scene->Reset();

				LoadingScene* loadingScene = dynamic_cast<LoadingScene*>(SceneManager::GetScene(eSceneType::Loading));
				loadingScene->SetLoading();

				SceneManager::LoadScene(eSceneType::Title);
			}
		}
	}

	void PlayerActionScript::PlayerJump()
	{
		if (Input::GetKeyDown(eKeyCode::SPACE))
		{
			if (mPlayer->IsStateFlag(ePlayerState::Jump))
			{
				if(mRigidbody->IsForwardBlocked() && mRigidbody->GetJumpCount() == 1)
					JumpDouble();
			}
			else
			{
				mPlayerAnim->Play(L"a000_200000");
				mRigidbody->SetJumping(true);
				Jump();
			}
		}
	}
}