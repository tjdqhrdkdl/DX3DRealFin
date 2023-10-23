#include "yaMonsterUIScript.h"
#include "yaMonsterUI.h"


void ya::MonsterUIScript::OnCollisionEnter(Collider2D* collider)
{
	GameObject* colObj = collider->GetOwner();
	eLayerType layer = colObj->GetLayerType();
	if (layer == eLayerType::PlayerProjectile)
	{
		if (mMonsterUI)
		{
			mMonsterUI->UIOn();
		}
	}
}
