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
		float GetAttack() { return mAttack; }
		float GetDeathBlowCount() { return mDeathBlowCount; }
		float GetMaxDeathBlowCount() { return mMaxDeathBlowCount; }
		float GetAlertnessCount() { return mAlertnessCount; }
		bool IsDeathBlow() { return mbDeathBlow; }

		void SetSituation(enums::eSituation situation) { mSituation = situation; }
		void SetHp(float hp) { mHp = hp; }
		void SetMaxHP(float maxhp) { mMaxHp = maxhp; }
		void SetSpeed(float speed) { mSpeed = speed; }
		void SetDeathBlowCount(float blowcount) { mDeathBlowCount = blowcount; }
		void SetMaxDeathBlowCount(float maxblowcount) { mMaxDeathBlowCount = maxblowcount; }
		void SetAlertnessCount(float count) { mAlertnessCount =+ count; }
		void SetDeathBlow(bool deathblow) { mbDeathBlow = deathblow; }

		void AddHp(float hp) { mHp += hp; }
		void AddPosture(float posture) { mPosture += posture; }
		void AddDeathBlow(float deathblow) { mDeathBlowCount += deathblow; }


	private:

		enums::eSituation	mSituation;				//어떠한 상태인지 

		// HP
		float               mHp;
		float               mMaxHp;

		// 체간게이지
		float				mPosture;
		float				mPostureMax;

		// 회생 구슬
		int					mResurrectionCount;
		int					mResurrectionCountMax;

		// Status
		float				mAttack;
		float               mSpeed;

		// 인살 게이지
		bool				mbDeathBlow;			//인살 가능한 상태인지 확인
		float               mDeathBlowCount;		//인살 게이지
		float               mMaxDeathBlowCount;

		// 경보 레벨
		float				mAlertnessCount;		//경보 레벨 (60이상이면 경계, 80이상이면 추격 100이상이면 공격)
	};

}