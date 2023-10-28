#pragma once
#include "yaGameObject.h"

namespace ya
{
    class BossUI : public GameObject
    {
    public:
        BossUI();
        virtual ~BossUI();

    public:

        virtual void Initialize() override;
        virtual void Update() override;

        void SetMonster(class MonsterBase* mon) { mMonster= mon; }

        void UIOn() { mbRender = true; }
        void UIOff() { mbRender = false; }
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

        bool mbRender;

    };

}