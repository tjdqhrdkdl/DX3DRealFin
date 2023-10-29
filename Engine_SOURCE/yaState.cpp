#include "yaState.h"

namespace ya
{
	State::State()
		: mHp(100.f)
		, mHpMax(100.f)
		, mPosture(0.f)
		, mPostureMax(100.f)
		, mResurrectionCount(1)
		, mResurrectionCountMax(1)
		, mAttack(0.f)
		, mSpeed(0.f)
		, mbDeathBlow(false)
		, mbDeath(false)
		, mStunEvent(nullptr)
		, mDeathEvent(nullptr)
		, mResurrectionEvent(nullptr)
	{
	}

	State::~State()
	{
	}

	void State::AddHp(float hp)
	{
		mHp += hp; 

		if (mHp < 0)
		{
			mHp = 0;
			mbDeath = true;

			if (mDeathEvent != nullptr)
				mDeathEvent();
		}
	}

	void State::AddPosture(float posture)
	{
		mPosture += posture;

		if (mPosture > mPostureMax)
		{
			mPosture = mPostureMax;
			mbDeathBlow = true;

			if (mStunEvent != nullptr)
				mStunEvent();
		}
	}

	void State::AddResurrectionCount(int count)
	{
		if (mResurrectionCount > mResurrectionCountMax)
			mResurrectionCount = mResurrectionCountMax;
		else
			mResurrectionCount += count;

		if (mResurrectionCount <= 0)
			mResurrectionCount = 0;
	}

	void State::Death()
	{
		if (mResurrectionCount > 0)
		{

		}
		else
		{
			// 완전히 죽음..
		}
	}

	// 부활
	void State::Resurrection()
	{
		if (mbDeath && mResurrectionCount > 0)
		{
			AddResurrectionCount(-1);

			mHp = mHpMax;
			mPosture = 0;
			mbDeath = false;

			if(mResurrectionEvent != nullptr)
				mResurrectionEvent();
		}
	}
}