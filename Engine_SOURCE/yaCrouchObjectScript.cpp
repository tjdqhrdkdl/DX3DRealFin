#include "yaCrouchObjectScript.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaInput.h"
#include "yaTime.h"
#include "yaAnimator.h"

#include "yaPlayer.h"

namespace ya
{
	CrouchObjectScript::CrouchObjectScript()
		: Script()
	{
	}

	CrouchObjectScript::~CrouchObjectScript()
	{
	}

	void CrouchObjectScript::Initialize()
	{
	}

	void CrouchObjectScript::Update()
	{
	}

	void CrouchObjectScript::Render()
	{
	}

	void CrouchObjectScript::OnCollisionEnter(Collider2D* collider)
	{
		GameObject* obj = collider->GetOwner();
		Player* player = dynamic_cast<Player*>(obj);

		if (player != nullptr)
		{
			if (player->IsStateFlag(ePlayerState::Crouch))
				player->SetStateFlag(ePlayerState::Stealth, true);
		}
	}

	void CrouchObjectScript::OnCollisionStay(Collider2D* collider)
	{
		GameObject* obj = collider->GetOwner();
		Player* player = dynamic_cast<Player*>(obj);

		if (player != nullptr)
		{
			if (!player->IsStateFlag(ePlayerState::Crouch))
				player->SetStateFlag(ePlayerState::Stealth, false);
		}
	}

	void CrouchObjectScript::OnCollisionExit(Collider2D* collider)
	{
		GameObject* obj = collider->GetOwner();
		Player* player = dynamic_cast<Player*>(obj);

		player->SetStateFlag(ePlayerState::Stealth, false);
	}

	void CrouchObjectScript::Start()
	{
	}

	void CrouchObjectScript::Action()
	{
	}

	void CrouchObjectScript::End()
	{
	}

}
