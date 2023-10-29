#pragma once
#include "yaGameObject.h"
#include "yaApplication.h"
#include "yaTime.h"
#include "yaState.h"
#include "yaTransform.h"
#include "yaRigidbody.h"
#include "yaObject.h"
#include "yaResources.h"
#include "yaMeshRenderer.h"
#include "yaCollider2D.h"
#include "yaMeshData.h"
#include "yaMeshObject.h"
#include "yaBoneCollider.h"
#include "yaActionScript.h"
#include "yaBoneAnimator.h"
#include "yaMonsterUI.h"
#include "yaCameraScript.h"
#include "yaBossUI.h"

#include <time.h>

#define STATE_HAVE(STATE) (mState & STATE) == STATE
#define ADD_STATE(STATE) mState |= STATE
#define RM_STATE(STATE) mState &= ~STATE

#define BEFORE_STATE_HAVE(STATE) (mBeforeState & STATE) == STATE

namespace ya
{
    class Player;
    class MonsterBase : public GameObject
    {
    public:
        struct OriginSetting
        {
            Vector3 position;
            State state;
        };

        enum SpecialAttack
        {
            None,
            Kick,
            Catch,
            UpwardSlash,
        };

        struct Attack
        {
            float damage;
            bool unGuardable;
            SpecialAttack special;
        };

        enum eMonsterState
        {
            MonsterState_None = 0x00000000,
            MonsterState_Idle = 0x00000001,
            MonsterState_Alert = 0x00000002,
            MonsterState_Recognize = 0x00000004,
            MonsterState_DrawSword = 0x00000008,
            MonsterState_Attack = 0x00000010,
            MonsterState_Defense = 0x00000020,
            MonsterState_Guard = 0x00000040,
            MonsterState_GuardSuccess = 0x00000080,
            MonsterState_GuardLeft = 0x00000100,
            MonsterState_Trace = 0x00000200,
            MonsterState_Move = 0x00000400,
            MonsterState_OnHit = 0x00000800,
            MonsterState_OnHitFront = 0x00001000,
            MonsterState_AttackBlocked = 0x00002000,
            MonsterState_SuperArmor = 0x00004000,
            MonsterState_Groggy = 0x00008000,
            MonsterState_DeathBlow = 0x00010000,
            MonsterState_LookAt = 0x00020000,
            MonsterState_Dead = 0x00040000,
        };

    public:
        MonsterBase();
        virtual ~MonsterBase();

    public:
        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() = 0;
        virtual void Render() override;

    public:
        virtual void DeathBlow();
        virtual void Reset();

        void ParryEffectOn();
        GameObject* GetParticleObject();

        bool IsMonsterState(eMonsterState eState) { return STATE_HAVE(eState); }

    public:
        void SetAttackUnGuardable(bool b);

    public: // State GetSet
        State*  GetState() { return mMonsterState; }
        void    SetState(State* state) { mMonsterState = state; }

        float   GetHP() { return mMonsterState->GetHP(); }
        void    SetHp(float hp) { mMonsterState->SetHp(hp); }
        void    AddHp(float hp) { mMonsterState->AddHp(hp); }
        
        float   GetMaxHP() { return mMonsterState->GetHPMax(); }
        void    SetMaxHP(float maxhp) { mMonsterState->SetHPMax(maxhp); }
        
        float   GetSpeed() { return mMonsterState->GetSpeed(); }
        void    SetSpeed(float speed) { mMonsterState->SetSpeed(speed); }
        
        float   GetPosture() { return mMonsterState->GetPosture(); }
        void    SetPosture(float posture) { mMonsterState->SetPosture(posture); }

        float   GetPostureMax() { return mMonsterState->GetPostureMax(); }
        void    SetPostureMax(float postureMax) { mMonsterState->SetPostureMax(postureMax); }
        
        int		GetResurrectionCount() { return mMonsterState->GetResurrectionCount(); }
        void    SetResurrectionCount(int count) {  mMonsterState->SetResurrectionCount(count); }
        
        int		GetResurrectionCountMax() { return mMonsterState->GetResurrectionCountMax(); }
        void    SetResurrectionCountMax(int count) {  mMonsterState->SetResurrectionCountMax(count); }

        bool    IsDeathBlow() { return mMonsterState->IsDeathBlow(); }
        void    SetDeathBlow(bool deathblow) { mMonsterState->SetDeathBlow(deathblow); }

        float	GetAlertnessCount() { return mAlertnessCount; }
        void	SetAlertnessCount(float count) { mAlertnessCount = count; }

        bool    IsRecognize() { return mbRecognize; }
        void    SetRecognize(bool recognize) { mbRecognize = recognize; }

    public:
        Player* GetPlayerObject() { return mPlayerObject; }
        void SetPlayerObject(Player* target) { mPlayerObject = target; }

        Vec3 GetPlayerPos() { return mPlayerPos; }
        void SetPlayerPos(Vec3 pos) { mPlayerPos = pos; }
        
        Vec3 GetMonster2PlayerNormalize() { return mMonster2PlayerNormalize; }
        void SetMonster2PlayerNormalize(Vec3 normal) { mMonster2PlayerNormalize = normal; }
        
        void SetPlayer2MonsterNormalize(Vec3 normal) { mPlayer2MonsterNormalize = normal; }
        Vec3 GetPlayer2MonsterNormalize() { return mPlayer2MonsterNormalize; }

        Attack GetAttackParams() { return mAttackParams; }

        virtual void OnCollisionEnter(Collider2D* collider) = 0;
        virtual void OnCollisionStay(Collider2D* collider) = 0;
        virtual void OnCollisionExit(Collider2D* collider) = 0;

    protected:
        void SetOriginState(State* state) { mOriginSetting.state = *state; }
        void SetOriginPosition(Vector3 position) { mOriginSetting.position = position; }

    protected:
        std::shared_ptr<MeshData>   mMeshData;
        ActionScript*       mActionScript;

        Transform*          mTransform;
        Collider2D*         mCollider;

        BossUI*             mBossUI;
        MonsterUI*          mMonsterUI;
        CameraScript*       mCamScript;
        Player*             mPlayerObject;
        Attack              mAttackParams;

        int                 mBeforeState;
        int                 mState;

    private:
        State*              mMonsterState;

        float				mAlertnessCount;		//경보 레벨 (60이상이면 경계, 80이상이면 추격 100이상이면 공격)

        Vec3                mPlayerPos;
        Vec3                mMonster2PlayerNormalize;
        Vec3                mPlayer2MonsterNormalize;

        bool                mbRecognize;

        // 체간 자연 회복
        bool                mbPostureRecovery;
        float               mBeforePosture;
        float               mPostureRecoveryTimeChecker;

        OriginSetting       mOriginSetting;
    };
}
