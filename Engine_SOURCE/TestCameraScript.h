#pragma once
#include "yaScript.h"

namespace ya
{
    class TestCameraScript :
        public Script
    {
    public:
        TestCameraScript();
        virtual ~TestCameraScript();

        virtual void Update() override;

    };
}


