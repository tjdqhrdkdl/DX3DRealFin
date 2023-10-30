#pragma once
#include "yaMonsterBase.h"


namespace ya
{
    class AshinaSpearMan :
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

        void OnCollisionEnter(Collider2D* collider) override;
        void OnCollisionStay(Collider2D* collider)  override;
        void OnCollisionExit(Collider2D* collider)  override;

        void SetAnimationName(const std::wstring& name) { mAnimationName = name; }
        void AddMonsterState(eMonsterState state);

        void SetAnimationChangeTime(float time) { mMeshData->GetAnimator()->SetAnimationChangeTime(time); }
        void KatanaColliderInit();

        //style은 0 , 1 , 2 가 있다.
        void SetStyle(UINT style);

    private:
        GameObject* mKatanaCollider;
        std::wstring mAnimationName;

        float mAlertTime;
        float mAlertTimeChecker;
        Vector3 mPlayerLastPosition;

        Vector3 mMoveDir;
        bool mbAnimReset;

        class SpearManSwordScript* mSwordScript;

        std::vector<Transform*> mHat;
        std::vector<Transform*> mStraw;
        std::vector<Transform*> mAccessories;
        std::vector<Transform*> mArmor;
        std::vector<Transform*> mBaseSword;
        std::vector<Transform*> mBaseSwordInScabbard;
        std::vector<Transform*> mShortSwordInScabbard;

        std::vector<Transform*>  mSpear;

        UINT mStyle;
    public:
        AshinaSpearMan();
        ~AshinaSpearMan();

    };
}