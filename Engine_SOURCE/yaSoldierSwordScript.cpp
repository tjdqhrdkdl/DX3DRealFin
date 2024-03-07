#include "yaSoldierSwordScript.h"
#include "yaPlayer.h"
#include "yaAshinaSoldier.h"
#include "yaParticleSystem.h"
namespace ya
{
	SoldierSwordScript::SoldierSwordScript()
	{
	}
	SoldierSwordScript::~SoldierSwordScript()
	{
	}
	void SoldierSwordScript::Initialize()
	{
	}
	void SoldierSwordScript::OnCollisionEnter(Collider2D* collider)
	{
		GameObject* colObj = collider->GetOwner();
		eLayerType layer = colObj->GetLayerType();
		if (layer == eLayerType::Player)
		{
			Player* pl = dynamic_cast<Player*>(colObj);
			GameObject* particleObj = pl->GetParticleObject();
			ParticleSystem* particleSys = particleObj->GetRenderer<ParticleSystem>();
			particleObj->GetComponent<Transform>()->SetPosition(GetOwner()->GetComponent<Transform>()->GetWorldPositioin());
			particleSys->ParticleOn();
			particleSys->SetParticleNum(100);
			if (mbBlock)
			{
				if (pl->IsStateFlag(ePlayerState::Block))
				{

					Transform* colTr = GetOwner()->GetComponent<Transform>();
					Transform* plTr = colObj->GetComponent<Transform>();
					Vector3 colPos = colTr->GetWorldPositioin();
					Vector3 plPos = plTr->GetWorldPositioin();
					Vector3 dir = colPos - plPos;
					dir.Normalize();

					Quaternion rot = Quaternion::FromToRotation(plTr->Forward(), dir);
					float theta = rot.ToEuler().y;
					theta *= 180.f / XM_PI;
					if (theta <90 && theta > -90)
					{

						GameObject* bigOwner = dynamic_cast<BoneCollider*>(GetOwner())->GetBoneColliderOwner();
						AshinaSoldier* soldier = dynamic_cast<AshinaSoldier*>(bigOwner);
						//패링당함
						if (pl->GetBlockTime() < 0.2f)
						{
							if (mbAttackLeft)
								soldier->SetAnimationName(L"ParriedLeft");
							else
								soldier->SetAnimationName(L"ParriedRight");

							soldier->ParryEffectOn();
							//체간 게이지 영향
							soldier->SetPosture(soldier->GetPosture() + 50);
						}
						//그냥 막힘
						else
						{
							if (mbAttackLeft)
								soldier->SetAnimationName(L"ParriedLeft");
							else
								soldier->SetAnimationName(L"ParriedRight");
						}
						soldier->AddMonsterState(MonsterBase::eMonsterState::MonsterState_AttackBlocked);
						soldier->SetAnimationChangeTime(0.05f);

						// 죽음 인살 체간 방어 
						// 죽음은 두번의 인살을 당할 경우 이루어진다.
						// 인살은 체간 게이지가 모두 닳았을때, 그리고 hp가 소진되었을때. 그로기 상태에 빠지며 이때 인살이 가능해진다.
						// 그로기 상태에서 인살을 하지 않으면, 체간 게이지 30% 정도 회복된다. 
						// 체력이 다닳으면 한대만 맞아도 그로기 상태에 빠진다.




						// 패링 

					}


				}
			}
		}
	}
	void SoldierSwordScript::OnCollisionStay(Collider2D* collider)
	{
	}
	void SoldierSwordScript::OnCollisionExit(Collider2D* collider)
	{
	}
}