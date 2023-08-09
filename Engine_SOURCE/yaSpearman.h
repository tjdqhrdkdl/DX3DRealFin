#pragma once
#include "MonsterBase.h"


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

        bool IsParrying() { return mbParrying; }
        


    private:

        float                   mTime;
        float                   mAttackRange;
        Vec3                    mRandomXY;
        Vec3                    mRandomFinPos;
        Vec3                    mWlakFixPos;
        
        bool                    mbParrying;


    };
}
