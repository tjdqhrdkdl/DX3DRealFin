#include "yaPlayerAttackScript.h"

#include "yaObject.h"
#include "yaInput.h"
#include "yaTime.h"

#include "yaPlayer.h"

#include "yaTransform.h"
#include "yaRigidbody.h"

namespace ya
{
	PlayerAttackScript::PlayerAttackScript()
		: ActionScript()
		, mAttackState(eAttackState::None)
		, mTimer{0.0f}
		, mTimerMax{ 0.4f, 0.4f, 0.4f, 0.4f, 0.4f,  0.4f, 0.4f, 0.4f, 0.4f, 0.4f }
	{
	}

	PlayerAttackScript::~PlayerAttackScript()
	{
	}

	void PlayerAttackScript::Initialize()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

		/*mAttackProjectile = object::Instantiate<GameObject>(eLayerType::PlayerProjectile, tr);
		mAttackProjectile->SetName(L"player attack projectile");
		Transform* attackTr = mAttackProjectile->GetComponent<Transform>();
		attackTr->SetScale(Vector3(10.0f, 2.0f, 2.0f));
		Collider2D* attackCollider = mAttackProjectile->AddComponent<Collider2D>();
		attackCollider->SetType(eColliderType::Box);
		attackCollider->SetSize(Vector3(1.0, 1.0f, 1.0f));
		attackCollider->Active(false);
		mAttackProjectile->AddComponent<PlayerProjectileScript>();*/
	}

	void PlayerAttackScript::Update()
	{
		Player* player = dynamic_cast<Player*>(GetOwner());

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
				}
				else if (player->IsStateFlag(ePlayerState::Crouch))
				{
					mAttackState = eAttackState::CrouchAttack1;
				}
				else if (player->IsStateFlag(ePlayerState::Hang))
				{
					mAttackState = eAttackState::HangAttack1;
				}
				else
				{
					mAttackState = eAttackState::Attack1;
				}
			}

			if (Input::GetKeyDown(eKeyCode::RBTN))
			{
				player->SetStateFlag(ePlayerState::Block, true);
				mAttackState = eAttackState::Block;
			}
		}
			break;
		case ya::PlayerAttackScript::eAttackState::Attack1:
		{
			 if (mTimer[(UINT)eAttackState::Attack1] <= 0.0f)
			{
				mAttackState = eAttackState::None;
				player->SetStateFlag(ePlayerState::Attack, false);
				mTimer[(UINT)eAttackState::Attack1] = mTimerMax[(UINT)eAttackState::Attack1];
			}

			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				mAttackState = eAttackState::Attack2;
			}
		}
			break;
		case ya::PlayerAttackScript::eAttackState::Attack2:
		{
			if (mTimer[(UINT)eAttackState::Attack2] <= 0.0f)
			{
				mAttackState = eAttackState::None;
				player->SetStateFlag(ePlayerState::Attack, false);
				mTimer[(UINT)eAttackState::Attack2] = mTimerMax[(UINT)eAttackState::Attack2];
			}

			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				mAttackState = eAttackState::Attack3;
			}
		}
			break;
		case ya::PlayerAttackScript::eAttackState::Attack3:
		{
			if (mTimer[(UINT)eAttackState::Attack3] <= 0.0f)
			{
				mAttackState = eAttackState::None;
				player->SetStateFlag(ePlayerState::Attack, false);
				mTimer[(UINT)eAttackState::Attack3] = mTimerMax[(UINT)eAttackState::Attack3];
			}

			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				mAttackState = eAttackState::Attack4;
			}
		}
			break;
		case ya::PlayerAttackScript::eAttackState::Attack4:
		{
			if (mTimer[(UINT)eAttackState::Attack4] <= 0.0f)
			{
				mAttackState = eAttackState::None;
				player->SetStateFlag(ePlayerState::Attack, false);
				mTimer[(UINT)eAttackState::Attack4] = mTimerMax[(UINT)eAttackState::Attack4];
			}

			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				mAttackState = eAttackState::Attack5;
			}
		}
			break;
		case ya::PlayerAttackScript::eAttackState::Attack5:
		{
			if (mTimer[(UINT)eAttackState::Attack5] <= 0.0f)
			{
				mAttackState = eAttackState::None;
				player->SetStateFlag(ePlayerState::Attack, false);
				mTimer[(UINT)eAttackState::Attack5] = mTimerMax[(UINT)eAttackState::Attack5];
			}
		}
			break;
		case ya::PlayerAttackScript::eAttackState::Block:
		{
			if (Input::GetKeyUp(eKeyCode::RBTN))
			{
				mAttackState = eAttackState::None;
				player->SetStateFlag(ePlayerState::Block, false);
			}
		}
			break;
		case ya::PlayerAttackScript::eAttackState::JumpAttack1:
		{
			if (mTimer[(UINT)eAttackState::JumpAttack1] <= 0.0f)
			{
				mAttackState = eAttackState::None;
				player->SetStateFlag(ePlayerState::Attack, false);
				mTimer[(UINT)eAttackState::JumpAttack1] = mTimerMax[(UINT)eAttackState::JumpAttack1];
			}

			if (!player->IsStateFlag(ePlayerState::Jump))
			{
				mAttackState = eAttackState::None;
				player->SetStateFlag(ePlayerState::Attack, false);
			}

			if (Input::GetKeyDown(eKeyCode::RBTN))
			{
				if (player->IsStateFlag(ePlayerState::Jump))
				{
					mAttackState = eAttackState::JumpAttack2;
				}
				else
				{
					mAttackState = eAttackState::Attack1;
				}
			}
		}
		break;
		case ya::PlayerAttackScript::eAttackState::JumpAttack2:
		{
			if (mTimer[(UINT)eAttackState::JumpAttack2] <= 0.0f)
			{
				mAttackState = eAttackState::None;
				player->SetStateFlag(ePlayerState::Attack, false);
				mTimer[(UINT)eAttackState::JumpAttack2] = mTimerMax[(UINT)eAttackState::JumpAttack2];
			}

			if (!player->IsStateFlag(ePlayerState::Jump))
			{
				mAttackState = eAttackState::None;
				player->SetStateFlag(ePlayerState::Attack, false);
			}

			if (Input::GetKeyDown(eKeyCode::RBTN))
			{
				if (player->IsStateFlag(ePlayerState::Jump))
				{
					mAttackState = eAttackState::JumpAttack2;
				}
				else
				{
					mAttackState = eAttackState::Attack1;
				}
			}
		}
		break;
		case ya::PlayerAttackScript::eAttackState::CrouchAttack1:
		{
			if (mTimer[(UINT)eAttackState::CrouchAttack1] <= 0.0f)
			{
				mAttackState = eAttackState::None;
				player->SetStateFlag(ePlayerState::Attack, false);
				mTimer[(UINT)eAttackState::CrouchAttack1] = mTimerMax[(UINT)eAttackState::CrouchAttack1];
			}

			if (Input::GetKeyDown(eKeyCode::RBTN))
			{
				mAttackState = eAttackState::Attack2;
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
		}
		break;
		default:
			break;
		}

		/*Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
		Vector3 rot = tr->GetRotation();
		Vector3 dir = tr->Forward();
		dir.Normalize();

		Transform* attackTr = mAttackProjectile->GetComponent<Transform>();
		Vector3 attackPos = attackTr->GetPosition();
		attackTr->SetPosition(pos + (dir * 5.0f));
		attackTr->SetRotation(rot);*/
	}

	void PlayerAttackScript::FixedUpdate()
	{
	}


	void PlayerAttackScript::Render()
	{
	}
}
