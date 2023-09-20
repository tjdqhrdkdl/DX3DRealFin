#include "yaPlayerProjectileScript.h"
#include "yaPlayer.h"
#include "yaPlayerMeshScript.h"

#include "yaBoneCollider.h"

namespace ya
{
	PlayerProjectileScript::PlayerProjectileScript()
		: Script()
		, mPlayer(nullptr)
		, mPlayerAnim(nullptr)
		, mbBlock(false)
	{
	}

	PlayerProjectileScript::~PlayerProjectileScript()
	{
	}

	void PlayerProjectileScript::Initialize()
	{
	}

	void PlayerProjectileScript::Update()
	{
	}

	void PlayerProjectileScript::FixedUpdate()
	{
	}

	void PlayerProjectileScript::Render()
	{
	}
	void PlayerProjectileScript::OnCollisionEnter(Collider2D* collider)
	{
		if (!mPlayer->IsStateFlag(ePlayerState::Block))
			return;

		// 패링
		GameObject* obj = collider->GetOwner();

		BoneCollider* c = dynamic_cast<BoneCollider*>(obj);
		if (c != nullptr)
		{
			if (mPlayer->IsStateFlag(ePlayerState::Block))
			{
				mPlayer->SetStateFlag(ePlayerState::Parrying, true);
				mPlayer->SetStateFlag(ePlayerState::Block, false);

				mPlayer->GetState()->AddPosture(-10);

				Transform* playerTr = mPlayer->GetComponent<Transform>();
				Vector3 playerPos = playerTr->GetPosition();
				Vector3 playerDir = playerTr->Forward();

				Vector3 colliderPos = collider->GetPosition();

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
		}
	}
	void PlayerProjectileScript::OnCollisionStay(Collider2D* collider)
	{
	}
	void PlayerProjectileScript::OnCollisionExit(Collider2D* collider)
	{
	}
	void PlayerProjectileScript::OnTriggerEnter(Collider2D* collider)
	{
	}
	void PlayerProjectileScript::OnTriggerStay(Collider2D* collider)
	{
	}
	void PlayerProjectileScript::OnTriggerExit(Collider2D* collider)
	{
	}
	void PlayerProjectileScript::SetPlayer(Player* player)
	{
		mPlayer = player;
		mPlayerAnim = player->GetScript<PlayerMeshScript>();
	}
}
