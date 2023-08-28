#pragma once
#include "yaMonsterBase.h"

namespace ya
{
    class Swordsman :
        public MonsterBase
    {
    public:

        virtual void Initialize() override;
        //virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;

    public:




    private:

        float                   mTime;

    };
}
