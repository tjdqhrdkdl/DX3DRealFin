#pragma once
#include "yaGameObject.h"
#include "yaTime.h"
#include "yaState.h"
#include "yaTransform.h"
#include "yaRigidbody.h"

namespace ya
{
    class MonsterBase :
        public GameObject
    {

    public:
        MonsterBase();
        virtual ~MonsterBase();


    public:

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() = 0;
        virtual void Render() = 0;


    public:

        GameObject* GetPlayerObject() { return mPlayerObject ; }

        Vec3 GetPlayerPos() { return mPlayerPos; }
        Vec3 GetMonster2PlayerNormalize() { return mMonster2PlayerNormalize; }
        Vec3 GetPlayer2MonsterNormalize() { return mPlayer2MonsterNormalize; }
        bool IsPlayerFront() { return mbPlayerFront; }
      

        void SetPlayerObject(GameObject* target) { mPlayerObject = target; }

        void SetPlayerPos(Vec3 pos) { mPlayerPos = pos; }
		void SetMonster2PlayerNormalize(Vec3 mormal) { mMonster2PlayerNormalize = mormal; }
		void SetPlayer2MonsterNormalize(Vec3 mormal) { mPlayer2MonsterNormalize = mormal; }
        void SetPlayerFront(bool front) {  mbPlayerFront = front; }


    private:

        GameObject*         mPlayerObject;
        State*              mMonsterState;

        Vec3                mPlayerPos;
        Vec3                mMonster2PlayerNormalize;
        Vec3                mPlayer2MonsterNormalize;


        bool                mbPlayerFront;

    };
}
