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
		//�浹���� - �÷��̾� �� ���
		if (dynamic_cast<Player*>(collider->GetOwner()))
		{
			Player* player = (Player*)collider->GetOwner();
			if (player->IsAttack())
			{
				//�� �����ڰ� ���Ǿ�� �� ���
				if (dynamic_cast<Spearman*>(GetOwner()))
				{
					Spearman* spearman = (Spearman*)GetOwner();
					if (spearman->GetSituation() == eSituation::Defense)
					{
						if (spearman->IsPlayerFront())
						{
							if (spearman->IsParrying())
							{
								//ƨ��� ����
								player->SetWalk(true);
							}
							else
							{
								//�׳� ���⸸ ����


							}
						}
						else
						{
							//�ڿ��� ������¶� ���� ����
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