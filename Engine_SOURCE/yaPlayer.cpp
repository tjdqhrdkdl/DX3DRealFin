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
#include "yaPlayerScreenUI.h"
#include "yaPlayerDangerUI.h"
#include "yaParryEffect.h"
#include "yaParticleSystem.h"

namespace ya
{
	Player::Player()
		: GameObject()
		, mCamera(nullptr)
		//, mProsthetic(eProsthetics::None)
		, mWeaponCollider(nullptr)
		, mStartStateEvent {}
		, mEndStateEvent {}
		, mbControl(true)
		, mControlTimer(0.0f)
		, mStateFlag(0)
		, mOriginSetting {}
		, mbPostureRecovery(false)
		, mBeforePosture(0.f)
		, mPostureRecoveryTimeChecker(0.f)
	{
		SetName(L"Player");

		Transform* tr = GetComponent<Transform>();
		Vector3 pos = Vector3(190.f, -28.6f, 88.0f);

		tr->SetPosition(pos);
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
		mState->SetHPMax(100.0f);
		mState->SetHp(mState->GetHPMax());
		mState->SetPostureMax(100.0f);
		mState->SetPosture(0);
		mState->SetResurrectionCountMax(1);
		mState->SetResurrectionCount(mState->GetResurrectionCountMax());

		// 최초 정보 저장
		mOriginSetting.position = pos;
		mOriginSetting.state = *mState;
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

		CreatePlayerUI();

		GameObject::Initialize();
	}

	void Player::Update()
	{
		if (!mbControl)
		{
			if (mControlTimer > 0.0f)
				mControlTimer -= Time::DeltaTime();
			else
				mbControl = true;
		}

		//체간 자연 회복
		{
			float posture = GetState()->GetPosture();
			if (mBeforePosture < posture)
			{
				mbPostureRecovery = false;
			}

			if (mbPostureRecovery == false)
			{
				mPostureRecoveryTimeChecker += Time::DeltaTime();
				if (mPostureRecoveryTimeChecker > 10.f)
				{
					mPostureRecoveryTimeChecker = 0.f;
					mbPostureRecovery = true;
				}
			}
			else
			{
				float newPosture = posture - Time::DeltaTime() * 8.f;
				if (newPosture < 0.f)
					newPosture = 0.f;

				GetState()->SetPosture(newPosture);
			}

			mBeforePosture = posture;
		}

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

	void Player::OnDeathUI(bool on)
	{
		mPlayerScreenUI->OnDeathUI(on);
	}

	void Player::OnGameOverUI(bool on)
	{
		mPlayerScreenUI->OnGameOverUI(on);
	}

	void Player::CreatePlayerUI()
	{		
		mPlayerScreenUI = object::Instantiate<PlayerScreenUI>(eLayerType::UI, GetScene());
		mPlayerScreenUI->SetPlayer(this);		
		mPlayerDangerUI = object::Instantiate<PlayerDangerUI>(eLayerType::UI, GetScene());
		mPlayerDangerUI->SetPlayer(this);
		mParryEffect = object::Instantiate<ParryEffect>(eLayerType::UI, GetScene());
		mParryEffect->SetPlayer(this);
		{
			mParticleObject= object::Instantiate<GameObject>(eLayerType::Particle, GetScene());
			mParticleObject->SetName(L"particle");
			mParticleObject->AddComponent<ParticleSystem>();
			
		}
	}

	// block 상태가 얼마나 지속 되었는지 시간 return
	float Player::GetBlockTime()
	{
		PlayerAttackScript* attack = GetScript<PlayerAttackScript>();
		if (attack == nullptr)
			return 0.0f;
		else
			return attack->GetBlockTime();
	}

	void Player::Reset()
	{
		// state reset
		mState->SetHPMax(mOriginSetting.state.GetHPMax());
		mState->SetHp(mOriginSetting.state.GetHP());
		mState->SetPostureMax(mOriginSetting.state.GetPostureMax());
		mState->SetPosture(0);
		mState->SetResurrectionCountMax(mOriginSetting.state.GetResurrectionCountMax());
		mState->SetResurrectionCount(mOriginSetting.state.GetResurrectionCount());

		// position reset
		Transform* tr = GetComponent<Transform>();
		tr->SetPosition(mOriginSetting.position);

		// 인살 대상 reset
		PlayerAttackScript* attack = GetScript<PlayerAttackScript>();
		if (attack == nullptr)
			return;
		else
			attack->ClearDeathBlowTarget();
		
		// state reset
		mStateFlag = 0;

		// UI reset
		mPlayerScreenUI->OnDeathUI(false);
		mPlayerScreenUI->OnGameOverUI(false);
	}

	/// <summary>
	/// 인살 가능한 몬스터를 플레이어의 인살 가능 몬스터 목록에 저장한다.
	/// </summary>
	/// <param name="monster">몬스터 object</param>
	/// <param name="distance">몬스터와의 거리</param>
	void Player::SetDeathBlowTarget(MonsterBase* monster, float distance)
	{
		PlayerAttackScript* attack = GetScript<PlayerAttackScript>();
		if (attack == nullptr)
			return;
		else
			attack->SetDeathBlowTarget(monster, distance);
	}

	/// <summary>
	/// 인살 불가능한 몬스터를 플레이어의 인살 가능 몬스터 목록에서 제거한다.
	/// </summary>
	/// <param name="monster"></param>
	void Player::EraseDeathBlowTarget(MonsterBase* monster)
	{
		PlayerAttackScript* attack = GetScript<PlayerAttackScript>();
		if (attack == nullptr)
			return;
		else
			attack->EraseDeathBlowTarget(monster);
	}

	void Player::DangerUION()
	{
		mPlayerDangerUI->UIOn();
	}

	void Player::ParryEffectOn()
	{
		mParryEffect->UIOn();
		ParticleSystem* particleSys = mParticleObject->GetComponent<ParticleSystem>();
		particleSys->ParticleOn();
		particleSys->SetParticleNum(100);

		Transform* tr = mParticleObject->GetComponent<Transform>();
		tr->SetPosition(GetComponent<Transform>()->GetPosition() + GetComponent<Transform>()->Forward());
	}

}
