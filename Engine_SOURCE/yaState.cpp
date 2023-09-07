#include "yaState.h"

namespace ya
{
	State::State()
	{
		mSituation = enums::eSituation::None;
		mHp = 0.f;
		mMaxHp = 0.f;
		mSpeed = 0.f;
		mDeathBlowCount = 0.f;
		mMaxDeathBlowCount = 0.f;
		mAlertnessCount = 0.f;
		mbDeathBlow = 0.f;
		//mbStartBlow = true;
		mbDeathBlowOnOff = true;

	}

	State::~State()
	{
	}

	void State::SetHp(float hp)
	{
		if (mHp + hp < 0)
		{
			mHp = 0;
			mSituation = enums::eSituation::Death;
		}
		else 
		{
			mHp += hp;
		}

	}

	void State::SetDeathBlowCount(float blowcount)
	{
		if (mDeathBlowCount + blowcount > mMaxDeathBlowCount || mDeathBlowCount + blowcount < 0)
		{
			if(mDeathBlowCount + blowcount > mMaxDeathBlowCount)
			{
				mDeathBlowCount = mMaxDeathBlowCount;
				mSituation = enums::eSituation::Groggy;
				mbDeathBlow = true;
				mbDeathBlowOnOff = false;
			}
			else if (mDeathBlowCount + blowcount < 0)
			{
				mDeathBlowCount = 0.f;
			}
		}
		else
			mDeathBlowCount += blowcount;	
	}

}