#pragma once
#include "yaGameObject.h"
#include "yaPlayer.h"
#include "yaTime.h"

namespace ya
{
    class MonsterBase :
        public GameObject
    {

    public:
        MonsterBase();
        virtual ~MonsterBase();


    public:

        virtual void Initalize() override;
        virtual void Update() override;
        virtual void FixedUpdate() = 0;
        virtual void Render() = 0;


        GameObject* GetTarget() { return mGameObject; }
        Vector3 GetPlayerToMonsterNormalize() { return mPlayerToMonsterNormalize; }
        Vector3 GetMonsterToPlayerNormalize() { return mMonsterToPlayerNormalize; }
        bool IsPlayerEyesight() { return mbPlayerEyesight; }

        void SetTarget(GameObject* gameobject) { mGameObject = gameobject; }
        



    private:

        GameObject* mGameObject;


        float mHp;
        float mSpeed;

        float mDeathblowCount;
        bool mbPlayerEyesight;

        Vector3 mPlayerPos;
        Vector3 mMonsterPos;
        
        Vector3 mPlayerToMonsterNormalize;
        Vector3 mMonsterToPlayerNormalize;



    };
}
