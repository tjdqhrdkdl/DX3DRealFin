#pragma once
#include "yaEnums.h"

namespace ya
{
	class State
	{
	public:




	public:
		State();
		virtual ~State();


	public:


		enums::eSituation GetSituation() { return mSituation; }
		float GetHP() { return mHp; }
		float GetMaxHP() { return mMaxHp; }
		float GetSpeed() { return mSpeed; }
		float GetDeathBlowCount() { return mDeathBlowCount; }
		float GetMaxDeathBlowCount() { return mMaxDeathBlowCount; }
		float GetAlertnessCount() { return mAlertnessCount; }
		bool IsDeathBlow() { return mbDeathBlow; }
		bool IsStartBlow() { return mbStartBlow; }
		bool IsDeathBlowOnOff() { return mbDeathBlowOnOff; }


		void SetSituation(enums::eSituation situation) { mSituation = situation; }
		void SetHp(float hp);
		void SetMaxHP(float maxhp) { mMaxHp += maxhp; }
		void SetSpeed(float speed) { mSpeed = speed; }
		void SetDeathBlowCount(float blowcount);
		void SetMaxDeathBlowCount(float maxblowcount) { mMaxDeathBlowCount =+ maxblowcount; }
		void SetAlertnessCount(float count) { mAlertnessCount += count; }
		void SetDeathBlow(bool deathblow) { mbDeathBlow = deathblow; }
		void SetStartBlow(bool blow) { mbStartBlow = blow; }
		void SetDeathBlowonoff(bool onoff) { mbDeathBlowOnOff = onoff; }




	private:

		enums::eSituation	mSituation;				//어떠한 상태인지 


		float               mHp;
		float               mMaxHp;

		float               mSpeed;

		

		float               mDeathBlowCount;				//인살 게이지
		float               mMaxDeathBlowCount;				//

		float				mAlertnessCount;				//경보 레벨 (60이상이면 경계, 80이상이면 추격 100이상이면 공격)

		bool				mbDeathBlow;					//인살 가능한 상태인지 확인
		bool				mbStartBlow;					//첫 암살 가능한 상태
		bool                mbDeathBlowOnOff;				//인살 상태에서 몇초 뒤에 풀어줄지

	};

}