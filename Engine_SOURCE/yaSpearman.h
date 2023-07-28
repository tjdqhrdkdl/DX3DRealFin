#pragma once
#include "MonsterBase.h"

namespace ya
{
    class Spearman :
        public MonsterBase
    {
    public:
        Spearman();
        virtual ~Spearman();

        //
        //virtual void Initalize() override;
        //virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;



    private:



    };
}
