#include "MonsterBase.h"

namespace ya

{

	MonsterBase::MonsterBase()
		: mPlayerObject(nullptr)
		, mMonsterState(nullptr)
		, mPlayerPos(Vec3::Zero)
		, mMonster2PlayerNormalize(Vec3::Zero)
		, mPlayer2MonsterNormalize(Vec3::Zero)

	{
		if(nullptr != mMonsterState)
		{
			mMonsterState = new State();			
		}



	}

	MonsterBase::~MonsterBase()
	{
		if (nullptr != mMonsterState)
		{
			delete mMonsterState;
			mMonsterState = nullptr;
		}

	}

	void MonsterBase::Initalize()
	{


		GameObject::Initalize();
	}

	void MonsterBase::Update()
	{
		if (nullptr != mPlayerObject)
		{
			//위치 정보
			mPlayerPos = mPlayerObject->GetComponent<Transform>()->GetPosition();
			Vec3 monsterPos = GetComponent<Transform>()->GetPosition();

			mMonster2PlayerNormalize = mPlayerPos - mPlayer2MonsterNormalize;
			mMonster2PlayerNormalize.Normalize();

			mPlayer2MonsterNormalize = mPlayer2MonsterNormalize - mPlayerPos;
			mPlayer2MonsterNormalize.Normalize();
						

			//앞뒤 판별
			Vec3 monForward = GetComponent<Transform>()->Forward();
			float direction = monForward.Dot(mPlayerPos);

			if (direction > 0)
			{
				mbPlayerFront = true;
			}
			else if (direction < 0)
			{
				mbPlayerFront = false;
			}
			

			//플레이어 위치 1단계
			


		}

		

		GameObject::Update();
	}

}