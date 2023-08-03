#include "yaMonsterScript.h"


namespace ya
{
	void MonsterScript::Initalize()
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
		//충돌시작 - 플레이어 일 경우
		if (dynamic_cast<Player*>(collider->GetOwner()))
		{
			Player* player = (Player*)collider->GetOwner();
			if (player->IsAttack())
			{
				//피 연산자가 스피어맨 인 경우
				if (dynamic_cast<Spearman*>(GetOwner()))
				{
					Spearman* spearman = (Spearman*)GetOwner();
					if (spearman->GetSituation() == eSituation::Defense)
					{
						if (spearman->IsPlayerFront())
						{
							if (spearman->IsParrying())
							{
								//튕기기 성공
								player->SetWalk(true);
							}
							else
							{
								//그냥 막기만 성공


							}
						}
						else
						{
							//뒤에서 막기상태라 막기 실패
							player->SetWalk(true);

						}
					}
					else
					{
						player->SetWalk(true);
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