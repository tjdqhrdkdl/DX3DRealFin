#include "yaPlayer.h"

#include "yaRigidbody.h"

#include "yaPlayerScript.h"
#include "yaPlayerActionScript.h"
#include "yaPlayerAttackScript.h"
#include "yaGrappleHookScript.h"
#include "yaHookTargetScript.h"
#include "yaPlayerMeshScript.h"

#include "yaBoneCollider.h"
#include "yaObject.h"

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
		col->SetCenter(Vector3(0.f, 0.f, 0.f));
		col->SetSize(Vector3(1.0, 1.0f, 1.0f));

		Rigidbody* playerRigidbody = AddComponent<Rigidbody>();

		AddComponent<PlayerScript>();
		AddComponent<PlayerMeshScript>();
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

		PlayerMeshScript* meshScript = GetScript<PlayerMeshScript>();
		std::shared_ptr<MeshData> weaponMeshData = meshScript->FindMeshData(L"Arm");
		if (weaponMeshData != nullptr)
		{
			BoneCollider* weaponCollider = object::Instantiate<BoneCollider>(eLayerType::PlayerProjectile);
			weaponCollider->SetMeshAndBone(weaponMeshData, L"R_Weapon");
			weaponCollider->SetScale(Vector3(1, 0.2, 0.2));

			//weaponCollider->SetAnimOffSet(L"SwingSword1", Vector3(1, 0.5, 1));
			weaponCollider->SetColliderActiveFrame(L"a000_000000", 0, 90);
			weaponCollider->SetColliderActiveFrame(L"a050_300100", 0, 20);
			/*weaponCollider->SetColliderActiveFrame(L"a050_305101", 0, 20);
			weaponCollider->SetColliderActiveFrame(L"a050_300020", 0, 20);
			weaponCollider->SetColliderActiveFrame(L"a050_300030", 0, 20);
			weaponCollider->SetColliderActiveFrame(L"a050_300040", 0, 20);*/
																	  
			//weaponCollider->SetColliderActiveFrame(L"a050_308010", 0, 20);
			//weaponCollider->SetColliderActiveFrame(L"a050_308000", 0, 20);
			//weaponCollider->SetColliderActiveFrame(L"a050_308050", 0, 20);
			//weaponCollider->SetColliderActiveFrame(L"a050_308060", 0, 20);

			////weaponCollider->SetColliderActiveFrame(L"a050_301050", 0, 100);
			//weaponCollider->SetColliderActiveFrame(L"a050_314000", 0, 20);
			//weaponCollider->SetColliderActiveFrame(L"a050_002000", 0, 20);
		}
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
