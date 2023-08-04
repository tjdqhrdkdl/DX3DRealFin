#include "yaPlayer.h"

#include "yaPlayerScript.h"
#include "yaActionScript.h"
#include "yaMoveScript.h"
#include "yaGrappleHookScript.h"
#include "yaHookTargetScript.h"

#include "yaRigidbody.h"

namespace ya
{
	Player::Player()
		: mCamera(nullptr)
	{
		SetName(L"Player");

		Collider2D* col = AddComponent<Collider2D>();
		col->SetType(eColliderType::Box);
		col->SetSize(Vector3(1.0, 1.0f, 1.0f));

		Rigidbody* playerRigidbody = AddComponent<Rigidbody>();
		playerRigidbody->SetGround(false);

		AddComponent<PlayerScript>();
		AddComponent<ActionScript>();
		AddComponent<MoveScript>();
		AddComponent<GrappleHookScript>();
	}

	Player::~Player()
	{
	}

	void Player::Initialize()
	{
		GameObject::Initialize();
	}

	void Player::Update()
	{
		GameObject::Update();
	}

	void Player::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void Player::Render()
	{
		GameObject::Render();
	}

}
