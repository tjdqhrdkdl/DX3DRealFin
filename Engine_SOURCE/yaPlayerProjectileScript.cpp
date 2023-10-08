#include "yaPlayerProjectileScript.h"
#include "yaPlayer.h"
#include "yaPlayerMeshScript.h"
#include "yaPlayerAttackScript.h"

#include "yaBoneCollider.h"
#include "yaMonsterBase.h"

namespace ya
{
	PlayerProjectileScript::PlayerProjectileScript()
		: Script()
		, mPlayer(nullptr)
		, mPlayerAnim(nullptr)
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
		Transform* playerTr = mPlayer->GetComponent<Transform>();
		PlayerAttackScript* attack = mPlayer->GetScript<PlayerAttackScript>();

		if (attack == nullptr)
			return;

		// 플레이어가 공격중이 아닐때는 충돌 검사를 하지 않는다.
		if (!mPlayer->IsStateFlag(ePlayerState::Attack))
			return;

		MonsterBase* monster = dynamic_cast<MonsterBase*>(collider->GetOwner());

		if (monster == nullptr)
			return;

		Transform* monsterTr = monster->GetComponent<Transform>();
		eSituation monsterState = monster->GetSituation();

		// 몬스터가 죽은 경우 충돌 검사를 하지 않는다.
		if (monsterState == eSituation::Death)
			return;

		// 플레이어와 몬스터의 forward 비교. 몬스터가 플레이어쪽으로 90도 이내 일때만 방어 유효
		Vector3 playerPos = playerTr->GetPosition();
		Vector3 monsterPos = monsterTr->GetPosition();
		Quaternion quater = Quaternion::FromToRotation(monsterTr->Forward(), playerPos-monsterPos);
		Vector3 quaterToEuler = quater.ToEuler();
		Vector3 theta = quaterToEuler * 180.0f / XM_PI;

		// 몬스터 패링
		if (monsterState == eSituation::Defense && abs(theta.y) <= 45.0f)
		{
			PlayerAttackScript::eAttackState attackState = attack->GetAttackState();
			if (attackState == PlayerAttackScript::eAttackState::Attack1)
			{
				mPlayerAnim->Play(L"a050_130100");
			}
			else if (attackState == PlayerAttackScript::eAttackState::Attack2)
			{
				mPlayerAnim->Play(L"a050_150001");
			}
			else if (attackState == PlayerAttackScript::eAttackState::Attack3)
			{
				//mPlayerAnim->Play(L"a050_130201");
			}
			else if (attackState == PlayerAttackScript::eAttackState::Attack4)
			{
				mPlayerAnim->Play(L"a050_130201");
			}
			else if (attackState == PlayerAttackScript::eAttackState::Attack5)
			{
				mPlayerAnim->Play(L"a050_130200");
			}

			return;
		}

		// 인살 가능 상태일때
		if (0)
		{
			mPlayerAnim->Play(L"a050_130200");
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
