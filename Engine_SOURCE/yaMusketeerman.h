#pragma once
#include "yaMonsterBase.h"

namespace ya
{
    class Musketeerman : public MonsterBase
    {
    public:
        Musketeerman();
        virtual ~Musketeerman();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;

    public:
        //void Attack_Almost();
        //void Attack_Shooting();
        //void Animation_Event();
        //void Idle_Stand();
        //void Link_attack();

    private:
        float           mTime;
        float           mAttackRange;
        bool            mbAlmostAttack;
    };
}
