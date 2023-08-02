#pragma once
#include "MonsterBase.h"

namespace ya
{
    class Spearman :
        public MonsterBase
    {
    public:

        virtual void Initalize() override;
        //virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;



    public:



    private:

    };
}
