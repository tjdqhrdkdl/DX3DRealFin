#pragma once
#include "yaEnums.h"
#include <functional>

namespace ya
{
	class State
	{
	public:
		State();
		virtual ~State();

	public:
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

		bool	IsDeath() { return mbDeath; }
		void	SetDeath(bool death) { mbDeath = death; }

		std::function<void()>& GetStunEvent() { return mStunEvent; }
		std::function<void()>& GetDeathEvent() { return mDeathEvent; }
		std::function<void()>& GetRessurctionEvent() { return mResurrectionEvent; }
		std::function<void()>& GetGameOverEvent() { return mGameOverEvent; }
	
	public:
		void	Death();
		void	Resurrection();

	private:
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

		// 인살 
		bool				mbDeathBlow;			//인살 가능한 상태인지 확인

		// 죽음
		bool				mbDeath;

		// 이벤트
		std::function<void()> mStunEvent;			// 체간 다찼을때
		std::function<void()> mDeathEvent;			// hp 0됐을때
		std::function<void()> mResurrectionEvent;	// 부활 할때
		std::function<void()> mGameOverEvent;		// 완전히 죽었을때
	};

}