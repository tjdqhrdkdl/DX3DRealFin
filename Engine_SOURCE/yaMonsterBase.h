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

#include <time.h>

namespace ya
{
    class Player;
    class MonsterBase : public GameObject
    {
    public:
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

    public:
        bool NavigationPlayer(float range);
        void MonsterRotation(Vector3 target_point);
        void TurnToPlayer();
        float TurnToPlayerDir();
        Vector3 TurnToPointDir(Vector3 point);
        bool WalkToPlayer(float range, float Speed);
       // Vector3 Convert3DTo2DScreenPos(Transform* tr);

        void AlertnessLevel();

        bool IsPlayerFieldview(float minangle, float maxangle);
       // int RandomNumber(int ieast, int Max);

        void OnceAniamtion(const std::wstring& animation);

    public:
        enums::eSituation GetSituation() { return mSituation; }
        void SetSituation(enums::eSituation situation, bool OnceAniamtion = false)
        {
            mSituation = situation;
            mbOnceAnimation = OnceAniamtion;
        }

        float	GetAlertnessCount() { return mAlertnessCount; }
        void	SetAlertnessCount(float count) { mAlertnessCount = count; }


    public: // State GetSet
        void    CreateMonsterState();

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

        float   GetDeathBlowCount() { return mMonsterState->GetDeathBlowCount(); }
        void    SetDeathBlowCount(float blowcount) { mMonsterState->SetDeathBlowCount(blowcount); }
        
        float   GetMaxDeathBlowCount() { return mMonsterState->GetMaxDeathBlowCount(); }
        void    SetMaxDeathBlowCount(float maxblowcount) { mMonsterState->SetMaxDeathBlowCount(maxblowcount); }
        
        bool    IsDeathBlow() { return mMonsterState->IsDeathBlow(); }
        void    SetDeathBlow(bool deathblow) { mMonsterState->SetDeathBlow(deathblow); }

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
        
        bool IsPlayerFront() { return mbPlayerFront; }
        void SetPlayerFront(bool front) { mbPlayerFront = front; }

        void SetHitRight(bool dir) { mbHitRight = dir; }
        bool IsHitRight() { return mbHitRight; }
        void SetHitLeft(bool dir) { mbHitLeft = dir; }
        bool IsHitLeft() { return mbHitLeft; }
        
        bool IsDeathBlowKill() { return mDeathBlowKill; }
        void SetDeathBlowKill(bool value) { mDeathBlowKill = value; }

        bool IsDefense() { return mbDefense; }
        void SetDefense(bool defense) { mbDefense = defense; }

        void SetOnceAnimation(bool animation) { mbOnceAnimation = animation; }

        Attack GetAttackParams() { return mAttackParams; }

     protected: // MonsterUI
         void CreateDeathBlowMark();

        Vec3 GetDeathBlowMarkOffSet() { return mDeathBlowMarkOffSet; }
        void SetDeathBlowMarkOffSet(Vec3 offset) { mDeathBlowMarkOffSet = offset; }
        void SetMonsterHpBarOffSetOffSet(Vec3 offset) { mMonsterHpBarOffSet = offset; }

    protected:
        std::shared_ptr<MeshData>   mMeshData;
        MeshObject* mMeshObject;
        ActionScript* mActionScript;

        GameObject* mDeathBlowMark;
        bool                        mbParrying;
        Attack                      mAttackParams;
        MonsterUI*                  mMonsterUI;
        CameraScript*               mCamScript;
        Player*                     mPlayerObject;

    private:
        State* mMonsterState;
        enums::eSituation	mSituation;


        float				mAlertnessCount;		//경보 레벨 (60이상이면 경계, 80이상이면 추격 100이상이면 공격)

        Vec3                mPlayerPos;
        Vec3                mMonster2PlayerNormalize;
        Vec3                mPlayer2MonsterNormalize;
        Vec3                mDeathBlowMarkOffSet;
        Vec3                mMonsterHpBarOffSet;

        bool                mbPlayerFront;
        bool                mbDefense;
        bool                mbOnceAnimation;

        bool                mbHitRight;
        bool                mbHitLeft;
        bool                mDeathBlowKill;

        bool                mbRecognize;

        bool                mbPostureRecovery;
        float               mBeforePosture;
        float               mPostureRecoveryTimeChecker;

    };
}
