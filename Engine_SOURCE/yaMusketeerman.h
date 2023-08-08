#pragma once
#include "MonsterBase.h"


namespace ya
{
    class Musketeerman :
        public MonsterBase
    {
    public:

        virtual void Initialize() override;
        //virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;

    public:


        void Attack_Almost();
        void Attack_Shooting();


    private:


        float           mTime;
        float           mAttackRange;
        bool            mbAlmostAttack;


    };
}
