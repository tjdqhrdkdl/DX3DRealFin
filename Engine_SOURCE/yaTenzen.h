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

    public:

        Tenzen();
        virtual ~Tenzen();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;

        virtual void DeathBlow() override;
        void Reset() override;
        void Idle();
        void Alert();
        void Recognize();
        void Attack();
        void Defense();
        void Trace();
        void Guard();
        void OnHit();
        void Groggy();


        void Move();
        void LookAtPlayer();
        

        void SetAnimationEvent();
       

        void RotateForwardTo(Vector3 dir);
        float GetDistanceToPlayer();
        float EyeSightCheck();


        void DrawSwordEndEvent();
        void DefenseEndEvent();
        void AttackEndEvent();
        void TraceEndEvent();
        void AttackSoundEvent();
        void AttackSwingSoundEvent();

        void OnCollisionEnter(Collider2D* collider) override;
        void OnCollisionStay(Collider2D* collider)  override;
        void OnCollisionExit(Collider2D* collider)  override;

        void SetAnimationName(const std::wstring& name) { mAnimationName = name; }
        void AddMonsterState(eMonsterState state);
		
        void SetAnimationChangeTime(float time ){ mMeshData->GetAnimator()->SetAnimationChangeTime(time); }
    private:
        GameObject* mKatanaCollider;
        Transform* mKatanaObjectTr;
        Transform* mKatanaHandleObjectTr;
        

        std::wstring mAnimationName;

        float mAlertTime;
        float mAlertTimeChecker;
        Vector3 mPlayerLastPosition;

        Vector3 mMoveDir;
        bool mbAnimReset;
        bool mbNavOn;

        class TenzenSwordScript* mSwordScript;
	};

}