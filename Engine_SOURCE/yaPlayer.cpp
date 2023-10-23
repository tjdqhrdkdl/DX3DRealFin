#include "yaPlayer.h"

#include "yaRigidbody.h"

#include "yaGrappleHookScript.h"
#include "yaHookTargetScript.h"
#include "yaPlayerMeshScript.h"
#include "yaPlayerActionScript.h"
#include "yaPlayerAttackScript.h"
#include "yaPlayerProjectileScript.h"

#include "yaBoneCollider.h"
#include "yaObject.h"

#include "yaState.h"
#include "yaPlayerHpTxture.h"


namespace ya
{
	Player::Player()
		: GameObject()
		, mCamera(nullptr)
		, mProsthetic(eProsthetics::None)
		, mWeaponCollider(nullptr)
		, mStartStateEvent {}
		, mEndStateEvent {}
	{
		SetName(L"Player");

		Transform* tr = GetComponent<Transform>();
		tr->SetPosition(Vector3(30.0f, 0.0f, -30.0f));
		//tr->SetScale(Vector3(0.4f, 0.4f, 0.4f));

		/*MeshRenderer* mr = player->AddComponent<MeshRenderer>();
		mr->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
		mr->SetMaterial(Resources::Find<Material>(L"BasicMaterial"), 0);*/

		Collider2D* col = AddComponent<Collider2D>();
		col->SetType(eColliderType::Box);
		//col->SetCenter(Vector3(0.f, 8.0f, 0.f));

		col->SetCenter(Vector3(0.f, 0.25f, 0.f));
		col->SetSize(Vector3(0.6f, 1.6f, 0.6f));

		Rigidbody* playerRigidbody = AddComponent<Rigidbody>();

		mState = new State();
		mState->SetHPMax(30.0f);
		mState->SetHp(mState->GetHPMax());
		mState->SetPostureMax(100.0f);
		mState->SetPosture(0);
		mState->SetResurrectionCountMax(3);
		mState->SetResurrectionCount(mState->GetResurrectionCountMax());
	}

	Player::~Player()
	{
		delete mState;
		mState = nullptr;
	}

	void Player::Initialize()
	{
		PlayerMeshScript* meshScript = AddComponent<PlayerMeshScript>();	// 메쉬, 애니메이션이므로 먼저 load

		std::shared_ptr<MeshData> weaponMeshData = meshScript->FindMeshData(ARM);
		if (weaponMeshData != nullptr)
		{
			mWeaponCollider = object::Instantiate<BoneCollider>(eLayerType::PlayerProjectile,GetScene());
			mWeaponCollider->SetMeshAndBone(weaponMeshData, L"R_Weapon");
			mWeaponCollider->SetScale(Vector3(1.6f, 0.2f, 0.2f));

			mWeaponCollider->SetBCOwner(this);
		}

		{
			PlayerProjectileScript* projectileScript = mWeaponCollider->AddComponent<PlayerProjectileScript>();
			projectileScript->SetPlayer(this);
		}

		AddComponent<PlayerActionScript>();
		AddComponent<PlayerAttackScript>();
		AddComponent<GrappleHookScript>();

		CreateHpTexture();

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

	void Player::CreateHpTexture()
	{		
		mPlayerHpBar = object::Instantiate<PlayerHpTxture>(eLayerType::UI, GetScene());
		mPlayerHpBar->SetName(L"player hp bar");
		mPlayerHpBar->SetPlayer(this);
	}

	float Player::GetBlockTime()
	{
		PlayerAttackScript* attack = GetScript<PlayerAttackScript>();
		if (attack == nullptr)
			return 0.0f;
		else
			return attack->GetBlockTime();
	}

	void Player::SetDeathBlowTarget(MonsterBase* monster, float distance)
	{
		PlayerAttackScript* attack = GetScript<PlayerAttackScript>();
		if (attack == nullptr)
			return;
		else
			attack->SetDeathBlowTarget(monster, distance);
	}

	void Player::EraseDeathBlowTarget(MonsterBase* monster)
	{
		PlayerAttackScript* attack = GetScript<PlayerAttackScript>();
		if (attack == nullptr)
			return;
		else
			attack->EraseDeathBlowTarget(monster);
	}

}
