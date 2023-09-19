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


#include <time.h>

namespace ya
{
    class MonsterBase :
        public GameObject
    {

    public:
        MonsterBase();
        virtual ~MonsterBase();


    public:

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() = 0;
        virtual void Render() override;


    public:

        
        bool NavigationPlayer(float range);
        void MonsterRotation(Vector3 target_point);
        void TurnToPlayer();
        float TurnToPlayerDir();
        bool WalkToPlayer(float range);
        void AlertnessLevel();


        bool IsPlayerFieldview(float minangle, float maxangle);
        int RandomNumber(int ieast, int Max);

        void OnceAniamtion(const std::wstring& animation);


#pragma region State_GetSet

        void CreateMonsterState();

        State* GetState() { return mMonsterState; }
        enums::eSituation GetSituation() { return mMonsterState->GetSituation(); }
        float GetHP() { return mMonsterState->GetHP(); }
        float GetMaxHP() { return mMonsterState->GetHPMax(); }
        float GetSpeed() { return mMonsterState->GetSpeed(); }
        float GetDeathBlowCount() { return mMonsterState->GetDeathBlowCount(); }
        float GetMaxDeathBlowCount() { return mMonsterState->GetMaxDeathBlowCount(); }
        float GetAlertnessCount() { return mMonsterState->GetAlertnessCount(); }
        bool IsDeathBlow() { return mMonsterState->IsDeathBlow(); }
        //bool IsStartBlow() { return mMonsterState->IsStartBlow(); }
        bool IsDeathBlowOnOff() { return mMonsterState->IsDeathBlowOnOff(); }



        void SetState(State* state) { mMonsterState = state; }
        void SetSituation(enums::eSituation situation, bool OnceAniamtion = false) 
        { 
            mMonsterState->SetSituation(situation);
            mbOnceAnimation = OnceAniamtion;
        }
        void SetHp(float hp) { mMonsterState->SetHp(hp);}
        void SetMaxHP(float maxhp) { mMonsterState->SetHPMax(maxhp); }
        void SetSpeed(float speed) { mMonsterState->SetSpeed(speed); }
        void SetDeathBlowCount(float blowcount) { mMonsterState->SetDeathBlowCount(blowcount); }
        void SetMaxDeathBlowCount(float maxblowcount) { mMonsterState->SetMaxDeathBlowCount(maxblowcount); }
        void SetAlertnessCount(float count) { mMonsterState->SetAlertnessCount(count); }
        void SetDeathBlow(bool deathblow) { mMonsterState->SetDeathBlow(deathblow); }
        //void SetStartBlow(bool blow) { mMonsterState->SetStartBlow(blow); }
        void SetDeathBlowonoff(bool onoff) { mMonsterState->SetDeathBlowonoff(onoff); }



#pragma endregion


    public:

        GameObject* GetPlayerObject() { return mPlayerObject ; }  

        Vec3 GetPlayerPos() { return mPlayerPos; }
        Vec3 GetMonster2PlayerNormalize() { return mMonster2PlayerNormalize; }
        Vec3 GetPlayer2MonsterNormalize() { return mPlayer2MonsterNormalize; }
        bool IsPlayerFront() { return mbPlayerFront; }
        //bool IsPlayerFieldview() { return mbPlayerFieldview; }
        bool IsDefense() { return mbDefense; }

      

        void SetPlayerObject(GameObject* target) { mPlayerObject = target; }

        void SetPlayerPos(Vec3 pos) { mPlayerPos = pos; }
		void SetMonster2PlayerNormalize(Vec3 mormal) { mMonster2PlayerNormalize = mormal; }
		void SetPlayer2MonsterNormalize(Vec3 mormal) { mPlayer2MonsterNormalize = mormal; }
        void SetPlayerFront(bool front) {  mbPlayerFront = front; }
        //void SetPlayerFieldview(bool view) { mbPlayerFieldview = view; }
        void SetDefense(bool defense) { mbDefense = defense; }
        void SetOnceAnimation(bool animation) { mbOnceAnimation = animation; }

    protected:

        std::shared_ptr<MeshData>   mMeshData;
        MeshObject*                 mMeshObject;
        Vec3                        mAnimationOffSet;


    private:

        GameObject*         mPlayerObject;
        State*              mMonsterState;

        Vec3                mPlayerPos;
        Vec3                mMonster2PlayerNormalize;
        Vec3                mPlayer2MonsterNormalize;



        bool                mbPlayerFront;
        //bool                mbPlayerFieldview;
        bool                mbDefense;
        bool                mbOnceAnimation;

        

        float               mTime;
        float               mRecoveryTime;

    };
}
