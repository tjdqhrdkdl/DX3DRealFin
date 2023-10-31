#include "yaPlayerProjectileScript.h"
#include "yaPlayer.h"
#include "yaPlayerMeshScript.h"
#include "yaPlayerAttackScript.h"

#include "yaBoneCollider.h"
#include "yaMonsterBase.h"
#include "yaParticleSystem.h"

#include "yaResources.h"
#include "yaAudioClip.h"

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
		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\enemy_damage1.wav");
			Resources::Insert<AudioClip>(L"enemy_damage1", clip);
		}
		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\enemy_damage2.wav");
			Resources::Insert<AudioClip>(L"enemy_damage2", clip);
		}
		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\enemy_damage3.wav");
			Resources::Insert<AudioClip>(L"enemy_damage3", clip);
		}
		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\enemy_damage4.wav");
			Resources::Insert<AudioClip>(L"enemy_damage4", clip);
		}
		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\enemy_damage5.wav");
			Resources::Insert<AudioClip>(L"enemy_damage5", clip);
		}
		
		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\sword-x-sword1.wav");
			Resources::Insert<AudioClip>(L"sword-x-sword1", clip);
		}
		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\sword-x-sword2.wav");
			Resources::Insert<AudioClip>(L"sword-x-sword2", clip);
		}
		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\sword-x-sword3.wav");
			Resources::Insert<AudioClip>(L"sword-x-sword3", clip);
		}
		
		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\c000006601.wav");
			Resources::Insert<AudioClip>(L"c000006601_1", clip);
		}
		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\c000006601b.wav");
			Resources::Insert<AudioClip>(L"c000006601_2", clip);
		}
		{
			std::shared_ptr<AudioClip> clip = std::make_shared<AudioClip>();
			clip->Load(L"..\\Resources\\Sound\\main\\c000006601c.wav");
			Resources::Insert<AudioClip>(L"c000006601_3", clip);
		}

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

		// 몬스터가 죽은 경우 충돌 검사를 하지 않는다.
		if (monster->IsMonsterState(MonsterBase::MonsterState_Dead))
			return;

		// 플레이어와 몬스터의 forward 비교. 몬스터가 플레이어쪽으로 90도 이내 일때만 방어 유효
		Vector3 playerPos = playerTr->GetPosition();
		Vector3 monsterPos = monsterTr->GetPosition();
		Quaternion quater = Quaternion::FromToRotation(monsterTr->Forward(), playerPos-monsterPos);
		Vector3 quaterToEuler = quater.ToEuler();
		Vector3 theta = quaterToEuler * 180.0f / XM_PI;

		PlayerAttackScript::eAttackState attackState = attack->GetAttackState();

		
			GameObject* particleObj = mPlayer->GetParticleObject();
			ParticleSystem* particleSys = particleObj->GetComponent<ParticleSystem>();
			Vector3 effectPos = GetOwner()->GetComponent<Transform>()->GetWorldPositioin();
			particleObj->GetComponent<Transform>()->SetPosition(effectPos);
			particleSys->ParticleOn();
			particleSys->SetParticleNum(100);



			// 몬스터 패링
			if (monster->IsMonsterState(MonsterBase::MonsterState_GuardSuccess) && abs(theta.y) <= 45.0f)
			{
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

				//Resources::Find<AudioClip>(L"sword-x-sword" + std::to_wstring(RandomNumber(1, 3)))->Play();
				Resources::Find<AudioClip>(L"c000006601_" + std::to_wstring(RandomNumber(1, 3)))->Play();
				return;
			}
			else
			{
				if (attackState == PlayerAttackScript::eAttackState::Attack1)
				{
					Resources::Find<AudioClip>(L"enemy_damage1")->Play();
				}
				else if (attackState == PlayerAttackScript::eAttackState::Attack2)
				{
					Resources::Find<AudioClip>(L"enemy_damage2")->Play();
				}
				else if (attackState == PlayerAttackScript::eAttackState::Attack3)
				{
					Resources::Find<AudioClip>(L"enemy_damage3")->Play();
				}
				else if (attackState == PlayerAttackScript::eAttackState::Attack4)
				{
					Resources::Find<AudioClip>(L"enemy_damage4")->Play();
				}
				else if (attackState == PlayerAttackScript::eAttackState::Attack5)
				{
					Resources::Find<AudioClip>(L"enemy_damage5")->Play();
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
