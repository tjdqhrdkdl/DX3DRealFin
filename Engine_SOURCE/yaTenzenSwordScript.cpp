#include "yaTenzenSwordScript.h"
#include "yaPlayer.h"
#include "yaTenzen.h"
namespace ya
{
	TenzenSwordScript::TenzenSwordScript()
	{
	}
	TenzenSwordScript::~TenzenSwordScript()
	{
	}
	void TenzenSwordScript::Initialize()
	{
	}
	void TenzenSwordScript::OnCollisionEnter(Collider2D* collider)
	{
		GameObject* colObj = collider->GetOwner();
		eLayerType layer = colObj->GetLayerType();
		if (layer == eLayerType::Player)
		{
			if (mbBlock)
			{
				Player* pl = dynamic_cast<Player*>(colObj);
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
						Tenzen* tenzen = dynamic_cast<Tenzen*>(bigOwner);
						if (mbAttackLeft)
							tenzen->SetAnimationName(L"BlockedLeft");
						else
							tenzen->SetAnimationName(L"BlockedRight");
						tenzen->AddTenzenState(Tenzen::eTenzenState::TenzenState_AttackBlocked);
						tenzen->SetAnimationChangeTime(0.05f);
					}


				}
			}
		}
	}
	void TenzenSwordScript::OnCollisionStay(Collider2D* collider)
	{
	}
	void TenzenSwordScript::OnCollisionExit(Collider2D* collider)
	{
	}
}