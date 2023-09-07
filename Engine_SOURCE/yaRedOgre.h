#pragma once

#include "yaMonsterBase.h"

namespace ya
{
    class RedOgre :
        public MonsterBase
    {

    public:
        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;

    private:
        std::shared_ptr<MeshData> mMeshData;
    };
}

