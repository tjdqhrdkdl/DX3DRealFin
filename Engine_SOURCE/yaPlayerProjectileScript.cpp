#include "yaPlayerProjectileScript.h"
#include "yaPlayer.h"
#include "yaPlayerMeshScript.h"

#include "yaBoneCollider.h"

namespace ya
{
	PlayerProjectileScript::PlayerProjectileScript()
		: Script()
		, mPlayer(nullptr)
		, mPlayerAnim(nullptr)
	{
	}

	PlayerProjectileScript::~PlayerProjectileScript()
	{
	}

	void PlayerProjectileScript::Initialize()
	{
	}

	void PlayerProjectileScript::Update()
	{
	}

	void PlayerProjectileScript::FixedUpdate()
	{
	}

	void PlayerProjectileScript::Render()
	{
	}
	void PlayerProjectileScript::OnCollisionEnter(Collider2D* collider)
	{
	}
	void PlayerProjectileScript::OnCollisionStay(Collider2D* collider)
	{
	}
	void PlayerProjectileScript::OnCollisionExit(Collider2D* collider)
	{
	}
	void PlayerProjectileScript::OnTriggerEnter(Collider2D* collider)
	{
	}
	void PlayerProjectileScript::OnTriggerStay(Collider2D* collider)
	{
	}
	void PlayerProjectileScript::OnTriggerExit(Collider2D* collider)
	{
	}
	void PlayerProjectileScript::SetPlayer(Player* player)
	{
		mPlayer = player;
		mPlayerAnim = player->GetScript<PlayerMeshScript>();
	}
}
