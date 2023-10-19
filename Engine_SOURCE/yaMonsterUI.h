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


        void MonsterMeterCheak();

    private:

        class MonsterBase* mMonster;
        GameObject* mMonsterHpLayout;
        GameObject* mMonsterHpBar;
        GameObject* mMonsterResurrection;
        GameObject* mMonsterpostureLayout;




    };

}