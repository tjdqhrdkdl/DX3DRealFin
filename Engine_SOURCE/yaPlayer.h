#pragma once
#include "yaGameObject.h"
#include "yaSekiroEnums.h"

namespace ya
{
	class Player : public GameObject
	{
	public:
		Player();
		virtual ~Player();

		virtual void Initalize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	public:
		void SetHp(int hp) { mHp = hp; }
		int GetHp() { return mHp; }
		void AddHp(int hp) { mHp += hp; }

		void SetPosture(int posture) { mPosture = posture; }
		int GetPosture() { return mPosture; }
		void AddPosture(int posture) { mPosture += posture; }

		void SetResurrectionCount(int count) { mResurrectionCount = count; }
		int GetRessurectionCount() { return mResurrectionCount; }
		void AddResurrectionCount(int count) { mResurrectionCount += count; }

		void SetProsthetic(eProsthetics prosthetic) { mProsthetic = prosthetic; }
		eProsthetics GetProsthetic() { return mProsthetic; }

	private:
		int mHp;
		int mHpMax;
		int mPosture;
		int mPostureMax;

		int mResurrectionCount;
		int mResurrectionCountMax;

		eProsthetics mProsthetic;
	};
}
