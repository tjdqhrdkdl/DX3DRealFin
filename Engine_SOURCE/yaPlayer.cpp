#include "yaPlayer.h"

#include "yaRigidbody.h"

#include "yaPlayerScript.h"
#include "yaPlayerActionScript.h"
#include "yaPlayerAttackScript.h"
#include "yaGrappleHookScript.h"
#include "yaHookTargetScript.h"
#include "yaPlayerMeshScript.h"
#include "yaPlayerProjectileScript.h"

#include "yaBoneCollider.h"
#include "yaObject.h"

#include "yaState.h"

namespace ya
{
	Player::Player()
		: mCamera(nullptr)
		, mProsthetic(eProsthetics::None)
		, mWeaponCollider(nullptr)
		, mStartStateEvent {}
		, mEndStateEvent {}
	{
		SetName(L"Player");

		Collider2D* col = AddComponent<Collider2D>();
		col->SetType(eColliderType::Box);
		//col->SetCenter(Vector3(0.f, 8.0f, 0.f));
		col->SetCenter(Vector3(0.f, 5.4f, 0.f));
		col->SetSize(Vector3(0.6, 3.2f, 0.6f));

		Rigidbody* playerRigidbody = AddComponent<Rigidbody>();

		mState = new State();
		mState->SetHPMax(100);
		mState->SetHp(mState->GetHPMax());
		mState->SetPostureMax(100);
		mState->SetPosture(mState->GetPostureMax());

		PlayerMeshScript* meshScript = AddComponent<PlayerMeshScript>();	// 메쉬, 애니메이션이므로 먼저 load

		std::shared_ptr<MeshData> weaponMeshData = meshScript->FindMeshData(L"Arm");
		if (weaponMeshData != nullptr)
		{
			mWeaponCollider = object::Instantiate<BoneCollider>(eLayerType::PlayerProjectile);
			mWeaponCollider->SetMeshAndBone(weaponMeshData, L"R_Weapon");
			mWeaponCollider->SetScale(Vector3(1.6, 0.2, 0.2));
		}


		AddComponent<PlayerScript>();
		AddComponent<PlayerActionScript>();
		AddComponent<PlayerAttackScript>();
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
		{
			if (!IsStateFlag(state))
			{
				std::map<ePlayerState, std::function<void()>>::iterator iter = mStartStateEvent.find(state);
				if (iter != mStartStateEvent.end())
					iter->second();
			}

			mStateFlag |= ((UINT)state);
		}
		else
		{
			if (IsStateFlag(state))
			{
				std::map<ePlayerState, std::function<void()>>::iterator iter = mEndStateEvent.find(state);
				if (iter != mEndStateEvent.end())
					iter->second();
			}
			
			mStateFlag &= ~((UINT)state);
		}
	}

	//std::function<void()>& Player::GetStartStateEvent(ePlayerState state)
	//{
	//	std::map<ePlayerState, std::function<void()>>::iterator iter = mStartStateEvent.find(state);

	//	if (iter == mStartStateEvent.end())
	//	{
	//		return nullptr;
	//	}

	//	return iter->second;
	//}

	//std::function<void()>& Player::GetEndStateEvent(ePlayerState state)
	//{
	//	// TODO: 여기에 return 문을 삽입합니다.
	//}

}
