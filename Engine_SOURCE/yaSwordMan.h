#pragma once
#include "yaMonsterBase.h"


namespace ya
{
    class SwordMan :
        public MonsterBase
    {
    public:

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;

        virtual void DeathBlow() override;

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

        void AttackStartEvent();

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

        void SetAnimationChangeTime(float time) { mMeshData->GetAnimator()->SetAnimationChangeTime(time); }
        void KatanaColliderInit();


    private:
        GameObject* mKatanaCollider;
        Transform* mKatanaObjectTr;
        Transform* mKatanaHandleObjectTr;

        std::wstring mAnimationName;

        float mAlertTime;
        float mAlertTimeChecker;

        Vector3 mMoveDir;
        bool mbAnimReset;

        class SwordManSwordScript* mRSwordScript;
        class SwordManSwordScript* mLSwordScript;

        bool mbMoveForward;
        bool mbNavOn;
    public:
        SwordMan();
        ~SwordMan();

    };
}