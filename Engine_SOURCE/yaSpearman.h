#pragma once
#include "yaMonsterBase.h"


namespace ya
{
    class Spearman :
        public MonsterBase
    {
    public:

        virtual void Initialize() override;
        //virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;



    public:


        void Attack_sting();



        void Idle_Stand();
        void Link_attack();
        void Link_Defense();
        void Link_Boundary();


        void KatanaColliderInit();
        void Animation_Event();

    private:

        float                   mTime;
        float                   mAttackRange;
        Vec3                    mRandomXY;
        Vec3                    mRandomFinPos;
        Vec3                    mWlakFixPos;

        UINT                    mRandom;





        //지울거에요 안씀
        class Transform* TEST;

    public:
        Spearman();
        ~Spearman();

    };
}
