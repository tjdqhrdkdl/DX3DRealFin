#pragma once

#include "yaMonsterBase.h"

namespace ya
{
    class RedOgre : public MonsterBase
    {

    public:
        RedOgre();
        virtual ~RedOgre();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;

    private:
        GameObject* mLHandCollider;
        GameObject* mRHandCollider;

        int mState;
        int mBeforeState;

        std::wstring mAnimationName;
    };
}

