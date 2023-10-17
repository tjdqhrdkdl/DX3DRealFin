#include "yaMonsterScript.h"


namespace ya
{
	void MonsterScript::Initialize()
	{

	}

	void MonsterScript::Update()
	{



	}

	void MonsterScript::FixedUpdate()
	{

	}

	void MonsterScript::OnCollisionEnter(Collider2D* collider)
	{
		MonsterBase* monster = (MonsterBase*)GetOwner();
		if (monster->GetSituation() == eSituation::Death)
			return;		

		//몬스터 작업
		if (dynamic_cast<BoneCollider*>(collider->GetOwner()))
		{

			
			BoneCollider* katana = (BoneCollider*)collider->GetOwner();
			Transform* katanatr = katana->GetComponent<Transform>();
			if (katanatr->GetPosition() == Vector3::Zero)
				return;


			 
			if (dynamic_cast<Player*>(katana->GetBCOwner()))

			{
				
				//Player* player = (Player*)katana->GetBCOwner();
				//if (player->IsStateFlag(ePlayerState::Attack))
				if (katana->GetComponent<Collider2D>()->IsActive())
				{
					if (monster->IsDeathBlow())
					{

						monster->SetSituation(eSituation::Death, true);
						monster->SetDeathBlowKill(true);
						monster->SetDeathBlow(false);
						return;
						int a = 0;

					}

					//몬스터 방어
					if (eSituation::Defense == monster->GetSituation() &&
						monster->IsPlayerFront())
					{

						return;
					}
					//몬스터가 같이 공격
					else if (eSituation::Attack == monster->GetSituation())
					{
						int a = 0;
						//monster->SetSituation(eSituation::AttackHit, true);
						//return;
					}


					Vector3 prevPos = katana->GetPrevPos();
					Vector3 culPos = katana->GetComponent<Transform>()->GetPosition();
					Vector3 Dirpos = culPos - prevPos;
					Dirpos.Normalize();
					if (Dirpos.x > 0)
					{

						monster->SetHitRight(true);
						monster->SetSituation(eSituation::Hit, true);
					}
					else if (Dirpos.x < 0)
					{
						monster->SetHitLeft(true);
						monster->SetSituation(eSituation::Hit, true);
					}

					monster->AddHp(-4.0f);
					if (monster->GetHP() <= 0.0f)
					{
						monster->SetSituation(eSituation::Death, true);
					}


				}
			}


		}


		
	}

	void MonsterScript::OnCollisionStay(Collider2D* collider)
	{
	}

	void MonsterScript::OnCollisionExit(Collider2D* collider)
	{
	}

}