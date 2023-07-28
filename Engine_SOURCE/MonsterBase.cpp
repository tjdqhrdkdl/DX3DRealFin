#include "MonsterBase.h"

namespace ya

{

	MonsterBase::MonsterBase()
		: mGameObject(nullptr)
		, mHp(5)
		, mSpeed(5)
		, mDeathblowCount(0)
	{
	}

	MonsterBase::~MonsterBase()
	{
	}

	void MonsterBase::Initalize()
	{
	}

	void MonsterBase::Update()
	{
		if (nullptr != mGameObject)
		{
			mPlayerPos = mGameObject->GetComponent<Transform>()->GetPosition();
			mMonsterPos = GetComponent<Transform>()->GetPosition();

			mMonsterToPlayerNormalize = mPlayerPos - mMonsterPos;
			mMonsterToPlayerNormalize.Normalize();

			mPlayerToMonsterNormalize = mMonsterPos - mPlayerPos;
			mPlayerToMonsterNormalize.Normalize();



			mMonsterToPlayerNormalize;



			Vector3 monsterfoward = GetComponent<Transform>()->Foward();
			//Vector3 MonsterFianal = monsterfoward + mMonsterPos;
	
			//mMonsterToPlayerNormalize;

			float fov = monsterfoward.Dot(mMonsterToPlayerNormalize);

			if (fov > 0)
			{
				mbPlayerEyesight = true;
			}			
			else if (fov <= 0)
			{
				mbPlayerEyesight = false;
			}
			

		}



	



	}

}