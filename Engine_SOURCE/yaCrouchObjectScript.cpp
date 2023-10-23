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

	void CrouchObjectScript::OnCollisionEnter(GameObject* _otherObj, const Vector3& _hitPoint)
	{
		
	}

	void CrouchObjectScript::OnCollisionStay(GameObject* _otherObj, const Vector3& _hitPoint)
	{
		Player* player = dynamic_cast<Player*>(_otherObj);

		if (player != nullptr)
		{
			UINT flag = player->GetStateFlag();
			if ((UINT)ePlayerState::Crouch & flag)
			{
				player->SetStealth(true);
			}
		}
	}

	void CrouchObjectScript::OnCollisionExit(GameObject* _otherObj, const Vector3& _hitPoint)
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
