#include "yaPlayerScript.h"
#include "yaPlayer.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaInput.h"
#include "yaTime.h"
#include "yaAnimator.h"
#include "yaObject.h";
#include "yaMeshRenderer.h"
#include "yaResources.h"
#include "yaRigidbody.h"
#include "yaActionScript.h"
#include "yaGrappleHookScript.h"

#include "yaSceneManager.h"
#include "yaScene.h"

#include "yaPlayerMeshScript.h"
#include "yaState.h"


namespace ya
{
	PlayerScript::PlayerScript()
		: Script()
	{
	}

	PlayerScript::~PlayerScript()
	{
	}

	void PlayerScript::Initialize()
	{
	}

	void PlayerScript::Update()
	{
	}

	void PlayerScript::Render()
	{
	}

	void PlayerScript::OnCollisionEnter(Collider2D* collider)
	{
		

	}

	void PlayerScript::OnCollisionStay(Collider2D* collider)
	{
	}

	void PlayerScript::OnCollisionExit(Collider2D* collider)
	{
	}

}
