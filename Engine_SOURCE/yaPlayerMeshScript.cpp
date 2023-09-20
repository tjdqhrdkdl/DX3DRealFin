#include "yaPlayerMeshScript.h"
#include "yaPlayer.h"
#include "yaMeshData.h"
#include "yaInput.h"

namespace ya
{
    PlayerMeshScript::PlayerMeshScript()
        : Script()
        , mbUse(true)
        , mbLoad(false)
        , mMeshDataMap{}
    {
    }

    PlayerMeshScript::~PlayerMeshScript()
    {
    }

    void PlayerMeshScript::Initialize()
    {
        if (mbLoad)
        {
            std::shared_ptr<MeshData> arm = MeshData::LoadFromFbx(L"Player\\Mesh\\AM_M_9000.fbx");
            mMeshDataMap.insert(std::make_pair(L"Arm", arm));

            std::shared_ptr<MeshData> body = MeshData::LoadFromFbx(L"Player\\Mesh\\BD_M_9000.fbx");
            mMeshDataMap.insert(std::make_pair(L"Body", body));

            std::shared_ptr<MeshData> face = MeshData::LoadFromFbx(L"Player\\Mesh\\FC_M_0100.fbx");
            mMeshDataMap.insert(std::make_pair(L"Face", face));

            std::shared_ptr<MeshData> leg = MeshData::LoadFromFbx(L"Player\\Mesh\\LG_M_9000.fbx");
            mMeshDataMap.insert(std::make_pair(L"Leg", leg));

            std::shared_ptr<MeshData> weapon = MeshData::LoadFromFbx(L"Player\\Mesh\\WP_A_0300.fbx");
            mMeshDataMap.insert(std::make_pair(L"Weapon", weapon));

            std::shared_ptr<MeshData> waeponCase = MeshData::LoadFromFbx(L"Player\\Mesh\\WP_A_0300_1_b.fbx");
            mMeshDataMap.insert(std::make_pair(L"WeaponCase", waeponCase));

        }
        else
        {
            std::shared_ptr<MeshData> arm = std::make_shared<MeshData>();
            arm->Load(L"Player\\MeshData\\AM_M_9000.meshdata");
            mMeshDataMap.insert(std::make_pair(L"Arm", arm));

            std::shared_ptr<MeshData> body = std::make_shared<MeshData>();
            body->Load(L"Player\\MeshData\\BD_M_9000.meshdata");
            mMeshDataMap.insert(std::make_pair(L"Body", body));

            std::shared_ptr<MeshData> face = std::make_shared<MeshData>();
            face->Load(L"Player\\MeshData\\FC_M_0100.meshdata");
            mMeshDataMap.insert(std::make_pair(L"Face", face));

            std::shared_ptr<MeshData> leg = std::make_shared<MeshData>();
            leg->Load(L"Player\\MeshData\\LG_M_9000.meshdata");
            mMeshDataMap.insert(std::make_pair(L"Leg", leg));

            std::shared_ptr<MeshData> weapon = std::make_shared<MeshData>();
            weapon->Load(L"Player\\MeshData\\WP_A_0300.meshdata");
            mMeshDataMap.insert(std::make_pair(L"Weapon", weapon));

            std::shared_ptr<MeshData> waeponCase = std::make_shared<MeshData>();
            waeponCase->Load(L"Player\\MeshData\\WP_A_0300_1_b.meshdata");
            mMeshDataMap.insert(std::make_pair(L"WeaponCase", waeponCase));
        }

        if (mbUse)
        {
        }
            LoadAnimation();

        Player* player = dynamic_cast<Player*>(GetOwner());
        Transform* playerTr = player->GetComponent<Transform>();

        for (auto iter = mMeshDataMap.begin(); iter != mMeshDataMap.end(); iter++)
        {
            std::shared_ptr<MeshData> meshData = iter->second;

            meshData->Instantiate(eLayerType::Player);

            Transform* meshTr = meshData->GetMeshObject()->GetComponent<Transform>();
            meshTr->SetPosition(Vector3(0.0f, -0.5f, 0.0f));
            meshTr->SetRotation(Vector3(-180.0f, 0.0f, 0.0f));
            meshTr->SetScale(Vector3(2.0f, 2.0f, 2.0f));
            meshTr->SetParent(playerTr);
        }

        if (mbUse)
        {
            for (auto iter = mMeshDataMap.begin(); iter != mMeshDataMap.end(); iter++)
            {
                std::shared_ptr<MeshData> meshData = iter->second;

                meshData->GetAnimationEndEvent(L"a000_000010") = [meshData]() { meshData->Play(L"a000_000500"); };
                meshData->GetAnimationEndEvent(L"a000_000011") = [meshData]() { meshData->Play(L"a000_000500"); };
                meshData->GetAnimationEndEvent(L"a000_000012") = [meshData]() { meshData->Play(L"a000_000500"); };
                meshData->GetAnimationEndEvent(L"a000_000013") = [meshData]() { meshData->Play(L"a000_000500"); };
               // meshData->GetAnimationEndEvent(L"a000_200000") = [meshData]() { meshData->Play(L"a000_000000"); };
                //meshData->GetAnimationEndEvent(L"a000_001510") = [meshData]() { meshData->Play(L"a000_000000"); };
                meshData->GetAnimationEndEvent(L"a050_308000") = [meshData]() { meshData->Play(L"a000_201030"); };
                meshData->GetAnimationEndEvent(L"a050_308010") = [meshData]() { meshData->Play(L"a000_201030"); };
                meshData->GetAnimationEndEvent(L"a000_200000") = [meshData]() { meshData->Play(L"a000_201030"); };
                meshData->GetAnimationEndEvent(L"a000_201040") = [meshData]() { meshData->Play(L"a000_000000"); };

                //피격
                meshData->GetAnimationEndEvent(L"a000_100100") = [meshData]() { meshData->Play(L"a000_000000"); };
                meshData->GetAnimationEndEvent(L"a000_100101") = [meshData]() { meshData->Play(L"a000_000000"); };
                meshData->GetAnimationEndEvent(L"a000_100102") = [meshData]() { meshData->Play(L"a000_000000"); };
                meshData->GetAnimationEndEvent(L"a000_100103") = [meshData]() { meshData->Play(L"a000_000000"); };
                
                //패링
                meshData->GetAnimationEndEvent(L"a050_120100") = [meshData]() { meshData->Play(L"a000_000000"); };
                meshData->GetAnimationEndEvent(L"a050_120101") = [meshData]() { meshData->Play(L"a000_000000"); };
                meshData->GetAnimationEndEvent(L"a050_120102") = [meshData]() { meshData->Play(L"a000_000000"); };
                meshData->GetAnimationEndEvent(L"a050_120103") = [meshData]() { meshData->Play(L"a000_000000"); };
            }

            Play(L"a000_000000");
        }
    }

    void PlayerMeshScript::Update()
    {
    }

    void PlayerMeshScript::Render()
    {
    }

    void PlayerMeshScript::OnCollisionEnter(Collider2D* collider)
    {
    }

    void PlayerMeshScript::OnCollisionStay(Collider2D* collider)
    {
    }

    void PlayerMeshScript::OnCollisionExit(Collider2D* collider)
    {
    }

    void PlayerMeshScript::LoadAnimation()
    {
        if (mbLoad)
        {
            for (auto iter = mMeshDataMap.begin(); iter != mMeshDataMap.end(); iter++)
            {
                std::wstring name = iter->first;
                std::shared_ptr<MeshData> meshData = iter->second;

                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000000.fbx", L"a000_000000");

#pragma region 1차 통합
                /*meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000000.fbx", L"a000_000000");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000010.fbx", L"a000_000010");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000011.fbx", L"a000_000011");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000012.fbx", L"a000_000012");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000013.fbx", L"a000_000013");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000100.fbx", L"a000_000100");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000101.fbx", L"a000_000101");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000102.fbx", L"a000_000102");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000103.fbx", L"a000_000103");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000300.fbx", L"a000_000300");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000301.fbx", L"a000_000301");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000302.fbx", L"a000_000302");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000303.fbx", L"a000_000303");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000400.fbx", L"a000_000400");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000401.fbx", L"a000_000401");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000402.fbx", L"a000_000402");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000403.fbx", L"a000_000403");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000500.fbx", L"a000_000500");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000501.fbx", L"a000_000501");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000502.fbx", L"a000_000502");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000503.fbx", L"a000_000503");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000600.fbx", L"a000_000600");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000601.fbx", L"a000_000601");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000602.fbx", L"a000_000602");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000603.fbx", L"a000_000603");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001151.fbx", L"a000_001151");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001152.fbx", L"a000_001152");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001153.fbx", L"a000_001153");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001154.fbx", L"a000_001154");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001200.fbx", L"a000_001200");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001510.fbx", L"a000_001510");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005000.fbx", L"a000_005000");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005100.fbx", L"a000_005100");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005101.fbx", L"a000_005101");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005102.fbx", L"a000_005102");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005103.fbx", L"a000_005103");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005110.fbx", L"a000_005110");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005111.fbx", L"a000_005111");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005112.fbx", L"a000_005112");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005113.fbx", L"a000_005113");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005200.fbx", L"a000_005200");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005201.fbx", L"a000_005201");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005202.fbx", L"a000_005202");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005203.fbx", L"a000_005203");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005300.fbx", L"a000_005300");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005301.fbx", L"a000_005301");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005302.fbx", L"a000_005302");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005303.fbx", L"a000_005303");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020000.fbx", L"a000_020000");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020102.fbx", L"a000_020102");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020103.fbx", L"a000_020103");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021000.fbx", L"a000_021000");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021102.fbx", L"a000_021102");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021103.fbx", L"a000_021103");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021202.fbx", L"a000_021202");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021203.fbx", L"a000_021203");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021302.fbx", L"a000_021302");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021303.fbx", L"a000_021303");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200000.fbx", L"a000_200000");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200903.fbx", L"a000_200903");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200907.fbx", L"a000_200907");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200908.fbx", L"a000_200908");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201030.fbx", L"a000_201030");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201040.fbx", L"a000_201040");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202000.fbx", L"a000_202000");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202010.fbx", L"a000_202010");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202011.fbx", L"a000_202011");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202012.fbx", L"a000_202012");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202035.fbx", L"a000_202035");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202100.fbx", L"a000_202100");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202110.fbx", L"a000_202110");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202300.fbx", L"a000_202300");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202310.fbx", L"a000_202310");
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217102.fbx", L"a000_217102");

                meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a000_1.animationdata");*/

                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002000.fbx", L"a050_002000");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002010.fbx", L"a050_002010");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002011.fbx", L"a050_002011");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002012.fbx", L"a050_002012");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002013.fbx", L"a050_002013");
                ////meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002200.fbx", L"a050_002200");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002201.fbx", L"a050_002201");
                ////meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002202.fbx", L"a050_002202");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002203.fbx", L"a050_002203");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120100.fbx", L"a050_120100");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120101.fbx", L"a050_120101");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120102.fbx", L"a050_120102");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120103.fbx", L"a050_120103");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120104.fbx", L"a050_120104");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120105.fbx", L"a050_120105");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130101.fbx", L"a050_130101");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130102.fbx", L"a050_130102");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130200.fbx", L"a050_130200");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130201.fbx", L"a050_130201");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130202.fbx", L"a050_130202");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130300.fbx", L"a050_130300");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130301.fbx", L"a050_130301");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130302.fbx", L"a050_130302");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130700.fbx", L"a050_130700");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300020.fbx", L"a050_300020");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300030.fbx", L"a050_300030");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300040.fbx", L"a050_300040");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300100.fbx", L"a050_300100");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_305101.fbx", L"a050_305101");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308000.fbx", L"a050_308000");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308010.fbx", L"a050_308010");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308020.fbx", L"a050_308020");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308050.fbx", L"a050_308050");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308060.fbx", L"a050_308060");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308070.fbx", L"a050_308070");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_314000.fbx", L"a050_314000");

                //meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a050_1.animationdata");

                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001200.fbx", L"a000_001200");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100100.fbx", L"a000_100100");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100101.fbx", L"a000_100101");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100102.fbx", L"a000_100102");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100103.fbx", L"a000_100103");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100200.fbx", L"a000_100200");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100201.fbx", L"a000_100201");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100202.fbx", L"a000_100202");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100203.fbx", L"a000_100203");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100300.fbx", L"a000_100300");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100301.fbx", L"a000_100301");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100310.fbx", L"a000_100310");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100311.fbx", L"a000_100311");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100320.fbx", L"a000_100320");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100321.fbx", L"a000_100321");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120100.fbx", L"a050_120100");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120101.fbx", L"a050_120101");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120102.fbx", L"a050_120102");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120103.fbx", L"a050_120103");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120104.fbx", L"a050_120104");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120105.fbx", L"a050_120105");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120700.fbx", L"a050_120700");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130200.fbx", L"a050_130200");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130201.fbx", L"a050_130201");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130202.fbx", L"a050_130202");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130300.fbx", L"a050_130300");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_139900.fbx", L"a050_139900");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190100.fbx", L"a050_190100");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190155.fbx", L"a050_190155");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190500.fbx", L"a050_190500");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190501.fbx", L"a050_190501");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190502.fbx", L"a050_190502");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203010.fbx", L"a050_203010");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_206200.fbx", L"a050_206200");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_206201.fbx", L"a050_206201");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_206202.fbx", L"a050_206202");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_206203.fbx", L"a050_206203");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300000.fbx", L"a050_300000");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300001.fbx", L"a050_300001");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_305000.fbx", L"a050_305000");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_305001.fbx", L"a050_305001");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002200.fbx", L"a050_002200");
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002202.fbx", L"a050_002202");

                //meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a0xx_1.animationdata");
#pragma endregion

            }
        }
        else
        {
            for (auto iter = mMeshDataMap.begin(); iter != mMeshDataMap.end(); iter++)
            {
                std::wstring name = iter->first;
                std::shared_ptr<MeshData> meshData = iter->second;

                meshData->AnimationLoad(L"Player\\AnimationData\\" + name + L"_a000_1.animationdata", nullptr, false);
                meshData->AnimationLoad(L"Player\\AnimationData\\" + name + L"_a050_1.animationdata", nullptr, false);
                meshData->AnimationLoad(L"Player\\AnimationData\\" + name + L"_a0xx_1.animationdata");
            }
        }
    }
    void PlayerMeshScript::Play(std::wstring name)
    {
        if (!mbUse)
            return;

        for (auto iter = mMeshDataMap.begin(); iter != mMeshDataMap.end(); iter++)
        {
            std::shared_ptr<MeshData> meshData = iter->second;
            meshData->Play(name);
        }
    }

    std::shared_ptr<MeshData> PlayerMeshScript::FindMeshData(std::wstring name)
    {
       if (mMeshDataMap.find(name) == mMeshDataMap.end()) 
           return nullptr; 
       return mMeshDataMap.find(name)->second;
    }
}
