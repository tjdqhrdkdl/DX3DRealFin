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

namespace ya
{
	PlayerAttackScript::PlayerAttackScript()
		: Script()
		, mAttackState(eAttackState::None)
		, mTimer{0.0f}
		, mTimerMax{ 0.0f,  0.8f, 0.8f, 0.8f, 0.8f, 0.8f,  0.5f, 0.5f, 0.5f,  0.8f, 0.8f, 0.8f, 0.5f}
		, mbKeyInput(false)
	{
	}

	PlayerAttackScript::~PlayerAttackScript()
	{
	}

	void PlayerAttackScript::Initialize()
	{
		//Transform* tr = GetOwner()->GetComponent<Transform>();

		for (size_t i = 0; i < (UINT)eAttackState::End; i++)
		{
			mTimer[i] = mTimerMax[i];
		}

		//PlayerMeshScript* meshScript = GetOwner()->GetScript<PlayerMeshScript>();
		//std::shared_ptr<MeshData> weaponMeshData = meshScript->FindMeshData(L"Arm");
		//weaponMeshData->GetMeshObject()->AddComponent<PlayerProjectileScript>();
		//if (weaponMeshData != nullptr)
		//{
		//	BoneCollider* weaponCollider = object::Instantiate<BoneCollider>(eLayerType::PlayerProjectile);
		//	weaponCollider->SetMeshAndBone(weaponMeshData, L"R_Weapon");
		//	weaponCollider->SetScale(Vector3(1, 0.2, 0.2));

		//	//weaponCollider->SetAnimOffSet(L"SwingSword1", Vector3(1, 0.5, 1));
		//	weaponCollider->SetColliderActiveFrame(L"a000_000000", 0, 100);
		//	/*weaponCollider->SetColliderActiveFrame(L"a050_300100", 0, 100);
		//	weaponCollider->SetColliderActiveFrame(L"a050_305101", 0, 100);
		//	weaponCollider->SetColliderActiveFrame(L"a050_300020", 0, 100);
		//	weaponCollider->SetColliderActiveFrame(L"a050_300030", 0, 100);
		//	weaponCollider->SetColliderActiveFrame(L"a050_300040", 0, 100);*/

		//	//weaponCollider->SetColliderActiveFrame(L"a050_308010", 0, 100);
		//	//weaponCollider->SetColliderActiveFrame(L"a050_308000", 0, 100);
		//	//weaponCollider->SetColliderActiveFrame(L"a050_308050", 0, 100);
		//	//weaponCollider->SetColliderActiveFrame(L"a050_308060", 0, 100);

		//	////weaponCollider->SetColliderActiveFrame(L"a050_301050", 0, 100);
		//	//weaponCollider->SetColliderActiveFrame(L"a050_314000", 0, 100);
		//	//weaponCollider->SetColliderActiveFrame(L"a050_002000", 0, 100);
		//}
	}

	void PlayerAttackScript::Update()
	{
		Player* player = dynamic_cast<Player*>(GetOwner());
		Transform* playerTr = player->GetComponent<Transform>();
		PlayerMeshScript* playerAnim = player->GetScript<PlayerMeshScript>();
		PlayerActionScript* playerAction = player->GetScript<PlayerActionScript>();

		if (mTimer[(UINT)eAttackState::Move] > 0.0f)
		{
			mTimer[(UINT)eAttackState::Move] -= Time::DeltaTime();
			playerAction->Move(playerTr->Forward(), 500.0f);
		}

		switch (mAttackState)
		{
		case ya::PlayerAttackScript::eAttackState::None:
		{
			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				player->SetStateFlag(ePlayerState::Attack, true);

				if (player->IsStateFlag(ePlayerState::Jump))
				{
					mAttackState = eAttackState::JumpAttack1;
					playerAnim->Play(L"a050_308010");
					
				}
				else if (player->IsStateFlag(ePlayerState::Crouch))
				{
					mAttackState = eAttackState::CrouchAttack1;
					player->SetStateFlag(ePlayerState::Crouch, false);
					playerAnim->Play(L"a050_301050");
					mTimer[(UINT)eAttackState::Move] = mTimerMax[(UINT)eAttackState::Move];
				}
				else if (player->IsStateFlag(ePlayerState::Hang))
				{
					mAttackState = eAttackState::HangAttack1;
					playerAnim->Play(L"a050_314000");
				}
				else
				{
					mAttackState = eAttackState::Attack1;
					playerAnim->Play(L"a050_300100");
					mTimer[(UINT)eAttackState::Move] = mTimerMax[(UINT)eAttackState::Move];
				}
			}

			if (Input::GetKeyDown(eKeyCode::RBTN))
			{
				player->SetStateFlag(ePlayerState::Block, true);
				mAttackState = eAttackState::Block;
				playerAnim->Play(L"a050_002000");
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
					playerAnim->Play(L"a050_305101");
				}
				else
				{
					mAttackState = eAttackState::None;
					playerAnim->Play(L"a000_000000");
					player->SetStateFlag(ePlayerState::Attack, false);
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
					playerAnim->Play(L"a050_300020");
				}
				else
				{
					mAttackState = eAttackState::None;
					playerAnim->Play(L"a000_000000");
					player->SetStateFlag(ePlayerState::Attack, false);
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
					playerAnim->Play(L"a050_300030");
				}
				else
				{
					mAttackState = eAttackState::None;
					playerAnim->Play(L"a000_000000");
					player->SetStateFlag(ePlayerState::Attack, false);
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
					playerAnim->Play(L"a050_300040");
				}
				else
				{
					mAttackState = eAttackState::None;
					playerAnim->Play(L"a000_000000");
					player->SetStateFlag(ePlayerState::Attack, false);
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
					playerAnim->Play(L"a050_300100");
				}
				else
				{
					mAttackState = eAttackState::None;
					playerAnim->Play(L"a000_000000");
					player->SetStateFlag(ePlayerState::Attack, false);
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
			if (Input::GetKeyUp(eKeyCode::RBTN))
			{
				mAttackState = eAttackState::None;
				player->SetStateFlag(ePlayerState::Block, false);

				if (player->IsStateFlag(ePlayerState::Walk))
				{

				}
				else
				{
					playerAnim->Play(L"a000_000000");
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

					if (player->IsStateFlag(ePlayerState::Jump))
					{
						playerAnim->Play(L"a050_308000");
					}
					else
					{
						playerAnim->Play(L"a050_308050");
					}
				}
				else
				{
					mAttackState = eAttackState::None;
					if (player->IsStateFlag(ePlayerState::Jump))
					{
					}
					else
					{
						playerAnim->Play(L"a000_000000");
					}
					player->SetStateFlag(ePlayerState::Attack, false);
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
					if (player->IsStateFlag(ePlayerState::Jump))
					{
						playerAnim->Play(L"a050_308060");
					}
					else
					{
						playerAnim->Play(L"a050_308060");
					}
				}
				else
				{
					mAttackState = eAttackState::None;
					if (player->IsStateFlag(ePlayerState::Jump))
					{
					}
					else
					{
						playerAnim->Play(L"a000_000000");
					}
					player->SetStateFlag(ePlayerState::Attack, false);
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
					if (player->IsStateFlag(ePlayerState::Jump))
					{
						mAttackState = eAttackState::JumpAttack1;
						playerAnim->Play(L"a050_308010");
					}
					else
					{
						mAttackState = eAttackState::Attack1;
						playerAnim->Play(L"a050_300100");
					}
				}
				else
				{
					mAttackState = eAttackState::None;
					if (player->IsStateFlag(ePlayerState::Jump))
					{
						playerAnim->Play(L"a000_201030");
					}
					else
					{
						playerAnim->Play(L"a000_000000");
					}
					player->SetStateFlag(ePlayerState::Attack, false);
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
					playerAnim->Play(L"a050_305101");
				}
				else
				{
					mAttackState = eAttackState::None;
					playerAnim->Play(L"a000_000000");
					player->SetStateFlag(ePlayerState::Attack, false);
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
				player->SetStateFlag(ePlayerState::Attack, false);
				mTimer[(UINT)eAttackState::HangAttack1] = mTimerMax[(UINT)eAttackState::HangAttack1];
			}


			if (mTimer[(UINT)eAttackState::HangAttack1] <= 0.0f)
			{
				if (mbKeyInput)
				{
					mAttackState = eAttackState::HangAttack1;
					playerAnim->Play(L"a050_314000");
				}
				else
				{
					mAttackState = eAttackState::None;
					playerAnim->Play(L"a000_020000");
					player->SetStateFlag(ePlayerState::Attack, false);
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
