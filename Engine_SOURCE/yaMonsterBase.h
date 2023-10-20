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
    
        void CreateDeathBlowMark();

        bool NavigationPlayer(float range);
        void MonsterRotation(Vector3 target_point);
        void TurnToPlayer();
        float TurnToPlayerDir();
        Vector3 TurnToPointDir(Vector3 point);
        bool WalkToPlayer(float range, float Speed);
        void AlertnessLevel();


        bool IsPlayerFieldview(float minangle, float maxangle);
        int RandomNumber(int ieast, int Max);

        void OnceAniamtion(const std::wstring& animation);

        Vector3 Convert3DTo2DScreenPos(Transform* tr);

        virtual void DeathBlow();

        enums::eSituation GetSituation() { return mSituation; }
        void SetSituation(enums::eSituation situation, bool OnceAniamtion = false)
        {
            mSituation = situation;
            mbOnceAnimation = OnceAniamtion;
        }

        float	GetAlertnessCount() { return mAlertnessCount; }
        void	SetAlertnessCount(float count) { mAlertnessCount = count; }

#pragma region State_GetSet

        void CreateMonsterState();

        State* GetState() { return mMonsterState; }
        float GetHP() { return mMonsterState->GetHP(); }
        float GetMaxHP() { return mMonsterState->GetHPMax(); }
        float GetSpeed() { return mMonsterState->GetSpeed(); }
        float GetDeathBlowCount() { return mMonsterState->GetDeathBlowCount(); }
        float GetMaxDeathBlowCount() { return mMonsterState->GetMaxDeathBlowCount(); }
        bool IsDeathBlow() { return mMonsterState->IsDeathBlow(); }
        //bool IsStartBlow() { return mMonsterState->IsStartBlow(); }
        bool IsDeathBlowOnOff() { return mMonsterState->IsDeathBlowOnOff(); }
        float GetPosture() { return mMonsterState->GetPosture(); }
        float GetPostureMax() { return mMonsterState->GetPostureMax(); }
        int		GetResurrectionCount() { return mMonsterState->GetResurrectionCount(); }
        int		GetResurrectionCountMax() { return mMonsterState->GetResurrectionCountMax(); }
        bool GetDeathBlowStart() {return mbDeathBlowStart; }
        bool IsRecognize() { return mbRecognize; }


        void SetRecognize(bool recognize) { mbRecognize = recognize; }
        void SetState(State* state) { mMonsterState = state; }
        void SetHp(float hp) { mMonsterState->SetHp(hp); }

        void AddHp(float hp) { mMonsterState->AddHp(hp); }

        void SetMaxHP(float maxhp) { mMonsterState->SetHPMax(maxhp); }
        void SetSpeed(float speed) { mMonsterState->SetSpeed(speed); }
        void SetDeathBlowCount(float blowcount) { mMonsterState->SetDeathBlowCount(blowcount); }
        void SetMaxDeathBlowCount(float maxblowcount) { mMonsterState->SetMaxDeathBlowCount(maxblowcount); }
        void SetDeathBlow(bool deathblow) { mMonsterState->SetDeathBlow(deathblow); }
        //void SetStartBlow(bool blow) { mMonsterState->SetStartBlow(blow); }
        void SetDeathBlowonoff(bool onoff) { mMonsterState->SetDeathBlowonoff(onoff); }
        void SetPosture(float posture) { mMonsterState->SetPosture(posture); }
        void SetPostureMax(float postureMax) { mMonsterState->SetPostureMax(postureMax); }
        void		SetResurrectionCount(int count) {  mMonsterState->SetResurrectionCount(count); }
        void		SetResurrectionCountMax(int count) {  mMonsterState->SetResurrectionCountMax(count); }
        void SetDeathBlowStart(bool start) { mbDeathBlowStart = start; }
#pragma endregion


    public:

        //enums::eSituation GetSituation() { return mSituation; }
        //void SetSituation(enums::eSituation situation, bool OnceAniamtion = false) { mSituation = situation; }

        Player* GetPlayerObject() { return mPlayerObject; }
        Vec3 GetPlayerPos() { return mPlayerPos; }
        Vec3 GetMonster2PlayerNormalize() { return mMonster2PlayerNormalize; }
        Vec3 GetPlayer2MonsterNormalize() { return mPlayer2MonsterNormalize; }
        Vec3 GetDeathBlowMarkOffSet() { return mDeathBlowMarkOffSet; }
        bool IsPlayerFront() { return mbPlayerFront; }
        //bool IsPlayerFieldview() { return mbPlayerFieldview; }
        bool IsDefense() { return mbDefense; }

        bool GetOnceAnimation() { return mbOnceAnimation; }

        bool IsHitRight() { return mbHitRight; }
        bool IsHitLeft() { return mbHitLeft; }
        bool IsDeathBlowKill() { return mDeathBlowKill; }

        void SetHitRight(bool dir) { mbHitRight = dir; }
        void SetHitLeft(bool dir) { mbHitLeft = dir; }
        void SetDeathBlowKill(bool value) { mDeathBlowKill = value; }

        void SetPlayerObject(Player* target) { mPlayerObject = target; }

        void SetPlayerPos(Vec3 pos) { mPlayerPos = pos; }
        void SetMonster2PlayerNormalize(Vec3 normal) { mMonster2PlayerNormalize = normal; }
        void SetPlayer2MonsterNormalize(Vec3 normal) { mPlayer2MonsterNormalize = normal; }
        void SetDeathBlowMarkOffSet(Vec3 offset) { mDeathBlowMarkOffSet = offset; }
        void SetMonsterHpBarOffSetOffSet(Vec3 offset) { mMonsterHpBarOffSet = offset; }
        void SetPlayerFront(bool front) { mbPlayerFront = front; }
        //void SetPlayerFieldview(bool view) { mbPlayerFieldview = view; }
        void SetDefense(bool defense) { mbDefense = defense; }
        void SetOnceAnimation(bool animation) { mbOnceAnimation = animation; }

  

        bool IsParrying() { return mbParrying; }

        Attack GetAttackParams() { return mAttackParams; }

    protected:

        std::shared_ptr<MeshData>   mMeshData;
        MeshObject* mMeshObject;
        Vec3                        mAnimationOffSet;
        ActionScript* mActionScript;
        GameObject* mDeathBlowMark;
        bool                        mbParrying;
        Attack                      mAttackParams;

    private:
        State* mMonsterState;
        enums::eSituation	mSituation;

        Player*             mPlayerObject;

        float				mAlertnessCount;		//경보 레벨 (60이상이면 경계, 80이상이면 추격 100이상이면 공격)



        Vec3                mPlayerPos;
        Vec3                mMonster2PlayerNormalize;
        Vec3                mPlayer2MonsterNormalize;
        Vec3                mDeathBlowMarkOffSet;
        Vec3                mMonsterHpBarOffSet;


        bool                mbPlayerFront;
        //bool              mbPlayerFieldview;
        bool                mbDefense;
        bool                mbOnceAnimation;

        bool                mbHitRight;
        bool                mbHitLeft;
        bool                mDeathBlowKill;

        bool                mbDeathBlowStart;


        float               mTime;
        float               mRecoveryTime;

        bool                mbRecognize;
        MonsterUI*          mMonsterUI;
    };
}
