#pragma once
#include "yaGameObject.h"
#include "yaSekiroEnums.h"
#include "yaState.h"

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

		void SetState(ePlayerState state) { mState = state; }
		ePlayerState GetState() { return mState; }

		void SetProsthetic(eProsthetics prosthetic) { mProsthetic = prosthetic; }
		eProsthetics GetProsthetic() { return mProsthetic; }

		void SetStateFlag(ePlayerState state, bool on = true);
		UINT GetStateFlag() { return mStateFlag; }

		bool IsAttack() { return mbTESTAttack; }
		void SetAttack(bool attack) { mbTESTAttack = attack; }
		bool IsWalk() { return mbTESTWalk; }
		void SetWalk(bool walk) { mbTESTWalk = walk; }

		bool IsStealth() { return mbStealth; }
		void SetStealth(bool stealth) { mbStealth = stealth; }

	private:
		GameObject* mCamera;

		State state;
		UINT mStateFlag;
		ePlayerState mState;
		eProsthetics mProsthetic;

		bool mbStealth;	// 은신

		bool mbTESTAttack;
		bool mbTESTWalk;
	};
}
