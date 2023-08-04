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


		void SetSituation(enums::eSituation situation) { mSituation = situation; }
		void SetHp(float hp) { mHp =+ hp; }
		void SetMaxHP(float maxhp) { mMaxHp =+ maxhp; }
		void SetSpeed(float speed) { mSpeed = speed; }
		void SetDeathBlowCount(float blowcount) { mDeathBlowCount =+ blowcount; }
		void SetMaxDeathBlowCount(float maxblowcount) { mMaxDeathBlowCount =+ maxblowcount; }
		void SetAlertnessCount(float count) { mAlertnessCount =+ count; }
		void SetDeathBlow(bool deathblow) { mbDeathBlow = deathblow; }




	private:

		enums::eSituation	mSituation;				//��� �������� 


		float               mHp;
		float               mMaxHp;

		float               mSpeed;

		float               mDeathBlowCount;		//�λ� ������
		float               mMaxDeathBlowCount;       

		float				mAlertnessCount;		//�溸 ���� (60�̻��̸� ���, 80�̻��̸� �߰� 100�̻��̸� ����)

		bool				mbDeathBlow;			//�λ� ������ �������� Ȯ��


	};

}