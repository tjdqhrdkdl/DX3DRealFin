#pragma once
#include "yaGameObject.h"
#include "yaSekiroEnums.h"
#include "yaState.h"
#include "yaBoneCollider.h"

namespace ya
{
	class Player : public GameObject
	{
	public:
		Player();
		virtual ~Player();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	public:
		void SetCamera(GameObject* camera) { mCamera = camera; }
		GameObject* GetCamera() { return mCamera; }

		/*void SetState(ePlayerState state) { mState = state; }
		ePlayerState GetState() { return mState; }*/
		State* GetState() { return mState; }

		void SetProsthetic(eProsthetics prosthetic) { mProsthetic = prosthetic; }
		eProsthetics GetProsthetic() { return mProsthetic; }

		void SetWeaponCollider(BoneCollider* collider) { mWeaponCollider = collider; }
		BoneCollider* GetWeaponCollider() { return mWeaponCollider; }

		void SetStateFlag(ePlayerState state, bool on = true);
		UINT GetStateFlag() { return mStateFlag; }
		bool IsStateFlag(ePlayerState state) { return (GetStateFlag() & (UINT)state) ? true : false; }

		bool IsAttack() { return mbTESTAttack; }
		void SetAttack(bool attack) { mbTESTAttack = attack; }
		bool IsWalk() { return mbTESTWalk; }
		void SetWalk(bool walk) { mbTESTWalk = walk; }

		bool IsStealth() { return mbStealth; }
		void SetStealth(bool stealth) { mbStealth = stealth; }

		/*std::function<void()>& GetStartStateEvent() { return mStartStateEvent; }
		std::function<void()>& GetEndStateEvent() { return mEndStateEvent; }*/

		std::map<ePlayerState, std::function<void()>>& GetStartStateEvent() { return mStartStateEvent; }
		std::map<ePlayerState, std::function<void()>>& GetEndStateEvent() { return mEndStateEvent; }

	private:
		GameObject* mCamera;

		State* mState;
		UINT mStateFlag;
		eProsthetics mProsthetic;
		BoneCollider* mWeaponCollider;

		bool mbStealth;	// 은신

		std::map<ePlayerState, std::function<void()>> mStartStateEvent;
		std::map<ePlayerState, std::function<void()>> mEndStateEvent;

		bool mbTESTAttack;
		bool mbTESTWalk;
	};
}
