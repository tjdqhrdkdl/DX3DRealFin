#include "yaPlayer.h"

#include "yaPlayerScript.h"
#include "yaPlayerActionScript.h"
#include "yaGrappleHookScript.h"
#include "yaHookTargetScript.h"
#include "yaPlayerMeshScript.h"
#include "yaRigidbody.h"

namespace ya
{
	Player::Player()
		: mCamera(nullptr)
		, mState(ePlayerState::None)
		, mProsthetic(eProsthetics::None)
	{
		SetName(L"Player");

		Collider2D* col = AddComponent<Collider2D>();
		col->SetType(eColliderType::Box);
		col->SetSize(Vector3(1.0, 1.0f, 1.0f));

		Rigidbody* playerRigidbody = AddComponent<Rigidbody>();
		playerRigidbody->SetGround(false);

		AddComponent<PlayerMeshScript>();
		AddComponent<PlayerScript>();
		AddComponent<PlayerActionScript>();
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

	/// <summary>
	/// 플레이어 state의 bit flag on/off를 설정한다.
	/// </summary>
	/// <param name="state">on/off할 state</param>
	/// <param name="on">on/off 여부 bool</param>
	void Player::SetStateFlag(ePlayerState state, bool on)
	{
		if(on)
			mStateFlag |= ((UINT)state);
		else
			mStateFlag &= ~((UINT)state);
	}

}
