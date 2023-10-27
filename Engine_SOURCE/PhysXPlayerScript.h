#pragma once
#include "yaScript.h"

namespace ya
{
    class PhysXPlayerScript :
        public Script
    {
    public:
        PhysXPlayerScript();
        virtual ~PhysXPlayerScript();

        virtual void Start() override;
        virtual void Update() override;

        virtual void OnCollisionEnter(GameObject* _otherObj, const Vector3& _hitPoint) override;
        virtual void OnCollisionStay(GameObject* _otherObj, const Vector3& _hitPoint) override;
        virtual void OnCollisionExit(GameObject* _otherObj, const Vector3& _hitPoint) override;
    };
}


