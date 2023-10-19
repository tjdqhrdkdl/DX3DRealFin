#pragma once
#include "yaGameObject.h"

namespace ya
{
    class MonsterUI :
        public GameObject
    {

    public:
        MonsterUI();
        virtual ~MonsterUI();


    public:

        virtual void Initialize() override;
        virtual void Update() override;
        void SetMonster(class MonsterBase* mon) { mMonster = mon; }


        void MonsterMeterCheck();

    private:

        class MonsterBase* mMonster;
        GameObject* mMonsterHpLayout;
        GameObject* mMonsterHpBar;
        GameObject* mMonsterPostureLayout;
        GameObject* mMonsterPostureBar1;
        GameObject* mMonsterPostureBar2;

        GameObject* mMonsterResurectionCount1;
        GameObject* mMonsterResurectionCount2;
        GameObject* mMonsterDeathBlow;
        GameObject* mMonsterLockOn;

        bool mbRender;
        float mRenderTime;
        float mRenderTimeChecker;


    };

}