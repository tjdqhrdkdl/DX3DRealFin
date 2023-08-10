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
		//Player collider
		if (dynamic_cast<Player*>(collider->GetOwner()))
		{
			Player* player = (Player*)collider->GetOwner();
			if (player->IsAttack())
			{
				//Is Spearman
				if (dynamic_cast<Spearman*>(GetOwner()))
				{
					Spearman* spearman = (Spearman*)GetOwner();
					if (spearman->GetSituation() == eSituation::Defense)
					{
						if (spearman->IsPlayerFront())
						{
							if (spearman->IsParrying())
							{
								//Parrying
								player->SetWalk(true);
							}
							else
							{
								//Def 성공 체간 게이지만 약간 올려주면 됨.
								spearman->SetDeathBlowCount(3.0f);
								spearman->SetDeathBlowonoff(false);
							}
						}
						else
						{
							float test = spearman->GetHP();
							spearman->SetHp(-4.0f);
						}
					}
					else
					{
						float test = spearman->GetHP();
						spearman->SetHp(-4.0f);
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