#include "yaPlayerMeshScript.h"
#include "yaPlayer.h"
#include "yaMeshData.h"
#include "yaInput.h"

#include "yaSceneManager.h"
#include "yaScene.h"

namespace ya
{
    PlayerMeshScript::PlayerMeshScript()
        : Script()
        , mbUse(true)
        , mbLoad(false)
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
            mMeshDataMap.insert(std::make_pair(ARM, arm));

            std::shared_ptr<MeshData> body = MeshData::LoadFromFbx(L"Player\\Mesh\\BD_M_9000.fbx");
            mMeshDataMap.insert(std::make_pair(BODY, body));

            std::shared_ptr<MeshData> face = MeshData::LoadFromFbx(L"Player\\Mesh\\FC_M_0100.fbx");
            mMeshDataMap.insert(std::make_pair(FACE, face));

            std::shared_ptr<MeshData> leg = MeshData::LoadFromFbx(L"Player\\Mesh\\LG_M_9000.fbx");
            mMeshDataMap.insert(std::make_pair(LEG, leg));

            std::shared_ptr<MeshData> weapon = MeshData::LoadFromFbx(L"Player\\Mesh\\WP_A_0300.fbx");
            mMeshDataMap.insert(std::make_pair(WEAPON, weapon));

            std::shared_ptr<MeshData> waeponCase = MeshData::LoadFromFbx(L"Player\\Mesh\\WP_A_0300_1_b.fbx");
            mMeshDataMap.insert(std::make_pair(WEAPONCASE, waeponCase));

        }
        else
        {
            std::shared_ptr<MeshData> arm = std::make_shared<MeshData>();
            arm->Load(L"Player\\MeshData\\AM_M_9000.meshdata");
            mMeshDataMap.insert(std::make_pair(ARM, arm));

            std::shared_ptr<MeshData> body = std::make_shared<MeshData>();
            body->Load(L"Player\\MeshData\\BD_M_9000.meshdata");
            mMeshDataMap.insert(std::make_pair(BODY, body));

            std::shared_ptr<MeshData> face = std::make_shared<MeshData>();
            face->Load(L"Player\\MeshData\\FC_M_0100.meshdata");
            mMeshDataMap.insert(std::make_pair(FACE, face));

            std::shared_ptr<MeshData> leg = std::make_shared<MeshData>();
            leg->Load(L"Player\\MeshData\\LG_M_9000.meshdata");
            mMeshDataMap.insert(std::make_pair(LEG, leg));

            std::shared_ptr<MeshData> weapon = std::make_shared<MeshData>();
            weapon->Load(L"Player\\MeshData\\WP_A_0300.meshdata");
            mMeshDataMap.insert(std::make_pair(WEAPON, weapon));

            /*std::shared_ptr<MeshData> waeponCase = std::make_shared<MeshData>();
            waeponCase->Load(L"Player\\MeshData\\WP_A_0300_1_b.meshdata");
            mMeshDataMap.insert(std::make_pair(WEAPONCASE, waeponCase));*/
        }

        if (mbUse)
        {
            LoadAnimation();
        }

        Player* player = dynamic_cast<Player*>(GetOwner());
        Transform* playerTr = player->GetComponent<Transform>();

        for (auto iter = mMeshDataMap.begin(); iter != mMeshDataMap.end(); iter++)
        {
            std::shared_ptr<MeshData> meshData = iter->second;

            Scene* scene = SceneManager::GetScene(eSceneType::Tilte);

            meshData->Instantiate(eLayerType::Player, scene);

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
                std::wstring name = iter->first;
                std::shared_ptr<MeshData> meshData = iter->second;

                // 방향 전환 시작
                meshData->GetAnimationCompleteEvent(L"a000_000010_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_000011_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_000012_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_000013_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
               
                // 빠르게 걷기 시작
                meshData->GetAnimationCompleteEvent(L"a000_000400_" + name) = [meshData, name]() { meshData->Play(L"a000_000500_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_000401_" + name) = [meshData, name]() { meshData->Play(L"a000_000501_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_000402_" + name) = [meshData, name]() { meshData->Play(L"a000_000502_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_000403_" + name) = [meshData, name]() { meshData->Play(L"a000_000503_" + name); };

                //// 빠르게 걷기
                //meshData->GetAnimationEndEvent(L"a000_000500_" + name) = [meshData, name]() { meshData->Play(L"a000_000600_" + name); };
                //meshData->GetAnimationEndEvent(L"a000_000501_" + name) = [meshData, name]() { meshData->Play(L"a000_000601_" + name); };
                //meshData->GetAnimationEndEvent(L"a000_000502_" + name) = [meshData, name]() { meshData->Play(L"a000_000602_" + name); };
                //meshData->GetAnimationEndEvent(L"a000_000503_" + name) = [meshData, name]() { meshData->Play(L"a000_000603_" + name); };

                // 빠르게 걷기 정지
                meshData->GetAnimationCompleteEvent(L"a000_000600_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_000601_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_000602_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_000603_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
               
                // 달리기 정지
                meshData->GetAnimationCompleteEvent(L"a000_001151_" + name) = [meshData, name]() { meshData->Play(L"a000_001200_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_001510_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_001511_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_001512_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };

                meshData->GetAnimationCompleteEvent(L"a000_200000_" + name) = [meshData, name]() { meshData->Play(L"a000_201030_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_201040_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                
                meshData->GetAnimationCompleteEvent(L"a050_308000_" + name) = [meshData, name]() { meshData->Play(L"a000_201030_" + name); };
                meshData->GetAnimationCompleteEvent(L"a050_308010_" + name) = [meshData, name]() { meshData->Play(L"a000_201030_" + name); };
                
                // 웅크리기 걷기 시작
                meshData->GetAnimationCompleteEvent(L"a000_005100_" + name) = [meshData, name]() { meshData->Play(L"a000_005200_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_005101_" + name) = [meshData, name]() { meshData->Play(L"a000_005201_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_005102_" + name) = [meshData, name]() { meshData->Play(L"a000_005202_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_005103_" + name) = [meshData, name]() { meshData->Play(L"a000_005203_" + name); };
                // 웅크리기 정지
                meshData->GetAnimationCompleteEvent(L"a000_005300_" + name) = [meshData, name]() { meshData->Play(L"a000_005000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_005301_" + name) = [meshData, name]() { meshData->Play(L"a000_005000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_005302_" + name) = [meshData, name]() { meshData->Play(L"a000_005000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_005303_" + name) = [meshData, name]() { meshData->Play(L"a000_005000_" + name); };

                // 피격
                meshData->GetAnimationCompleteEvent(L"a000_100100_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_100101_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_100102_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_100103_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };

                // 매달리기
                meshData->GetAnimationCompleteEvent(L"a000_217103_" + name) = [meshData, name]() { meshData->Play(L"a000_020000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_217500_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a000_217620_" + name) = [meshData, name]() { meshData->Play(L"a000_201030_" + name); };


                // 가드 이동 시작
                meshData->GetAnimationCompleteEvent(L"a050_002010_" + name) = [meshData, name]() { meshData->Play(L"a050_002200_" + name); };
                meshData->GetAnimationCompleteEvent(L"a050_002011_" + name) = [meshData, name]() { meshData->Play(L"a050_002201_" + name); };
                meshData->GetAnimationCompleteEvent(L"a050_002012_" + name) = [meshData, name]() { meshData->Play(L"a050_002202_" + name); };
                meshData->GetAnimationCompleteEvent(L"a050_002013_" + name) = [meshData, name]() { meshData->Play(L"a050_002203_" + name); };
                
                // 패링
                meshData->GetAnimationCompleteEvent(L"a050_120100_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a050_120101_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a050_120102_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a050_120103_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
            
                // 공격 패링
                meshData->GetAnimationCompleteEvent(L"a050_130101_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a050_130102_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a050_130200_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a050_130201_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };
                meshData->GetAnimationCompleteEvent(L"a050_130202_" + name) = [meshData, name]() { meshData->Play(L"a000_000000_" + name); };

                // 매달리기
                meshData->GetAnimationCompleteEvent(L"a050_314000_" + name) = [meshData, name]() { meshData->Play(L"a000_020000_" + name); };
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

                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000000.fbx", L"a000_000000");

#pragma region 1차 통합

                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000000.fbx", L"a000_000000_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000010.fbx", L"a000_000010_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000011.fbx", L"a000_000011_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000012.fbx", L"a000_000012_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000013.fbx", L"a000_000013_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000100.fbx", L"a000_000100_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000101.fbx", L"a000_000101_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000102.fbx", L"a000_000102_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000103.fbx", L"a000_000103_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000300.fbx", L"a000_000300_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000301.fbx", L"a000_000301_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000302.fbx", L"a000_000302_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000303.fbx", L"a000_000303_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000400.fbx", L"a000_000400_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000401.fbx", L"a000_000401_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000402.fbx", L"a000_000402_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000403.fbx", L"a000_000403_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000500.fbx", L"a000_000500_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000501.fbx", L"a000_000501_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000502.fbx", L"a000_000502_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000503.fbx", L"a000_000503_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000600.fbx", L"a000_000600_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000601.fbx", L"a000_000601_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000602.fbx", L"a000_000602_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000603.fbx", L"a000_000603_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001151.fbx", L"a000_001151_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001152.fbx", L"a000_001152_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001153.fbx", L"a000_001153_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001154.fbx", L"a000_001154_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001200.fbx", L"a000_001200_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001402.fbx", L"a000_001402_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001403.fbx", L"a000_001403_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001510.fbx", L"a000_001510_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001511.fbx", L"a000_001511_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001512.fbx", L"a000_001512_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005000.fbx", L"a000_005000_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005100.fbx", L"a000_005100_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005101.fbx", L"a000_005101_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005102.fbx", L"a000_005102_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005103.fbx", L"a000_005103_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005110.fbx", L"a000_005110_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005111.fbx", L"a000_005111_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005112.fbx", L"a000_005112_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005113.fbx", L"a000_005113_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005200.fbx", L"a000_005200_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005201.fbx", L"a000_005201_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005202.fbx", L"a000_005202_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005203.fbx", L"a000_005203_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005300.fbx", L"a000_005300_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005301.fbx", L"a000_005301_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005302.fbx", L"a000_005302_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005303.fbx", L"a000_005303_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020000.fbx", L"a000_020000_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020102.fbx", L"a000_020102_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020103.fbx", L"a000_020103_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021000.fbx", L"a000_021000_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021102.fbx", L"a000_021102_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021103.fbx", L"a000_021103_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021202.fbx", L"a000_021202_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021203.fbx", L"a000_021203_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021302.fbx", L"a000_021302_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021303.fbx", L"a000_021303_" + name);
                //meshData->AnimationSave(L"Player\\AnimationData\\a000_0xxxxx_" + name + L".animationdata");


                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100100.fbx", L"a000_100100_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100101.fbx", L"a000_100101_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100102.fbx", L"a000_100102_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100103.fbx", L"a000_100103_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100200.fbx", L"a000_100200_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100200.fbx", L"a000_100200_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100201.fbx", L"a000_100201_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100201.fbx", L"a000_100201_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100202.fbx", L"a000_100202_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100202.fbx", L"a000_100202_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100203.fbx", L"a000_100203_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100203.fbx", L"a000_100203_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100300.fbx", L"a000_100300_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100300.fbx", L"a000_100300_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100301.fbx", L"a000_100301_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100301.fbx", L"a000_100301_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100310.fbx", L"a000_100310_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100310.fbx", L"a000_100310_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100311.fbx", L"a000_100311_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100311.fbx", L"a000_100311_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100320.fbx", L"a000_100320_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100320.fbx", L"a000_100320_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100320.fbx", L"a000_100320_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100321.fbx", L"a000_100321_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100321.fbx", L"a000_100321_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100321.fbx", L"a000_100321_" + name);
                //meshData->AnimationSave(L"Player\\AnimationData\\a000_1xxxxx_" + name + L".animationdata");


                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200000.fbx", L"a000_200000_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200903.fbx", L"a000_200903_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200907.fbx", L"a000_200907_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200907.fbx", L"a000_200907_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200908.fbx", L"a000_200908_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200908.fbx", L"a000_200908_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201010.fbx", L"a000_201010_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201011.fbx", L"a000_201011_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201030.fbx", L"a000_201030_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201040.fbx", L"a000_201040_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201045.fbx", L"a000_201045_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201210.fbx", L"a000_201210_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202000.fbx", L"a000_202000_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202000.fbx", L"a000_202000_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202010.fbx", L"a000_202010_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202010.fbx", L"a000_202010_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202011.fbx", L"a000_202011_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202011.fbx", L"a000_202011_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202012.fbx", L"a000_202012_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202012.fbx", L"a000_202012_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202035.fbx", L"a000_202035_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202035.fbx", L"a000_202035_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202100.fbx", L"a000_202100_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202100.fbx", L"a000_202100_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202110.fbx", L"a000_202110_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202110.fbx", L"a000_202110_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202300.fbx", L"a000_202300_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202300.fbx", L"a000_202300_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202310.fbx", L"a000_202310_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202310.fbx", L"a000_202310_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202400.fbx", L"a000_202400_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202410.fbx", L"a000_202410_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202600.fbx", L"a000_202600_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202610.fbx", L"a000_202610_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202700.fbx", L"a000_202700_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202710.fbx", L"a000_202710_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_206100.fbx", L"a000_206100_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_206101.fbx", L"a000_206101_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217102.fbx", L"a000_217102_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217103.fbx", L"a000_217103_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217500.fbx", L"a000_217500_" + name);
                //meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217620.fbx", L"a000_217620_" + name);
               // meshData->AnimationSave(L"Player\\AnimationData\\a000_2xxxxx_" + name + L".animationdata");


                /*meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002000.fbx", L"a050_002000_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002010.fbx", L"a050_002010_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002011.fbx", L"a050_002011_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002012.fbx", L"a050_002012_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002013.fbx", L"a050_002013_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002200.fbx", L"a050_002200_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002201.fbx", L"a050_002201_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002202.fbx", L"a050_002202_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002203.fbx", L"a050_002203_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120100.fbx", L"a050_120100_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120101.fbx", L"a050_120101_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120102.fbx", L"a050_120102_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120103.fbx", L"a050_120103_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120104.fbx", L"a050_120104_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120105.fbx", L"a050_120105_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120301.fbx", L"a050_120301_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120302.fbx", L"a050_120302_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120700.fbx", L"a050_120700_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130100.fbx", L"a050_130100_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130101.fbx", L"a050_130101_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130102.fbx", L"a050_130102_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130200.fbx", L"a050_130200_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130201.fbx", L"a050_130201_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130202.fbx", L"a050_130202_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130300.fbx", L"a050_130300_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130301.fbx", L"a050_130301_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130302.fbx", L"a050_130302_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130700.fbx", L"a050_130700_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_139900.fbx", L"a050_139900_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_150000.fbx", L"a050_150000_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_150001.fbx", L"a050_150001_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190100.fbx", L"a050_190100_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190155.fbx", L"a050_190155_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190500.fbx", L"a050_190500_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190501.fbx", L"a050_190501_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190502.fbx", L"a050_190502_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203010.fbx", L"a050_203010_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_206200.fbx", L"a050_206200_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_206201.fbx", L"a050_206201_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_206202.fbx", L"a050_206202_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_206203.fbx", L"a050_206203_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300000.fbx", L"a050_300000_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300001.fbx", L"a050_300001_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300020.fbx", L"a050_300020_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300030.fbx", L"a050_300030_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300040.fbx", L"a050_300040_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300100.fbx", L"a050_300100_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_301050.fbx", L"a050_301050_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_305000.fbx", L"a050_305000_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_305001.fbx", L"a050_305001_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_305101.fbx", L"a050_305101_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308000.fbx", L"a050_308000_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308010.fbx", L"a050_308010_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308020.fbx", L"a050_308020_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308050.fbx", L"a050_308050_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308060.fbx", L"a050_308060_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308070.fbx", L"a050_308070_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_314000.fbx", L"a050_314000_" + name);
                meshData->AnimationSave(L"Player\\AnimationData\\a050_xxxxxx_" + name + L".animationdata");*/


                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510200.fbx", L"a200_510200_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510210.fbx", L"a200_510210_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510220.fbx", L"a200_510220_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510230.fbx", L"a200_510230_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510600.fbx", L"a200_510600_" + name);
                meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510700.fbx", L"a200_510700_" + name);
                meshData->AnimationSave(L"Player\\AnimationData\\a200_xxxxxx_" + name + L".animationdata");

#pragma endregion
           
             }
        }
        else
        {
            for (auto iter = mMeshDataMap.begin(); iter != mMeshDataMap.end(); iter++)
            {
                std::wstring name = iter->first;
                std::shared_ptr<MeshData> meshData = iter->second;

                meshData->AnimationLoad(L"Player\\AnimationData\\a000_0xxxxx_" + name + L".animationdata", nullptr, false);
                meshData->AnimationLoad(L"Player\\AnimationData\\a000_1xxxxx_" + name + L".animationdata", nullptr, false);
                meshData->AnimationLoad(L"Player\\AnimationData\\a000_2xxxxx_" + name + L".animationdata", nullptr, false);
                meshData->AnimationLoad(L"Player\\AnimationData\\a050_xxxxxx_" + name + L".animationdata", nullptr, false);
                meshData->AnimationLoad(L"Player\\AnimationData\\a200_xxxxxx_" + name + L".animationdata");
            }
        }
    }
    void PlayerMeshScript::Play(std::wstring name)
    {
        if (!mbUse)
            return;

        for (auto iter = mMeshDataMap.begin(); iter != mMeshDataMap.end(); iter++)
        {
            std::wstring part = iter->first;
            std::shared_ptr<MeshData> meshData = iter->second;
            meshData->Play(name + L"_" + part);
        }
    }

    std::shared_ptr<MeshData> PlayerMeshScript::FindMeshData(std::wstring name)
    {
       if (mMeshDataMap.find(name) == mMeshDataMap.end()) 
           return nullptr; 
       return mMeshDataMap.find(name)->second;
    }
}
