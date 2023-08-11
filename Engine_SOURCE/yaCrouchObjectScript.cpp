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
		
	}

	void CrouchObjectScript::OnCollisionStay(Collider2D* collider)
	{
		GameObject* obj = collider->GetOwner();
		Player* player = dynamic_cast<Player*>(obj);

		if (player != nullptr)
		{
			UINT flag = player->GetStateFlag();
			if ((UINT)ePlayerState::Crouch & flag)
			{
				player->SetStealth(true);
			}
		}
	}

	void CrouchObjectScript::OnCollisionExit(Collider2D* collider)
	{
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
