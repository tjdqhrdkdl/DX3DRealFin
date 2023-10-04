#include "yaTenzenSwordScript.h"
#include "yaPlayer.h"
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
			dynamic_cast<Player*>(colObj);

		}
	}
	void TenzenSwordScript::OnCollisionStay(Collider2D* collider)
	{
	}
	void TenzenSwordScript::OnCollisionExit(Collider2D* collider)
	{
	}
}