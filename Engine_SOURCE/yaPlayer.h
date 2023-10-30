#pragma once
#include "yaGameObject.h"
#include "yaSekiroEnums.h"
#include "yaState.h"
#include "yaBoneCollider.h"

namespace ya
{
	class MonsterBase;
	class Player : public GameObject
	{
		struct OriginSetting
		{
			Vector3 position;
			State state;
		};

	public:
		Player();
		virtual ~Player();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	public:
		State* GetState() { return mState; }

		float GetBlockTime();

		void SetDeathBlowTarget(MonsterBase* monster, float distance);
		void EraseDeathBlowTarget(MonsterBase* monster);

		void DangerUION();
		void ParryEffectOn();
		GameObject* GetParticleObject() { return mParticleObject; }

		void Reset();

	public:
		void SetCamera(GameObject* camera) { mCamera = camera; }
		GameObject* GetCamera() { return mCamera; }

		//void SetProsthetic(eProsthetics prosthetic) { mProsthetic = prosthetic; }
		//eProsthetics GetProsthetic() { return mProsthetic; }

		void SetWeaponCollider(BoneCollider* collider) { mWeaponCollider = collider; }
		BoneCollider* GetWeaponCollider() { return mWeaponCollider; }

		void SetStateFlag(ePlayerState state, bool on = true);
		UINT GetStateFlag() { return mStateFlag; }
		bool IsStateFlag(ePlayerState state) { return (GetStateFlag() & (UINT)state) ? true : false; }

		bool IsStealth() { return mbStealth; }
		void SetStealth(bool stealth) { mbStealth = stealth; }

		std::map<ePlayerState, std::function<void()>>& GetStartStateEvent() { return mStartStateEvent; }
		std::map<ePlayerState, std::function<void()>>& GetEndStateEvent() { return mEndStateEvent; }

	private:
		void CreatePlayerUI();

	private:
		GameObject* mCamera;
		class PlayerHpTexture* mPlayerHpBar;
		class PlayerDangerUI* mPlayerDangerUI;
		class ParryEffect* mParryEffect;
		GameObject* mParticleObject;

		State* mState;
		UINT mStateFlag;
		//eProsthetics mProsthetic;
		BoneCollider* mWeaponCollider;

		bool mbStealth;	// 은신

		std::map<ePlayerState, std::function<void()>> mStartStateEvent;
		std::map<ePlayerState, std::function<void()>> mEndStateEvent;

		OriginSetting mOriginSetting;
	};
}
