#pragma once
#include "yaMonsterBase.h"

namespace ya
{
    class MeshData;
	class Tenzen : public MonsterBase
	{
        //몬스터의 상태
        //뭐뭐있을까 ?
        //아무일도 없는경우 팔짱끼고 idle
        //플레이어가 가까이 오면 경계게이지가 쌓인다. 거리에 비례하는 듯? 
        //노랑경계게이지가 다 쌓이면 본격 수색에 들어감.
        //수색중에 발견되면 빨간 경계상태. 플레이어를 이제부터 인지하며 추격 및 전투에 돌입.
        //거리가 만약 일정수준 이상 벌어지면 플레이어를 추격. 이동한다.
        //시야에서 사라지면 다시 노란색 게이지로 변경되며 수색모션으로 변환
        //전투중에는 여러가지 공격 모션을 활용한다.
        //방어 모션도 활용한다.
        //
        //
        enum eTenzenState
        {
            TenzenState_None = 0x0000,
            TenzenState_Idle = 0x0001,
            TenzenState_Alert = 0x0002,
            TenzenState_Recognize = 0x0004,
            TenzenState_Attack = 0x0008,
            TenzenState_Deffense = 0x0010,
            TenzenState_Trace = 0x0020,
            TenzenState_OnHit = 0x0040,
            TenzenState_Dead = 0x0080,
        };
    public:

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;

        void SetAnimation();

    private:
        MeshData* mMeshData;
        GameObject* mKatana;

        int mState;

	};

}