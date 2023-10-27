#pragma once
#include "yaScript.h"

namespace ya
{
    class GroundCheckScript :
        public Script
    {
    public:
        GroundCheckScript();
        virtual ~GroundCheckScript();

        virtual void OnCollisionEnter(GameObject* _otherObj, const Vector3& _hitPoint) override;
        virtual void OnCollisionExit(GameObject* _otherObj, const Vector3& _hitPoint) override;

        bool IsGround() const { return (mGrounded > 0); }

    private:
        int mGrounded;
    };
}


