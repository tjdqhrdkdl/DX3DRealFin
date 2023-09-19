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
		// 몬스터 공격
		int a = 0;

		//Player* player = dynamic_cast<Player*>(GetOwner());


		// 패링
		GameObject* obj = collider->GetOwner();

		BoneCollider* c = dynamic_cast<BoneCollider*>(obj);
		if (c != nullptr)
		{
			//if (player->IsStateFlag(ePlayerState::Block))
			if(mbBlock)
			{
				mPlayer->SetStateFlag(ePlayerState::Parrying, true);
				mPlayer->SetStateFlag(ePlayerState::Block, false);

				mPlayer->GetState()->AddPosture(10);

				// 튕겨나는 애니메이션
				//mPlayerAnim->Play(L"");

				// 패링 발동
				// 패링상태로 변경, 체간 증가

				// 패링 애니메이션

				// 패링 이펙트 

				mbBlock = false;
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
}
