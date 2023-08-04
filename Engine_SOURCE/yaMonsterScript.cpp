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
								//Def
							}
						}
						else
						{
							//Monster Hit
						}
					}
					else
					{
						//Monster Hit
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