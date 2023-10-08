#include "yaPlayerAttackScript.h"

#include "yaObject.h"
#include "yaInput.h"
#include "yaTime.h"

#include "yaPlayer.h"

#include "yaTransform.h"
#include "yaRigidbody.h"

#include "yaPlayerMeshScript.h"
#include "yaPlayerActionScript.h"
#include "yaPlayerProjectileScript.h"

#include "yaBoneCollider.h"
#include "yaObject.h"

#include "yaCameraScript.h"

namespace ya
{
	PlayerAttackScript::PlayerAttackScript()
		: Script()
		, mPlayer(nullptr)
		, mPlayerAnim(nullptr)
		, mAttackState(eAttackState::None)
		, mTimer{0.0f}
		, mTimerMax{ 0.0f,  0.8f, 0.8f, 0.8f, 0.8f, 0.8f,  0.5f, 0.5f, 0.5f,  0.8f, 0.8f, 0.8f, 0.4f}
		, mbKeyInput(false)
		, mBlockTime(0.0f)
	{
	}

	PlayerAttackScript::~PlayerAttackScript()
	{
	}

	void PlayerAttackScript::Initialize()
	{
		GameObject* owner = GetOwner();
		mPlayer = dynamic_cast<Player*>(GetOwner());
		mPlayerAnim = mPlayer->GetScript<PlayerMeshScript>();

		for (size_t i = 0; i < (UINT)eAttackState::End; i++)
		{
			mTimer[i] = mTimerMax[i];
		}

		BoneCollider* weaponCollider = mPlayer->GetWeaponCollider();
		if (weaponCollider != nullptr)
		{
			std::wstring arm = ARM;
			//weaponCollider->SetAnimOffSet(L"SwingSword1", Vector3(1, 0.5, 1));
			weaponCollider->SetColliderActiveFrame(L"a050_300100_" + arm, 5, 40);
			weaponCollider->SetColliderActiveFrame(L"a050_305101_" + arm, 5, 40);
			weaponCollider->SetColliderActiveFrame(L"a050_300020_" + arm, 5, 40);
			weaponCollider->SetColliderActiveFrame(L"a050_300030_" + arm, 5, 40);
			weaponCollider->SetColliderActiveFrame(L"a050_300040_" + arm, 5, 40);

			weaponCollider->SetColliderActiveFrame(L"a050_308010_" + arm, 0, 20);
			weaponCollider->SetColliderActiveFrame(L"a050_308000_" + arm, 0, 20);
			weaponCollider->SetColliderActiveFrame(L"a050_308050_" + arm, 0, 20);
			weaponCollider->SetColliderActiveFrame(L"a050_308060_" + arm, 0, 20);

			//weaponCollider->SetColliderActiveFrame(L"a050_301050_" + arm, 0, 100);
			weaponCollider->SetColliderActiveFrame(L"a050_314000_" + arm, 0, 20);
			//weaponCollider->SetColliderActiveFrame(L"a050_002000_" + arm, 0, 100);
		}

		mPlayer->GetStartStateEvent().insert(std::make_pair(ePlayerState::Attack, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			player->SetStateFlag(ePlayerState::Idle, false);
			player->SetStateFlag(ePlayerState::Walk, false);
			player->SetStateFlag(ePlayerState::Sprint, false);
			player->SetStateFlag(ePlayerState::Block, false);
			player->SetStateFlag(ePlayerState::Wall, false);
		}));
		mPlayer->GetEndStateEvent().insert(std::make_pair(ePlayerState::Attack, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			player->SetStateFlag(ePlayerState::Idle, true);

			PlayerActionScript* action = player->GetScript<PlayerActionScript>();
			action->AdjustState();
		}));
	}

	void PlayerAttackScript::Update()
	{
		Transform* playerTr = mPlayer->GetComponent<Transform>();
		PlayerActionScript* playerAction = mPlayer->GetScript<PlayerActionScript>();

		if (mTimer[(UINT)eAttackState::Move] > 0.0f)
		{
			mTimer[(UINT)eAttackState::Move] -= Time::DeltaTime();
			if(mTimer[(UINT)eAttackState::Move] < 0.2f)
				playerAction->Move(playerTr->Forward(), 300.0f);
		}

		switch (mAttackState)
		{
		case ya::PlayerAttackScript::eAttackState::None:
		{
			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				mPlayer->SetStateFlag(ePlayerState::Attack, true);

				if (mPlayer->IsStateFlag(ePlayerState::Jump))
				{
					mAttackState = eAttackState::JumpAttack1;
					mPlayerAnim->Play(L"a050_308010");
					
				}
				else if (mPlayer->IsStateFlag(ePlayerState::Crouch))
				{
					mAttackState = eAttackState::CrouchAttack1;
					mPlayer->SetStateFlag(ePlayerState::Crouch, false);
					mPlayerAnim->Play(L"a050_301050");
					mTimer[(UINT)eAttackState::Move] = mTimerMax[(UINT)eAttackState::Move];
				}
				else if (mPlayer->IsStateFlag(ePlayerState::Hang))
				{
					mAttackState = eAttackState::HangAttack1;
					mPlayerAnim->Play(L"a050_314000");
				}
				else
				{
					mAttackState = eAttackState::Attack1;
					mPlayerAnim->Play(L"a050_300100");
					mTimer[(UINT)eAttackState::Move] = mTimerMax[(UINT)eAttackState::Move];
				}
			}

			if (Input::GetKeyDown(eKeyCode::RBTN))
			{
				mPlayer->SetStateFlag(ePlayerState::Block, true);
				mAttackState = eAttackState::Block;

				GameObject* camera = mPlayer->GetCamera();
				CameraScript* cameraScript = camera->GetScript<CameraScript>();
				bool bLockOn = cameraScript->IsLockOn();

				if (mPlayer->IsStateFlag(ePlayerState::Walk))
				{
					if (bLockOn)
					{
						if(Input::GetKey(eKeyCode::W))
							mPlayerAnim->Play(L"a050_002010");
						else if(Input::GetKey(eKeyCode::S))
							mPlayerAnim->Play(L"a050_002011");
						else if(Input::GetKey(eKeyCode::D))
							mPlayerAnim->Play(L"a050_002012");
						else if(Input::GetKey(eKeyCode::A))
							mPlayerAnim->Play(L"a050_002013");
					}
					else
					{
						mPlayerAnim->Play(L"a050_002010");
					}
				}
				else
				{
					mPlayerAnim->Play(L"a050_002000");
				}
			}
		}
		break;
		case ya::PlayerAttackScript::eAttackState::Attack1:
		{
			if (mTimer[(UINT)eAttackState::Attack1] <= 0.0f)
			{
				if (mbKeyInput)
				{
					mAttackState = eAttackState::Attack2;
					mPlayerAnim->Play(L"a050_305101");
					mTimer[(UINT)eAttackState::Move] = mTimerMax[(UINT)eAttackState::Move];
				}
				else
				{
					mAttackState = eAttackState::None;
					mPlayerAnim->Play(L"a000_000000");
					mPlayer->SetStateFlag(ePlayerState::Attack, false);
				}

				mTimer[(UINT)eAttackState::Attack1] = mTimerMax[(UINT)eAttackState::Attack1];
				mbKeyInput = false;
			}
			else
			{
				mTimer[(UINT)eAttackState::Attack1] -= Time::DeltaTime();
			}

			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				mbKeyInput = true;
			}
		}
		break;
		case ya::PlayerAttackScript::eAttackState::Attack2:
		{
			if (mTimer[(UINT)eAttackState::Attack2] <= 0.0f)
			{
				if (mbKeyInput)
				{
					mAttackState = eAttackState::Attack3;
					mPlayerAnim->Play(L"a050_300020");
					mTimer[(UINT)eAttackState::Move] = mTimerMax[(UINT)eAttackState::Move];
				}
				else
				{
					mAttackState = eAttackState::None;
					mPlayerAnim->Play(L"a000_000000");
					mPlayer->SetStateFlag(ePlayerState::Attack, false);
				}

				mTimer[(UINT)eAttackState::Attack2] = mTimerMax[(UINT)eAttackState::Attack2];
				mbKeyInput = false;
			}
			else
			{
				mTimer[(UINT)eAttackState::Attack2] -= Time::DeltaTime();
			}

			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				mbKeyInput = true;
			}
		}
		break;
		case ya::PlayerAttackScript::eAttackState::Attack3:
		{
			if (mTimer[(UINT)eAttackState::Attack3] <= 0.0f)
			{
				if (mbKeyInput)
				{
					mAttackState = eAttackState::Attack4;
					mPlayerAnim->Play(L"a050_300030");
					mTimer[(UINT)eAttackState::Move] = mTimerMax[(UINT)eAttackState::Move];
				}
				else
				{
					mAttackState = eAttackState::None;
					mPlayerAnim->Play(L"a000_000000");
					mPlayer->SetStateFlag(ePlayerState::Attack, false);
				}

				mTimer[(UINT)eAttackState::Attack3] = mTimerMax[(UINT)eAttackState::Attack3];
				mbKeyInput = false;
			}
			else
			{
				mTimer[(UINT)eAttackState::Attack3] -= Time::DeltaTime();
			}

			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				mbKeyInput = true;
			}
		}
		break;
		case ya::PlayerAttackScript::eAttackState::Attack4:
		{
			if (mTimer[(UINT)eAttackState::Attack4] <= 0.0f)
			{
				if (mbKeyInput)
				{
					mAttackState = eAttackState::Attack5;
					mPlayerAnim->Play(L"a050_300040");
					mTimer[(UINT)eAttackState::Move] = mTimerMax[(UINT)eAttackState::Move];
				}
				else
				{
					mAttackState = eAttackState::None;
					mPlayerAnim->Play(L"a000_000000");
					mPlayer->SetStateFlag(ePlayerState::Attack, false);
				}

				mTimer[(UINT)eAttackState::Attack4] = mTimerMax[(UINT)eAttackState::Attack4];
				mbKeyInput = false;
			}
			else
			{
				mTimer[(UINT)eAttackState::Attack4] -= Time::DeltaTime();
			}

			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				mbKeyInput = true;
			}
		}
		break;
		case ya::PlayerAttackScript::eAttackState::Attack5:
		{
			if (mTimer[(UINT)eAttackState::Attack5] <= 0.0f)
			{
				if (mbKeyInput)
				{
					mAttackState = eAttackState::Attack1;
					mPlayerAnim->Play(L"a050_300100");
					mTimer[(UINT)eAttackState::Move] = mTimerMax[(UINT)eAttackState::Move];
				}
				else
				{
					mAttackState = eAttackState::None;
					mPlayerAnim->Play(L"a000_000000");
					mPlayer->SetStateFlag(ePlayerState::Attack, false);
				}

				mTimer[(UINT)eAttackState::Attack5] = mTimerMax[(UINT)eAttackState::Attack5];
				mbKeyInput = false;
			}
			else
			{
				mTimer[(UINT)eAttackState::Attack5] -= Time::DeltaTime();
			}

			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				mbKeyInput = true;
			}
		}
		break;
		case ya::PlayerAttackScript::eAttackState::Block:
		{
			mBlockTime += Time::DeltaTime();

			if (Input::GetKeyUp(eKeyCode::RBTN))
			{
				mAttackState = eAttackState::None;
				mPlayer->SetStateFlag(ePlayerState::Block, false);

				GameObject* camera = mPlayer->GetCamera();
				CameraScript* cameraScript = camera->GetScript<CameraScript>();
				bool bLockOn = cameraScript->IsLockOn();

				if (mPlayer->IsStateFlag(ePlayerState::Walk))
				{
					if (bLockOn)
					{
						if (Input::GetKey(eKeyCode::W))
							mPlayerAnim->Play(L"a050_002010");
						else if (Input::GetKey(eKeyCode::S))
							mPlayerAnim->Play(L"a050_002011");
						else if (Input::GetKey(eKeyCode::D))
							mPlayerAnim->Play(L"a050_002012");
						else if (Input::GetKey(eKeyCode::A))
							mPlayerAnim->Play(L"a050_002013");
					}
					else
					{
						mPlayerAnim->Play(L"a050_002010");
					}
				}
				else
				{
					mPlayerAnim->Play(L"a050_002000");
				}

				mBlockTime = 0.0f;
			}
		}
		break;
		case ya::PlayerAttackScript::eAttackState::JumpAttack1:
		{
			if (mTimer[(UINT)eAttackState::JumpAttack1] <= 0.0f)
			{
				if (mbKeyInput)
				{
					mAttackState = eAttackState::JumpAttack2;

					if (mPlayer->IsStateFlag(ePlayerState::Jump))
					{
						mPlayerAnim->Play(L"a050_308000");
					}
					else
					{
						mPlayerAnim->Play(L"a050_308050");
					}
				}
				else
				{
					mAttackState = eAttackState::None;
					if (mPlayer->IsStateFlag(ePlayerState::Jump))
					{
					}
					else
					{
						mPlayerAnim->Play(L"a000_000000");
					}
					mPlayer->SetStateFlag(ePlayerState::Attack, false);
				}

				mTimer[(UINT)eAttackState::JumpAttack1] = mTimerMax[(UINT)eAttackState::JumpAttack1];
				mbKeyInput = false;
			}
			else
			{
				mTimer[(UINT)eAttackState::JumpAttack1] -= Time::DeltaTime();
			}

			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				mbKeyInput = true;
			}
		}
		break;
		case ya::PlayerAttackScript::eAttackState::JumpAttack2:
		{
			if (mTimer[(UINT)eAttackState::JumpAttack2] <= 0.0f)
			{
				if (mbKeyInput)
				{
					mAttackState = eAttackState::JumpAttack3;
					if (mPlayer->IsStateFlag(ePlayerState::Jump))
					{
						mPlayerAnim->Play(L"a050_308060");
					}
					else
					{
						mPlayerAnim->Play(L"a050_308060");
					}
				}
				else
				{
					mAttackState = eAttackState::None;
					if (mPlayer->IsStateFlag(ePlayerState::Jump))
					{
					}
					else
					{
						mPlayerAnim->Play(L"a000_000000");
					}
					mPlayer->SetStateFlag(ePlayerState::Attack, false);
				}

				mTimer[(UINT)eAttackState::JumpAttack2] = mTimerMax[(UINT)eAttackState::JumpAttack2];
				mbKeyInput = false;
			}
			else
			{
				mTimer[(UINT)eAttackState::JumpAttack2] -= Time::DeltaTime();
			}

			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				mbKeyInput = true;
			}
		}
		break;
		case ya::PlayerAttackScript::eAttackState::JumpAttack3:
		{
			if (mTimer[(UINT)eAttackState::JumpAttack3] <= 0.0f)
			{
				if (mbKeyInput)
				{
					if (mPlayer->IsStateFlag(ePlayerState::Jump))
					{
						mAttackState = eAttackState::JumpAttack1;
						mPlayerAnim->Play(L"a050_308010");
					}
					else
					{
						mAttackState = eAttackState::Attack1;
						mPlayerAnim->Play(L"a050_300100");
					}
				}
				else
				{
					mAttackState = eAttackState::None;
					if (mPlayer->IsStateFlag(ePlayerState::Jump))
					{
						mPlayerAnim->Play(L"a000_201030");
					}
					else
					{
						mPlayerAnim->Play(L"a000_000000");
					}
					mPlayer->SetStateFlag(ePlayerState::Attack, false);
				}

				mTimer[(UINT)eAttackState::JumpAttack3] = mTimerMax[(UINT)eAttackState::JumpAttack3];
				mbKeyInput = false;
			}
			else
			{
				mTimer[(UINT)eAttackState::JumpAttack3] -= Time::DeltaTime();
			}

			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				mbKeyInput = true;
			}
		}
		break;
		case ya::PlayerAttackScript::eAttackState::CrouchAttack1:
		{
			if (mTimer[(UINT)eAttackState::CrouchAttack1] <= 0.0f)
			{
				if (mbKeyInput)
				{
					mAttackState = eAttackState::Attack2;
					mPlayerAnim->Play(L"a050_305101");
				}
				else
				{
					mAttackState = eAttackState::None;
					mPlayerAnim->Play(L"a000_000000");
					mPlayer->SetStateFlag(ePlayerState::Attack, false);
				}

				mTimer[(UINT)eAttackState::CrouchAttack1] = mTimerMax[(UINT)eAttackState::CrouchAttack1];
				mbKeyInput = false;
			}
			else
			{
				mTimer[(UINT)eAttackState::CrouchAttack1] -= Time::DeltaTime();
			}

			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				mbKeyInput = true;
			}
		}
		break;
		case ya::PlayerAttackScript::eAttackState::HangAttack1:
		{
			if (mTimer[(UINT)eAttackState::HangAttack1] <= 0.0f)
			{
				mAttackState = eAttackState::None;
				mPlayer->SetStateFlag(ePlayerState::Attack, false);
				mTimer[(UINT)eAttackState::HangAttack1] = mTimerMax[(UINT)eAttackState::HangAttack1];
			}


			if (mTimer[(UINT)eAttackState::HangAttack1] <= 0.0f)
			{
				if (mbKeyInput)
				{
					mAttackState = eAttackState::HangAttack1;
					mPlayerAnim->Play(L"a050_314000");
				}
				else
				{
					mAttackState = eAttackState::None;
					mPlayerAnim->Play(L"a000_020000");
					mPlayer->SetStateFlag(ePlayerState::Attack, false);
				}

				mTimer[(UINT)eAttackState::HangAttack1] = mTimerMax[(UINT)eAttackState::HangAttack1];
				mbKeyInput = false;
			}
			else
			{
				mTimer[(UINT)eAttackState::HangAttack1] -= Time::DeltaTime();
			}

			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				mbKeyInput = true;
			}
		}
		break;
		default:
			break;
		}
	}

	void PlayerAttackScript::FixedUpdate()
	{
	}


	void PlayerAttackScript::Render()
	{
	}
}
