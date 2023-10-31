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

#include "yaApplication.h"

#include "yaSwordMan.h"
#include "yaAshinaSoldier.h"
#include "yaAshinaSpearMan.h"
#include "yaTenzen.h"

#include "yaAudioClip.h"
#include "yaResources.h"

extern ya::Application application;

namespace ya
{
	PlayerAttackScript::PlayerAttackScript()
		: Script()
		, mPlayer(nullptr)
		, mPlayerAnim(nullptr)
		, mAttackState(eAttackState::None)
		, mTimer{ 0.0f }
		, mTimerMax{ 0.0f,  0.8f, 0.8f, 0.8f, 0.8f, 0.8f,  0.5f, 0.5f, 0.5f,  0.8f, 0.8f,  0.8f,  0.4f, 0.15f, 3.0f }
		, mbKeyInput(false)
		, mHitDirection(Vector3::Zero)
		, mDeathBlowTarget(nullptr)
		, mDeathBlowTargets{}
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
		if (weaponCollider != nullptr && mPlayerAnim->IsUse())
		{
			std::wstring arm = ARM;
			//weaponCollider->SetAnimOffSet(L"SwingSword1", Vector3(1, 0.5, 1));
			weaponCollider->SetColliderActiveFrame(L"a050_300100_" + arm, 5, 40);
			weaponCollider->SetColliderActiveFrame(L"a050_305101_" + arm, 0, 40);
			weaponCollider->SetColliderActiveFrame(L"a050_300020_" + arm, 5, 40);
			weaponCollider->SetColliderActiveFrame(L"a050_300030_" + arm, 5, 40);
			weaponCollider->SetColliderActiveFrame(L"a050_300040_" + arm, 5, 40);

			weaponCollider->SetColliderActiveFrame(L"a050_308010_" + arm, 0, 20);
			weaponCollider->SetColliderActiveFrame(L"a050_308000_" + arm, 0, 20);
			weaponCollider->SetColliderActiveFrame(L"a050_308050_" + arm, 0, 20);
			weaponCollider->SetColliderActiveFrame(L"a050_308060_" + arm, 0, 20);

			weaponCollider->SetColliderActiveFrame(L"a050_301050_" + arm, 0, 100);
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

		mPlayer->GetStartStateEvent().insert(std::make_pair(ePlayerState::Block, [this, owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			player->SetStateFlag(ePlayerState::Sprint, false);
			player->SetStateFlag(ePlayerState::Attack, false);

			PlayerActionScript* action = player->GetScript<PlayerActionScript>();
			action->Velocity(8.0f);
		}));

		mPlayer->GetEndStateEvent().insert(std::make_pair(ePlayerState::Block, [this, owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			PlayerActionScript* action = player->GetScript<PlayerActionScript>();
			action->Velocity();
			action->AdjustState();

			mTimer[(UINT)eAttackState::Block] = 0.0f;
		}));

		mPlayer->GetStartStateEvent().insert(std::make_pair(ePlayerState::DeathBlow, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			player->SetStateFlag(ePlayerState::Walk, false);
			player->SetStateFlag(ePlayerState::Jump, false);
			player->SetStateFlag(ePlayerState::Crouch, false);
			player->SetStateFlag(ePlayerState::Sprint, false);
			player->SetStateFlag(ePlayerState::Attack, false);
			player->SetStateFlag(ePlayerState::Block, false);
		}));

		mPlayer->GetEndStateEvent().insert(std::make_pair(ePlayerState::DeathBlow, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			player->SetStateFlag(ePlayerState::Idle, false);
		}));

		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\swing-katana.wav");
			Resources::Insert<AudioClip>(L"swing-katana", clip);
		}

		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\kill-successx2.wav");
			Resources::Insert<AudioClip>(L"kill-successx2", clip);
		}

		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\kill3.wav");
			Resources::Insert<AudioClip>(L"kill3", clip);
		}

		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\damage_SE.wav");
			Resources::Insert<AudioClip>(L"damage_SE1", clip);
		}
		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\damage_SE2.wav");
			Resources::Insert<AudioClip>(L"damage_SE2", clip);
		}
		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\damage_SE3.wav");
			Resources::Insert<AudioClip>(L"damage_SE3", clip);
		}

		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\voice-m-damage-m-1.wav");
			Resources::Insert<AudioClip>(L"voice-m-damage-m-1", clip);
		}
		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\voice-m-damage-m-2.wav");
			Resources::Insert<AudioClip>(L"voice-m-damage-m-2", clip);
		}
		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\voice-m-damage-m-3.wav");
			Resources::Insert<AudioClip>(L"voice-m-damage-m-3", clip);
		}

		std::wstring weapon = WEAPON;
		mPlayerAnim->FindMeshData(weapon)->GetAnimationFrameEvent(L"a200_510000_" + weapon, 20) = []() { Resources::Find<AudioClip>(L"damage_SE" + std::to_wstring(RandomNumber(1, 3)))->Play(); };
		mPlayerAnim->FindMeshData(weapon)->GetAnimationFrameEvent(L"a200_510000_" + weapon, 50) = []() { Resources::Find<AudioClip>(L"kill3")->Play(); };
		mPlayerAnim->FindMeshData(weapon)->GetAnimationFrameEvent(L"a201_510000_" + weapon, 65) = []() { Resources::Find<AudioClip>(L"damage_SE" + std::to_wstring(RandomNumber(1, 3)))->Play(); };
		mPlayerAnim->FindMeshData(weapon)->GetAnimationFrameEvent(L"a201_510000_" + weapon, 86) = []() { Resources::Find<AudioClip>(L"kill3")->Play(); };
	}

	void PlayerAttackScript::Update()
	{
		if (mPlayer->IsStateFlag(ePlayerState::Hook) || mPlayer->IsStateFlag(ePlayerState::Death) || mPlayer->IsStateFlag(ePlayerState::Groggy) || !mPlayer->IsControl())
			return;

		Transform* playerTr = mPlayer->GetComponent<Transform>();
		PlayerActionScript* playerAction = mPlayer->GetScript<PlayerActionScript>();

		if (mTimer[(UINT)eAttackState::HitMove] > 0.0f)
		{
			mTimer[(UINT)eAttackState::HitMove] -= Time::DeltaTime();
			playerAction->Move(mHitDirection, 200.0f);
		}

		if (mTimer[(UINT)eAttackState::AttackMove] > 0.0f)
		{
			mTimer[(UINT)eAttackState::AttackMove] -= Time::DeltaTime();
			if (mTimer[(UINT)eAttackState::AttackMove] < 0.2f)
				playerAction->Move(playerTr->Forward(), 300.0f);
		}

		switch (mAttackState)
		{
		case ya::PlayerAttackScript::eAttackState::None:
		{
			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				if (mDeathBlowTarget != nullptr)
				{
					DeathBlow(mDeathBlowTarget);
					mAttackState = eAttackState::DeathBlow;
					return;
				}

				mPlayer->SetStateFlag(ePlayerState::Attack, true);
				Resources::Find<AudioClip>(L"swing-katana")->Play();

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
					mTimer[(UINT)eAttackState::AttackMove] = mTimerMax[(UINT)eAttackState::AttackMove];
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
					mTimer[(UINT)eAttackState::AttackMove] = mTimerMax[(UINT)eAttackState::AttackMove];
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
			}
		}
		break;
		case ya::PlayerAttackScript::eAttackState::Attack1:
		{
			if (mTimer[(UINT)eAttackState::Attack1] <= 0.0f)
			{
				if (mbKeyInput)
				{
					if (mDeathBlowTarget != nullptr)
					{
						DeathBlow(mDeathBlowTarget);
						mAttackState = eAttackState::DeathBlow;
						return;
					}

					mAttackState = eAttackState::Attack2;
					mPlayerAnim->Play(L"a050_305101");
					mTimer[(UINT)eAttackState::AttackMove] = mTimerMax[(UINT)eAttackState::AttackMove];
					Resources::Find<AudioClip>(L"swing-katana")->Play();
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
					if (mDeathBlowTarget != nullptr)
					{
						DeathBlow(mDeathBlowTarget);
						mAttackState = eAttackState::DeathBlow;
						return;
					}

					mAttackState = eAttackState::Attack3;
					mPlayerAnim->Play(L"a050_300020");
					mTimer[(UINT)eAttackState::AttackMove] = mTimerMax[(UINT)eAttackState::AttackMove];
					Resources::Find<AudioClip>(L"swing-katana")->Play();
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
					if (mDeathBlowTarget != nullptr)
					{
						DeathBlow(mDeathBlowTarget);
						mAttackState = eAttackState::DeathBlow;
						return;
					}

					mAttackState = eAttackState::Attack4;
					mPlayerAnim->Play(L"a050_300030");
					mTimer[(UINT)eAttackState::AttackMove] = mTimerMax[(UINT)eAttackState::AttackMove];
					Resources::Find<AudioClip>(L"swing-katana")->Play();
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
					if (mDeathBlowTarget != nullptr)
					{
						DeathBlow(mDeathBlowTarget);
						mAttackState = eAttackState::DeathBlow;
						return;
					}

					mAttackState = eAttackState::Attack5;
					mPlayerAnim->Play(L"a050_300040");
					mTimer[(UINT)eAttackState::AttackMove] = mTimerMax[(UINT)eAttackState::AttackMove];
					Resources::Find<AudioClip>(L"swing-katana")->Play();
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
					if (mDeathBlowTarget != nullptr)
					{
						DeathBlow(mDeathBlowTarget);
						mAttackState = eAttackState::DeathBlow;
						return;
					}

					mAttackState = eAttackState::Attack1;
					mPlayerAnim->Play(L"a050_300100");
					mTimer[(UINT)eAttackState::AttackMove] = mTimerMax[(UINT)eAttackState::AttackMove];
					Resources::Find<AudioClip>(L"swing-katana")->Play();
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
			mTimer[(UINT)eAttackState::Block] += Time::DeltaTime();

			if (mTimer[(UINT)eAttackState::Block] > 0.3f)
			{
				if (!Input::GetKey(eKeyCode::RBTN))
				{
					mAttackState = eAttackState::None;
					mPlayer->SetStateFlag(ePlayerState::Block, false);

					playerAction->AdjustState();
				}
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
		case ya::PlayerAttackScript::eAttackState::HitMove:
		{
			if (mTimer[(UINT)eAttackState::HitMove] <= 0.0f)
				mAttackState = eAttackState::None;
		}
		case ya::PlayerAttackScript::eAttackState::DeathBlow:
		{
			if (mTimer[(UINT)eAttackState::DeathBlow] <= 0.0f)
			{
				mAttackState = eAttackState::None;
				mPlayer->SetStateFlag(ePlayerState::DeathBlow, false);
				mTimer[(UINT)eAttackState::DeathBlow] = mTimerMax[(UINT)eAttackState::DeathBlow];
			}
			else
			{
				mTimer[(UINT)eAttackState::DeathBlow] -= Time::DeltaTime();
			}
		}
		default:
			break;
		}
	}

	void PlayerAttackScript::FixedUpdate()
	{
	}

	void PlayerAttackScript::Render()
	{
		/*wchar_t szFloat[50] = {};
		std::wstring str = L"target: " + std::to_wstring(mDeathBlowTargets.size());
		swprintf_s(szFloat, 50, str.c_str());
		TextOut(application.GetHdc(), 800, 150, szFloat, (int)wcslen(szFloat));*/
	}

	void PlayerAttackScript::OnCollisionEnter(Collider2D* collider)
	{
		// 인살중이거나 죽었을 때는 피격받지 않는다
		if (mPlayer->IsStateFlag(ePlayerState::DeathBlow) || mPlayer->IsStateFlag(ePlayerState::Death) || mPlayer->IsStateFlag(ePlayerState::Invincible))
			return;

		GameObject* obj = collider->GetOwner();

		BoneCollider* boneCollider = dynamic_cast<BoneCollider*>(obj);
		if (boneCollider != nullptr)
		{
			MonsterBase* monster = dynamic_cast<MonsterBase*>(boneCollider->GetBCOwner());
			MonsterBase::Attack attackParam = monster->GetAttackParams();

			Transform* playerTr = mPlayer->GetComponent<Transform>();
			Vector3 playerPos = playerTr->GetPosition();
			Vector3 playerDir = playerTr->Forward();

			Vector3 colliderPos = collider->GetPosition();
			GameObject* other = collider->GetOwner();
			Transform* otherTr = other->GetComponent<Transform>();


			// 플레이어의 방향과 몬스터간의 각도를 구한다.
			float theta = playerTr->Forward().Dot(otherTr->Forward());
			Vector3 cross = playerTr->Forward().Cross(otherTr->Forward());
			theta = acos(theta);
			theta *= 180.0f / XM_PI;
			theta *= (cross.y / abs(cross.y));


			if (mPlayer->IsStateFlag(ePlayerState::Block) && (!attackParam.unGuardable || (mPlayer->GetBlockTime() < 1.0f)))
			{
				mPlayer->SetStateFlag(ePlayerState::Block, false);
				mAttackState = eAttackState::None;

				// 플레이어의 방향과 collider간의 각도를 구한다.
				Quaternion quater = Quaternion::FromToRotation(playerDir, Vector3(colliderPos.x - playerPos.x, playerPos.y, colliderPos.z - playerPos.z));
				Vector3 quaterToEuler = quater.ToEuler();
				Vector3 theta = quaterToEuler * 180.0f / XM_PI;

				if (attackParam.damage > 50.0f)
				{
					// 충돌 각도에 따라 피격 방향(애니메이션) 달라짐
					if (theta.y > -45.0f && theta.y <= 45.0f)
						mPlayerAnim->Play(L"a050_120301");
					else if (theta.y > 45.0f && theta.y <= 135.0f)
						mPlayerAnim->Play(L"a050_120301");
					else if (theta.y > 135.0f && theta.y <= 180.0f)
						mPlayerAnim->Play(L"a050_120301");
					else if (theta.y > -180.0f && theta.y <= -135.0f)
						mPlayerAnim->Play(L"a050_120301");
					else if (theta.y > -135.0f && theta.y <= -45.0f)
						mPlayerAnim->Play(L"a050_120302");

					mPlayer->GetState()->AddPosture(30.0f);
				}
				else
				{
					// 충돌 각도에 따라 피격 방향(애니메이션) 달라짐
					if (theta.y > -45.0f && theta.y <= 45.0f)
						mPlayerAnim->Play(L"a050_120101");
					else if (theta.y > 45.0f && theta.y <= 135.0f)
						mPlayerAnim->Play(L"a050_120100");
					else if (theta.y > 135.0f && theta.y <= 180.0f)
						mPlayerAnim->Play(L"a050_120103");
					else if (theta.y > -180.0f && theta.y <= -135.0f)
						mPlayerAnim->Play(L"a050_120103");
					else if (theta.y > -135.0f && theta.y <= -45.0f)
						mPlayerAnim->Play(L"a050_120102");

					mPlayer->GetState()->AddPosture(10.0f);
				}

				//Resources::Find<AudioClip>(L"sword-x-sword" + std::to_wstring(RandomNumber(1, 3)))->Play();
				Resources::Find<AudioClip>(L"c000006601_" + std::to_wstring(RandomNumber(1, 3)))->Play();

				mTimer[(UINT)eAttackState::HitMove] = mTimerMax[(UINT)eAttackState::HitMove];
			}
			else
			{
				mPlayer->SetStateFlag(ePlayerState::Hit, true); // 경직상태
				//mAttackState = eAttackState::HitMove;
				mAttackState = eAttackState::None;

				if (attackParam.damage > 50.0f)
				{
					if (!mPlayer->IsStateFlag(ePlayerState::Groggy))
					{
						// 충돌 각도에 따라 피격 방향(애니메이션) 달라짐
						if (theta > -45.0f && theta <= 45.0f)
							mPlayerAnim->Play(L"a000_100202");
						else if (theta > 45.0f && theta <= 135.0f)
							mPlayerAnim->Play(L"a000_100200");
						else if (theta > 135.0f && theta <= 180.0f || theta > -180.0f && theta <= -135.0f)
							mPlayerAnim->Play(L"a000_100203");
						else if (theta > -135.0f && theta <= -45.0f)
							mPlayerAnim->Play(L"a000_100201");


						if (theta < 90.0f && theta >= -90.0f)
							mHitDirection = -playerTr->Forward();
						else
							mHitDirection = playerTr->Forward();

						// 피격 당했을때 밀려나는 로직
						if (mTimer[(UINT)eAttackState::HitMove] <= 0.0f)
							mTimer[(UINT)eAttackState::HitMove] = 0.3f;

						mPlayer->GetState()->AddPosture(50.0f);
					}
					
					mPlayer->GetState()->AddHp(-50.0f);

					Resources::Find<AudioClip>(L"damage_SE" + std::to_wstring(RandomNumber(1, 3)))->Play();
					Resources::Find<AudioClip>(L"voice-m-damage-m-" + std::to_wstring(RandomNumber(1, 3)))->Play();
				}
				else
				{
					// 충돌 각도에 따라 피격 방향(애니메이션) 달라짐
					if (!mPlayer->IsStateFlag(ePlayerState::Groggy))
					{
						if (theta > -45.0f && theta <= 45.0f)
							mPlayerAnim->Play(L"a000_100102");
						else if (theta > 45.0f && theta <= 135.0f)
							mPlayerAnim->Play(L"a000_100100");
						else if (theta > 135.0f && theta <= 180.0f || theta > -180.0f && theta <= -135.0f)
							mPlayerAnim->Play(L"a000_100103");
						else if (theta > -135.0f && theta <= -45.0f)
							mPlayerAnim->Play(L"a000_100101");
					
						if(theta < 90.0f && theta >= -90.0f)
							mHitDirection = -playerTr->Forward();
						else
							mHitDirection = playerTr->Forward();

						// 피격 당했을때 밀려나는 로직
						if (mTimer[(UINT)eAttackState::HitMove] <= 0.0f)
							mTimer[(UINT)eAttackState::HitMove] = mTimerMax[(UINT)eAttackState::HitMove];

						mPlayer->GetState()->AddPosture(30.0f);
					}

					mPlayer->GetState()->AddHp(-30.0f);

					Resources::Find<AudioClip>(L"damage_SE" + std::to_wstring(RandomNumber(1, 3)))->Play();
					Resources::Find<AudioClip>(L"voice-m-damage-m-" + std::to_wstring(RandomNumber(1, 3)))->Play();
				}
			}
		}
	}
	void PlayerAttackScript::OnCollisionStay(Collider2D* collider)
	{
	}
	void PlayerAttackScript::OnCollisionExit(Collider2D* collider)
	{
	}
	void PlayerAttackScript::SetDeathBlowTarget(MonsterBase* _monster, float _distance)
	{
		if (mDeathBlowTargets.find(_monster) == mDeathBlowTargets.end())
		{
			bool bTarget = true;
			for (auto pair : mDeathBlowTargets)
			{
				MonsterBase* monster = pair.first;
				float dist = pair.second;

				if (dist < _distance)
					bTarget = false;
			}

			if (bTarget)
				mDeathBlowTarget = _monster;

			mDeathBlowTargets.insert(std::make_pair(_monster, _distance));
		}
		else
		{
			mDeathBlowTargets.find(_monster)->second = _distance;
		}
	}
	void PlayerAttackScript::EraseDeathBlowTarget(MonsterBase* monster)
	{
		if (mDeathBlowTargets.find(monster) != mDeathBlowTargets.end())
		{
			mDeathBlowTargets.erase(monster);

			if(mDeathBlowTargets.size() == 0)
				mDeathBlowTarget = nullptr;
		}
	}

	void PlayerAttackScript::DeathBlow(MonsterBase* monster)
	{
		mPlayer->SetStateFlag(ePlayerState::DeathBlow, true);

		// 플레이어가 몬스터를 보도록 회전한다.
		Transform* playerTr = GetOwner()->GetComponent<Transform>();
		Vector3 playerPos = playerTr->GetPosition();

		Transform* monsterTr = monster->GetComponent<Transform>();
		Vector3 monsterPos = monsterTr->GetPosition();

		Quaternion quater = Quaternion::FromToRotation(playerTr->Forward(), Vector3(monsterPos.x - playerPos.x, 0.0f, monsterPos.z - playerPos.z));
		Vector3 quaterToEuler = quater.ToEuler();
		Vector3 quaterTheta = quaterToEuler * 180.0f / XM_PI;

		playerTr->SetRotation(Vector3(0.0f, playerTr->GetRotation().y + quaterTheta.y, 0.0f));
		playerTr->FixedUpdate();

		// 인살 가능한 몬스터가 있는 상태일때
		// 몬스터 

		if (dynamic_cast<SwordMan*>(monster) != nullptr)
		{ 
			if (monster->IsRecognize())
			{
				mPlayerAnim->Play(L"a200_510000");
			}
			else
			{
				//암살 인살
				mPlayerAnim->Play(L"a200_510000");
			}
		}
		else if (dynamic_cast<AshinaSoldier*>(monster) != nullptr)
		{
			if (monster->IsRecognize())
			{
				mPlayerAnim->Play(L"a200_510000");
			}
			else
			{
				//암살 인살
				mPlayerAnim->Play(L"a200_510000");
			}
			EraseDeathBlowTarget(monster);
			mDeathBlowTarget = nullptr;
			monster->DeathBlow();
		}
		else if (dynamic_cast<AshinaSpearMan*>(monster) != nullptr)
		{
			if (monster->IsRecognize())
			{
				mPlayerAnim->Play(L"a200_510000");
			}
			else
			{
				//암살 인살
				mPlayerAnim->Play(L"a200_510000");
			}
			EraseDeathBlowTarget(monster);
			mDeathBlowTarget = nullptr;
			monster->DeathBlow();
		}
		else if (dynamic_cast<Tenzen*>(monster) != nullptr)
		{
			if (monster->IsRecognize())
			{	
				if (monster->GetResurrectionCount() > 0)
					mPlayerAnim->Play(L"a201_510000");
				else
					mPlayerAnim->Play(L"a201_510000");
			}
			else
			{
				mPlayerAnim->Play(L"a201_510000");
			}
		}

		Resources::Find<AudioClip>(L"kill-successx2")->Play();

		EraseDeathBlowTarget(monster);
		mDeathBlowTarget = nullptr;
		monster->DeathBlow();
	}
}