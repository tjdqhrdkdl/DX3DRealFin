#include "GroundCheckScript.h"
#include "yaGameObject.h"

namespace ya
{
	GroundCheckScript::GroundCheckScript()
		: mGrounded(false)
	{
	}
	GroundCheckScript::~GroundCheckScript()
	{
	}
	void GroundCheckScript::OnCollisionEnter(GameObject* _otherObj, const Vector3& _hitPoint)
	{
		if (eLayerType::Ground == _otherObj->GetLayerType())
		{
			++mGrounded;
		}
	}
	void GroundCheckScript::OnCollisionExit(GameObject* _otherObj, const Vector3& _hitPoint)
	{
		if (eLayerType::Ground == _otherObj->GetLayerType())
		{
			--mGrounded;
			if (mGrounded < 0)
			{
				mGrounded = 0;
			}
		}
	}
}