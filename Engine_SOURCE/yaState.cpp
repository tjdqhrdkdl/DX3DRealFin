#include "yaState.h"

namespace ya
{
	State::State()
	{
		mHp = 0.f;
		mHpMax = 0.f;
		mSpeed = 0.f;
		mDeathBlowCount = 0.f;
		mMaxDeathBlowCount = 0.f;
		mbDeathBlow = 0.f;
		//mbStartBlow = true;
		mbDeathBlowOnOff = true;

	}

	State::~State()
	{
	}

	void State::AddHp(float hp)
	{
		mHp += hp; 

		if (mHp < 0)
			mbDeath = true;
	}

	void State::AddPosture(float posture)
	{
		mPosture += posture;

		if (mPosture < 0)
			mbDeathBlow = true;
	}

	void State::AddResurrectionCount(int count)
	{
		if (mResurrectionCount >= mResurrectionCountMax)
			mResurrectionCount = mResurrectionCountMax;
		else
			mResurrectionCount += count;

		if (mResurrectionCount <= 0)
			mbDeath = true;
	}
}