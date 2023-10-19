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

extern ya::Application application;

namespace ya
{
	PlayerAttackScript::PlayerAttackScript()
		: Script()
		, mPlayer(nullptr)
		, mPlayerAnim(nullptr)
		, mAttackState(eAttackState::None)
		, mTimer{0.0f}
		, mTimerMax{ 0.0f,  0.8f, 0.8f, 0.8f, 0.8f, 0.8f,  0.5f, 0.5f, 0.5f,  0.8f, 0.8f,  0.8f,  0.4f, 0.15f, 2.0f}
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

		mPlayer->GetStartStateEvent().insert(std::make_pair(ePlayerState::Block, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			player->SetStateFlag(ePlayerState::Sprint, false);
			player->SetStateFlag(ePlayerState::Attack, false);
			
			PlayerActionScript* action = player->GetScript<PlayerActionScript>();
			action->Velocity(8.0f);
		}));

		mPlayer->GetEndStateEvent().insert(std::make_pair(ePlayerState::Block, [owner]() {
			Player* player = dynamic_cast<Player*>(owner);
			PlayerActionScript* action = player->GetScript<PlayerActionScript>();
			action->Velocity();
		}));

	}

	void PlayerAttackScript::Update()
	{
		if (mPlayer->IsStateFlag(ePlayerState::Hook))
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
			if(mTimer[(UINT)eAttackState::AttackMove] < 0.2f)
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
					mTimer[(UINT)eAttackState::AttackMove] = mTimerMax[(UINT)eAttackState::AttackMove];
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
					mTimer[(UINT)eAttackState::AttackMove] = mTimerMax[(UINT)eAttackState::AttackMove];
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
					mTimer[(UINT)eAttackState::AttackMove] = mTimerMax[(UINT)eAttackState::AttackMove];
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
					mTimer[(UINT)eAttackState::AttackMove] = mTimerMax[(UINT)eAttackState::AttackMove];
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
					mTimer[(UINT)eAttackState::AttackMove] = mTimerMax[(UINT)eAttackState::AttackMove];
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

			if (Input::GetKeyUp(eKeyCode::RBTN))
			{
				mAttackState = eAttackState::None;
				mPlayer->SetStateFlag(ePlayerState::Block, false);

				GameObject* camera = mPlayer->GetCamera();
				CameraScript* cameraScript = camera->GetScript<CameraScript>();
				bool bLockOn = cameraScript->IsLockOn();

				playerAction->AdjustState();

				mTimer[(UINT)eAttackState::Block] = 0.0f;
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
		}
		case ya::PlayerAttackScript::eAttackState::DeathBlow:
		{
			if(mTimer[(UINT)eAttackState::DeathBlow] <= 0.0f)
			{
				mAttackState = eAttackState::None;
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
		wchar_t szFloat[50] = {};
		std::wstring str = L"target: " + std::to_wstring(mDeathBlowTargets.size());
		swprintf_s(szFloat, 50, str.c_str());
		TextOut(application.GetHdc(), 800, 150, szFloat, (int)wcslen(szFloat));
	}

	void PlayerAttackScript::OnCollisionEnter(Collider2D* collider)
	{
		GameObject* obj = collider->GetOwner();

		BoneCollider* boneCollider = dynamic_cast<BoneCollider*>(obj);
		if (boneCollider != nullptr)
		{
			Transform* playerTr = mPlayer->GetComponent<Transform>();
			Vector3 playerPos = playerTr->GetPosition();
			Vector3 playerDir = playerTr->Forward();

			Vector3 colliderPos = collider->GetPosition();
			GameObject* other = collider->GetOwner();
			Transform* otherTr = other->GetComponent<Transform>();

			if (mPlayer->IsStateFlag(ePlayerState::Block))
			{
				mPlayer->SetStateFlag(ePlayerState::Block, false);

				mPlayer->GetState()->AddPosture(-10);

				// 플레이어의 방향과 collider간의 각도를 구한다.
				Quaternion quater = Quaternion::FromToRotation(playerDir, Vector3(colliderPos.x - playerPos.x, playerPos.y, colliderPos.z - playerPos.z));
				Vector3 quaterToEuler = quater.ToEuler();
				Vector3 theta = quaterToEuler * 180.0f / XM_PI;

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
			}
			else
			{
				mPlayer->GetState()->AddHp(-10);
				mPlayer->SetStateFlag(ePlayerState::Hit, true); // 경직상태

				// 플레이어의 방향과 몬스터간의 각도를 구한다.
				float theta = playerTr->Forward().Dot(otherTr->Forward());
				Vector3 cross = playerTr->Forward().Cross(otherTr->Forward());
				theta = acos(theta);
				theta *= 180.0f / XM_PI;
				theta *= (cross.y / abs(cross.y));

				// 충돌 각도에 따라 피격 방향(애니메이션) 달라짐
				if (theta > -45.0f && theta <= 45.0f)
				{
					mPlayerAnim->Play(L"a000_100102");
					mHitDirection = -playerTr->Forward();
				}
				else if (theta > 45.0f && theta <= 135.0f)
				{
					mPlayerAnim->Play(L"a000_100100");
					mHitDirection = playerTr->Right();
				}
				else if (theta > 135.0f && theta <= 180.0f || theta > -180.0f && theta <= -135.0f)
				{
					mPlayerAnim->Play(L"a000_100103");
					mHitDirection = playerTr->Forward();
				}
				else if (theta > -135.0f && theta <= -45.0f)
				{
					mPlayerAnim->Play(L"a000_100101");
					mHitDirection = -playerTr->Right();
				}

				if (mTimer[(UINT)eAttackState::HitMove] <= 0.0f)
				{
					mTimer[(UINT)eAttackState::HitMove] = mTimerMax[(UINT)eAttackState::HitMove];
					//mHitDirection = -playerTr->Forward();
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
			mDeathBlowTargets.erase(monster);
	}
	void PlayerAttackScript::DeathBlow(MonsterBase* monster)
	{
		// 인살 가능한 몬스터가 있는 상태일때
		// 몬스터 

		//monster->SetDeathBlow();
		mPlayerAnim->Play(L"a200_510200");
		
		EraseDeathBlowTarget(monster);
		mDeathBlowTarget = nullptr;
		monster->DeathBlow();
	}
}
