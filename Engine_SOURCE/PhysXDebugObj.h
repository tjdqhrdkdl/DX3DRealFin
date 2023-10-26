#pragma once
#include "yaGameObject.h"

namespace ya
{
    class PhysXDebugObj :
        public GameObject
    {
    public:
        PhysXDebugObj();
        virtual ~PhysXDebugObj();

        virtual void Initialize() override;
        virtual void FixedUpdate() override;

    };
}


