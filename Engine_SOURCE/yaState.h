#pragma once
#include "yaEnums.h"

namespace ya
{
	class State
	{
	public:
		State();
		virtual ~State();

	public:

		enums::eSituation GetSituation() { return mSituation; }
		void SetSituation(enums::eSituation situation, bool OnceAniamtion = false) { mSituation = situation; }

		float	GetHP() { return mHp; }
		void	SetHp(float hp) { mHp = hp; }
		void	AddHp(float hp);

		float	GetHPMax() { return mHpMax; }
		void	SetHPMax(float maxhp) { mHpMax = maxhp; }
		
		float	GetPosture() { return mPosture; }
		void	SetPosture(float posture) { mPosture = posture; }
		void	AddPosture(float posture);
		
		float	GetPostureMax() { return mPostureMax; }
		void	SetPostureMax(float posture) { mPostureMax = posture; }
		
		float	GetSpeed() { return mSpeed; }
		void	SetSpeed(float speed) { mSpeed = speed; }
		
		float	GetAttack() { return mAttack; }
		void	SetAttack(float attack) { mAttack = attack; }

		int		GetResurrectionCount() { return mResurrectionCount; }
		void	SetResurrectionCount(int count) { mResurrectionCount = count; }
		void	AddResurrectionCount(int count);

		int		GetResurrectionCountMax() { return mResurrectionCountMax; }
		void	SetResurrectionCountMax(int count) { mResurrectionCountMax = count; }

		bool	IsDeathBlow() { return mbDeathBlow; }
		void	SetDeathBlow(bool deathblow) { mbDeathBlow = deathblow; }
		
		float	GetDeathBlowCount() { return mDeathBlowCount; }
		void	SetDeathBlowCount(float blowcount) { mDeathBlowCount = blowcount; }
		void	AddDeathBlow(float deathblow) { mDeathBlowCount += deathblow; }
		
		float	GetMaxDeathBlowCount() { return mMaxDeathBlowCount; }
		void	SetMaxDeathBlowCount(float maxblowcount) { mMaxDeathBlowCount = maxblowcount; }

		bool	IsDeathBlowOnOff() { return mbDeathBlowOnOff; }
		void	SetDeathBlowonoff(bool onoff) { mbDeathBlowOnOff = onoff; }

		//bool IsStartBlow() { return mbStartBlow; }
		//void SetStartBlow(bool blow) { mbStartBlow = blow; }

		bool	IsDeath() { return mbDeath; }
		void	SetDeath(bool death) { mbDeath = death; }

	private:

		enums::eSituation	mSituation;				

		// HP
		float               mHp;
		float               mHpMax;

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

		bool				mbDeathBlowOnOff;
		//bool				mbStartBlow;

		// 죽음
		bool				mbDeath;
	};

}