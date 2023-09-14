#include "yaPlayerProjectileScript.h"

namespace ya
{
	PlayerProjectileScript::PlayerProjectileScript()
		: Script()
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
		// 몬스터 공격
		int a = 0;

		// 패링
		GameObject* obj = collider->GetOwner();
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
}
