#pragma once
#include "yaGameObject.h"
#include "yaSekiroEnums.h"
#include "yaState.h"
#include "yaBoneCollider.h"

namespace ya
{
	class MonsterBase;
	class PlayerScreenUI;
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

		void SetControl(bool control, float time = 2.0f) { mbControl = control; mControlTimer = time; }
		bool IsControl() { return mbControl; }

		std::map<ePlayerState, std::function<void()>>& GetStartStateEvent() { return mStartStateEvent; }
		std::map<ePlayerState, std::function<void()>>& GetEndStateEvent() { return mEndStateEvent; }

		//PlayerScreenUI* GetPlayerScreenUI() { return mPlayerScreenUI; }
		void OnDeathUI(bool on = true);
		void OnGameOverUI(bool on = true);

	private:
		void CreatePlayerUI();

	private:
		GameObject* mCamera;
		class PlayerScreenUI* mPlayerScreenUI;
		class PlayerDangerUI* mPlayerDangerUI;

		State* mState;
		UINT mStateFlag;
		//eProsthetics mProsthetic;
		BoneCollider* mWeaponCollider;

		// 조작할 수 있는지
		bool mbControl;
		float mControlTimer;

		std::map<ePlayerState, std::function<void()>> mStartStateEvent;
		std::map<ePlayerState, std::function<void()>> mEndStateEvent;

		OriginSetting mOriginSetting;
	};
}
