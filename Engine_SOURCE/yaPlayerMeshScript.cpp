#include "yaPlayerMeshScript.h"
#include "yaPlayer.h"
#include "yaMeshData.h"
#include "yaInput.h"

namespace ya
{
	PlayerMeshScript::PlayerMeshScript()
		: Script()
		, mbLoad(true)
		, mMeshDataMap{}
	{
	}

	PlayerMeshScript::~PlayerMeshScript()
	{
	}

	void PlayerMeshScript::Initialize()
	{
		if(mbLoad)
		{
			std::shared_ptr<MeshData> arm = MeshData::LoadFromFbx(L"Player\\Mesh\\AM_M_9000.fbx");
			mMeshDataMap.insert(std::make_pair(L"Arm", arm));

			std::shared_ptr<MeshData> body = MeshData::LoadFromFbx(L"Player\\Mesh\\BD_M_9000.fbx");
			mMeshDataMap.insert(std::make_pair(L"Body", body));

			std::shared_ptr<MeshData> face = MeshData::LoadFromFbx(L"Player\\Mesh\\FC_M_0100.fbx");
			mMeshDataMap.insert(std::make_pair(L"Face", face));

			std::shared_ptr<MeshData> leg = MeshData::LoadFromFbx(L"Player\\Mesh\\LG_M_9000.fbx");
			mMeshDataMap.insert(std::make_pair(L"Leg", leg));

			LoadAnimation();
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
		}

		for (auto iter = mMeshDataMap.begin(); iter != mMeshDataMap.end(); iter++)
		{
			iter->second->Instantiate(eLayerType::Player);
		}

		//Play(L"a000_000000");
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

#pragma region a000_000000 ~ a000_0015xx 걷기, 달리기
				/*
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000000.fbx", L"a000_000000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000010.fbx", L"a000_000010");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000011.fbx", L"a000_000011");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000012.fbx", L"a000_000012");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000013.fbx", L"a000_000013");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000100.fbx", L"a000_000100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000101.fbx", L"a000_000101");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000102.fbx", L"a000_000102");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000103.fbx", L"a000_000103");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000110.fbx", L"a000_000110");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000111.fbx", L"a000_000111");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000112.fbx", L"a000_000112");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000113.fbx", L"a000_000113");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000120.fbx", L"a000_000120");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000121.fbx", L"a000_000121");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000122.fbx", L"a000_000122");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000123.fbx", L"a000_000123");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000132.fbx", L"a000_000132");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000133.fbx", L"a000_000133");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000200.fbx", L"a000_000200");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000201.fbx", L"a000_000201");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000202.fbx", L"a000_000202");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000203.fbx", L"a000_000203");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000300.fbx", L"a000_000300");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000301.fbx", L"a000_000301");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000302.fbx", L"a000_000302");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000303.fbx", L"a000_000303");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000400.fbx", L"a000_000400");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000401.fbx", L"a000_000401");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000402.fbx", L"a000_000402");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000403.fbx", L"a000_000403");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000410.fbx", L"a000_000410");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000411.fbx", L"a000_000411");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000412.fbx", L"a000_000412");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000413.fbx", L"a000_000413");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000420.fbx", L"a000_000420");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000421.fbx", L"a000_000421");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000422.fbx", L"a000_000422");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000423.fbx", L"a000_000423");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000432.fbx", L"a000_000432");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000433.fbx", L"a000_000433");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000442.fbx", L"a000_000442");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_000443.fbx", L"a000_000443");
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
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001402.fbx", L"a000_001402");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001403.fbx", L"a000_001403");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001410.fbx", L"a000_001410");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001500.fbx", L"a000_001500");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001510.fbx", L"a000_001510");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001511.fbx", L"a000_001511");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_001512.fbx", L"a000_001512");
				meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a000_0015xx.animationdata");
				*/
#pragma endregion

#pragma region a000_005xxx 웅크린 걷기
				/*meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005100.fbx", L"a000_005100");
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
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005400.fbx", L"a000_005400");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005401.fbx", L"a000_005401");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005402.fbx", L"a000_005402");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005403.fbx", L"a000_005403");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005410.fbx", L"a000_005410");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005411.fbx", L"a000_005411");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005412.fbx", L"a000_005412");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005413.fbx", L"a000_005413");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005500.fbx", L"a000_005500");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005501.fbx", L"a000_005501");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005502.fbx", L"a000_005502");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005503.fbx", L"a000_005503");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005600.fbx", L"a000_005600");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005601.fbx", L"a000_005601");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005602.fbx", L"a000_005602");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_005603.fbx", L"a000_005603");
				meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a000_005xxx.animationdata");
				*/
#pragma endregion
				
#pragma region a000_002xxx 매달리기, 벽		
				/*meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020000.fbx", L"a000_020000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020010.fbx", L"a000_020010");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020102.fbx", L"a000_020102");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020103.fbx", L"a000_020103");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020112.fbx", L"a000_020112");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020113.fbx", L"a000_020113");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020202.fbx", L"a000_020202");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020203.fbx", L"a000_020203");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020212.fbx", L"a000_020212");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020213.fbx", L"a000_020213");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020302.fbx", L"a000_020302");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020303.fbx", L"a000_020303");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020312.fbx", L"a000_020312");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020313.fbx", L"a000_020313");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020402.fbx", L"a000_020402");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020403.fbx", L"a000_020403");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020412.fbx", L"a000_020412");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020413.fbx", L"a000_020413");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020422.fbx", L"a000_020422");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020423.fbx", L"a000_020423");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020432.fbx", L"a000_020432");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_020433.fbx", L"a000_020433");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021000.fbx", L"a000_021000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021102.fbx", L"a000_021102");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021103.fbx", L"a000_021103");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021202.fbx", L"a000_021202");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021203.fbx", L"a000_021203");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021302.fbx", L"a000_021302");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021303.fbx", L"a000_021303");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021402.fbx", L"a000_021402");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021403.fbx", L"a000_021403");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021502.fbx", L"a000_021502");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021503.fbx", L"a000_021503");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021602.fbx", L"a000_021602");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_021603.fbx", L"a000_021603");
				meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a000_02xxxx.animationdata");
				*/
#pragma endregion

#pragma region a000_10xxxx 피격, 사망
				/*
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100000.fbx", L"a000_100000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100001.fbx", L"a000_100001");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100002.fbx", L"a000_100002");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100003.fbx", L"a000_100003");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100100.fbx", L"a000_100100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100101.fbx", L"a000_100101");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100102.fbx", L"a000_100102");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100103.fbx", L"a000_100103");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100200.fbx", L"a000_100200");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100201.fbx", L"a000_100201");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100202.fbx", L"a000_100202");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100203.fbx", L"a000_100203");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100300.fbx", L"a000_100300");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100301.fbx", L"a000_100301");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100310.fbx", L"a000_100310");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100311.fbx", L"a000_100311");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100320.fbx", L"a000_100320");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100321.fbx", L"a000_100321");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100400.fbx", L"a000_100400");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100401.fbx", L"a000_100401");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100410.fbx", L"a000_100410");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100411.fbx", L"a000_100411");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100420.fbx", L"a000_100420");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100421.fbx", L"a000_100421");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100500.fbx", L"a000_100500");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100600.fbx", L"a000_100600");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100610.fbx", L"a000_100610");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100620.fbx", L"a000_100620");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100800.fbx", L"a000_100800");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_100801.fbx", L"a000_100801");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_101100.fbx", L"a000_101100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_101101.fbx", L"a000_101101");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_101300.fbx", L"a000_101300");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_101301.fbx", L"a000_101301");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102000.fbx", L"a000_102000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102100.fbx", L"a000_102100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102101.fbx", L"a000_102101");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102110.fbx", L"a000_102110");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102111.fbx", L"a000_102111");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102300.fbx", L"a000_102300");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102301.fbx", L"a000_102301");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102310.fbx", L"a000_102310");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102311.fbx", L"a000_102311");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102320.fbx", L"a000_102320");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102321.fbx", L"a000_102321");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102390.fbx", L"a000_102390");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102391.fbx", L"a000_102391");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102392.fbx", L"a000_102392");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102500.fbx", L"a000_102500");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102510.fbx", L"a000_102510");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102520.fbx", L"a000_102520");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102900.fbx", L"a000_102900");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102910.fbx", L"a000_102910");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102930.fbx", L"a000_102930");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_102940.fbx", L"a000_102940");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_103000.fbx", L"a000_103000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_103100.fbx", L"a000_103100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_103101.fbx", L"a000_103101");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_103300.fbx", L"a000_103300");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_103301.fbx", L"a000_103301");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_104100.fbx", L"a000_104100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_104101.fbx", L"a000_104101");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_104300.fbx", L"a000_104300");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_104301.fbx", L"a000_104301");

				meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a000_10xxxx.animationdata");
			*/
#pragma endregion

#pragma region a000_11xxxx 피격, 사망, 부활
	/*
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_110000.fbx", L"a000_110000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_110001.fbx", L"a000_110001");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_110010.fbx", L"a000_110010");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_110011.fbx", L"a000_110011");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_110030.fbx", L"a000_110030");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_110031.fbx", L"a000_110031");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_111000.fbx", L"a000_111000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_111001.fbx", L"a000_111001");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_111010.fbx", L"a000_111010");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_111011.fbx", L"a000_111011");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_111030.fbx", L"a000_111030");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_111031.fbx", L"a000_111031");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_112020.fbx", L"a000_112020");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_112021.fbx", L"a000_112021");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_113000.fbx", L"a000_113000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_113010.fbx", L"a000_113010");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_113030.fbx", L"a000_113030");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_114000.fbx", L"a000_114000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_114010.fbx", L"a000_114010");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_114030.fbx", L"a000_114030");

			meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a000_11xxxx.animationdata");
		*/
#pragma endregion

#pragma region a000_19xxxx 피격, 사망, 부활
		/*
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_190000.fbx", L"a000_190000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_190001.fbx", L"a000_190001");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_190010.fbx", L"a000_190010");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_190011.fbx", L"a000_190011");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_190030.fbx", L"a000_190030");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_190031.fbx", L"a000_190031");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_190040.fbx", L"a000_190040");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_190041.fbx", L"a000_190041");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_190050.fbx", L"a000_190050");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_190055.fbx", L"a000_190055");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_191000.fbx", L"a000_191000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_191100.fbx", L"a000_191100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_191200.fbx", L"a000_191200");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_191400.fbx", L"a000_191400");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_191500.fbx", L"a000_191500");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_191510.fbx", L"a000_191510");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_191511.fbx", L"a000_191511");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_191600.fbx", L"a000_191600");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192400.fbx", L"a000_192400");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192410.fbx", L"a000_192410");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192420.fbx", L"a000_192420");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192421.fbx", L"a000_192421");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192430.fbx", L"a000_192430");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192500.fbx", L"a000_192500");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192510.fbx", L"a000_192510");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192540.fbx", L"a000_192540");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192550.fbx", L"a000_192550");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192560.fbx", L"a000_192560");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192570.fbx", L"a000_192570");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192600.fbx", L"a000_192600");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192610.fbx", L"a000_192610");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192611.fbx", L"a000_192611");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192700.fbx", L"a000_192700");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192701.fbx", L"a000_192701");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192900.fbx", L"a000_192900");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192910.fbx", L"a000_192910");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192911.fbx", L"a000_192911");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192920.fbx", L"a000_192920");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192930.fbx", L"a000_192930");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192931.fbx", L"a000_192931");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192950.fbx", L"a000_192950");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_192970.fbx", L"a000_192970");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_193000.fbx", L"a000_193000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_193020.fbx", L"a000_193020");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_193050.fbx", L"a000_193050");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_193070.fbx", L"a000_193070");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_199201.fbx", L"a000_199201");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_199211.fbx", L"a000_199211");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_199300.fbx", L"a000_199300");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_199301.fbx", L"a000_199301");

			meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a000_19xxxx.animationdata");
		*/
#pragma endregion

#pragma region a000_200xxx 피격, 사망, 부활
	/*
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200000.fbx", L"a000_200000");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200021.fbx", L"a000_200021");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200100.fbx", L"a000_200100");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200120.fbx", L"a000_200120");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200121.fbx", L"a000_200121");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200903.fbx", L"a000_200903");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200907.fbx", L"a000_200907");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_200908.fbx", L"a000_200908");

		meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a000_200xxx.animationdata");
	*/

#pragma endregion

#pragma region a000_201xxx 점프, 구르기, 착지, 
			/*
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201000.fbx", L"a000_201000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201001.fbx", L"a000_201001");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201010.fbx", L"a000_201010");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201011.fbx", L"a000_201011");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201030.fbx", L"a000_201030");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201040.fbx", L"a000_201040");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201045.fbx", L"a000_201045");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201050.fbx", L"a000_201050");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201055.fbx", L"a000_201055");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201100.fbx", L"a000_201100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201101.fbx", L"a000_201101");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201102.fbx", L"a000_201102");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201103.fbx", L"a000_201103");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201104.fbx", L"a000_201104");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201105.fbx", L"a000_201105");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201106.fbx", L"a000_201106");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201107.fbx", L"a000_201107");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201110.fbx", L"a000_201110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201111.fbx", L"a000_201111");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201112.fbx", L"a000_201112");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201113.fbx", L"a000_201113");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201114.fbx", L"a000_201114");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201115.fbx", L"a000_201115");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201116.fbx", L"a000_201116");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201117.fbx", L"a000_201117");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201140.fbx", L"a000_201140");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201141.fbx", L"a000_201141");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201142.fbx", L"a000_201142");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201143.fbx", L"a000_201143");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201144.fbx", L"a000_201144");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201145.fbx", L"a000_201145");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201146.fbx", L"a000_201146");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201147.fbx", L"a000_201147");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201200.fbx", L"a000_201200");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201210.fbx", L"a000_201210");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201300.fbx", L"a000_201300");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201301.fbx", L"a000_201301");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201302.fbx", L"a000_201302");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201303.fbx", L"a000_201303");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201304.fbx", L"a000_201304");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201305.fbx", L"a000_201305");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201320.fbx", L"a000_201320");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201321.fbx", L"a000_201321");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201340.fbx", L"a000_201340");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201341.fbx", L"a000_201341");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201350.fbx", L"a000_201350");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201360.fbx", L"a000_201360");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201411.fbx", L"a000_201411");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201461.fbx", L"a000_201461");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201500.fbx", L"a000_201500");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201501.fbx", L"a000_201501");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201510.fbx", L"a000_201510");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201511.fbx", L"a000_201511");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201600.fbx", L"a000_201600");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201610.fbx", L"a000_201610");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201640.fbx", L"a000_201640");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201660.fbx", L"a000_201660");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201700.fbx", L"a000_201700");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201701.fbx", L"a000_201701");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201800.fbx", L"a000_201800");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201801.fbx", L"a000_201801");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201802.fbx", L"a000_201802");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201803.fbx", L"a000_201803");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201804.fbx", L"a000_201804");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201805.fbx", L"a000_201805");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201806.fbx", L"a000_201806");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_201807.fbx", L"a000_201807");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202000.fbx", L"a000_202000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202010.fbx", L"a000_202010");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202011.fbx", L"a000_202011");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202012.fbx", L"a000_202012");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202035.fbx", L"a000_202035");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202100.fbx", L"a000_202100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202110.fbx", L"a000_202110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202112.fbx", L"a000_202112");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202300.fbx", L"a000_202300");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202310.fbx", L"a000_202310");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202400.fbx", L"a000_202400");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202410.fbx", L"a000_202410");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202600.fbx", L"a000_202600");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202610.fbx", L"a000_202610");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202700.fbx", L"a000_202700");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_202710.fbx", L"a000_202710");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_205010.fbx", L"a000_205010");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_205011.fbx", L"a000_205011");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_205020.fbx", L"a000_205020");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_205021.fbx", L"a000_205021");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_205030.fbx", L"a000_205030");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_205031.fbx", L"a000_205031");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_206100.fbx", L"a000_206100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_206101.fbx", L"a000_206101");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_206110.fbx", L"a000_206110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_206111.fbx", L"a000_206111");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_206112.fbx", L"a000_206112");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_206113.fbx", L"a000_206113");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_206120.fbx", L"a000_206120");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_206121.fbx", L"a000_206121");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_206122.fbx", L"a000_206122");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_206123.fbx", L"a000_206123");


			meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a000_20xxxx.animationdata");
		*/
#pragma endregion

#pragma region a000_21xxxx 회피기, 매달리기, 벽
			/*
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_210000.fbx", L"a000_210000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_210001.fbx", L"a000_210001");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_210010.fbx", L"a000_210010");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_210011.fbx", L"a000_210011");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_210012.fbx", L"a000_210012");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_210018.fbx", L"a000_210018");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_210050.fbx", L"a000_210050");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_210051.fbx", L"a000_210051");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_210100.fbx", L"a000_210100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_210150.fbx", L"a000_210150");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_210160.fbx", L"a000_210160");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_210161.fbx", L"a000_210161");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_210300.fbx", L"a000_210300");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_213100.fbx", L"a000_213100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_213110.fbx", L"a000_213110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_213111.fbx", L"a000_213111");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_213301.fbx", L"a000_213301");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_213302.fbx", L"a000_213302");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_213303.fbx", L"a000_213303");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_213304.fbx", L"a000_213304");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_216000.fbx", L"a000_216000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_216010.fbx", L"a000_216010");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_216020.fbx", L"a000_216020");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_216021.fbx", L"a000_216021");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_216100.fbx", L"a000_216100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217100.fbx", L"a000_217100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217102.fbx", L"a000_217102");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217103.fbx", L"a000_217103");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217106.fbx", L"a000_217106");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217107.fbx", L"a000_217107");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217120.fbx", L"a000_217120");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217121.fbx", L"a000_217121");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217122.fbx", L"a000_217122");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217130.fbx", L"a000_217130");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217131.fbx", L"a000_217131");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217132.fbx", L"a000_217132");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217202.fbx", L"a000_217202");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217203.fbx", L"a000_217203");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217206.fbx", L"a000_217206");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217207.fbx", L"a000_217207");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217220.fbx", L"a000_217220");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217221.fbx", L"a000_217221");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217222.fbx", L"a000_217222");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217230.fbx", L"a000_217230");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217231.fbx", L"a000_217231");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217232.fbx", L"a000_217232");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217500.fbx", L"a000_217500");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217520.fbx", L"a000_217520");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217600.fbx", L"a000_217600");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217620.fbx", L"a000_217620");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217700.fbx", L"a000_217700");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217710.fbx", L"a000_217710");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_217720.fbx", L"a000_217720");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_218103.fbx", L"a000_218103");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_218104.fbx", L"a000_218104");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_218105.fbx", L"a000_218105");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_218106.fbx", L"a000_218106");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_218107.fbx", L"a000_218107");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_218111.fbx", L"a000_218111");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_218112.fbx", L"a000_218112");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_218490.fbx", L"a000_218490");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_219000.fbx", L"a000_219000");

			meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a000_21xxxx.animationdata");
		*/
#pragma endregion

#pragma region a000_22xxxx
			/*
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220000.fbx", L"a000_220000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220001.fbx", L"a000_220001");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220002.fbx", L"a000_220002");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220003.fbx", L"a000_220003");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220004.fbx", L"a000_220004");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220005.fbx", L"a000_220005");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220006.fbx", L"a000_220006");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220007.fbx", L"a000_220007");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220010.fbx", L"a000_220010");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220011.fbx", L"a000_220011");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220012.fbx", L"a000_220012");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220013.fbx", L"a000_220013");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220014.fbx", L"a000_220014");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220015.fbx", L"a000_220015");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220016.fbx", L"a000_220016");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220017.fbx", L"a000_220017");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220020.fbx", L"a000_220020");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220021.fbx", L"a000_220021");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220022.fbx", L"a000_220022");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220023.fbx", L"a000_220023");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220024.fbx", L"a000_220024");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220025.fbx", L"a000_220025");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220026.fbx", L"a000_220026");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_220027.fbx", L"a000_220027");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_226010.fbx", L"a000_226010");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_226100.fbx", L"a000_226100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_226101.fbx", L"a000_226101");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_226110.fbx", L"a000_226110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_226111.fbx", L"a000_226111");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_226120.fbx", L"a000_226120");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_226130.fbx", L"a000_226130");

			meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a000_22xxxx.animationdata");
		*/
#pragma endregion

#pragma region a000_25xxxx
/*
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250000.fbx", L"a000_250000");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250001.fbx", L"a000_250001");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250005.fbx", L"a000_250005");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250006.fbx", L"a000_250006");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250010.fbx", L"a000_250010");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250011.fbx", L"a000_250011");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250015.fbx", L"a000_250015");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250016.fbx", L"a000_250016");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250030.fbx", L"a000_250030");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250031.fbx", L"a000_250031");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250035.fbx", L"a000_250035");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250036.fbx", L"a000_250036");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250040.fbx", L"a000_250040");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250041.fbx", L"a000_250041");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250045.fbx", L"a000_250045");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250046.fbx", L"a000_250046");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250100.fbx", L"a000_250100");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250110.fbx", L"a000_250110");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250200.fbx", L"a000_250200");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250201.fbx", L"a000_250201");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250210.fbx", L"a000_250210");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250211.fbx", L"a000_250211");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250300.fbx", L"a000_250300");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250310.fbx", L"a000_250310");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250400.fbx", L"a000_250400");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250500.fbx", L"a000_250500");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250600.fbx", L"a000_250600");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250610.fbx", L"a000_250610");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250700.fbx", L"a000_250700");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250701.fbx", L"a000_250701");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250800.fbx", L"a000_250800");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_250900.fbx", L"a000_250900");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251000.fbx", L"a000_251000");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251100.fbx", L"a000_251100");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251200.fbx", L"a000_251200");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251300.fbx", L"a000_251300");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251400.fbx", L"a000_251400");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251410.fbx", L"a000_251410");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251440.fbx", L"a000_251440");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251441.fbx", L"a000_251441");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251460.fbx", L"a000_251460");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251461.fbx", L"a000_251461");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251500.fbx", L"a000_251500");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251510.fbx", L"a000_251510");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251530.fbx", L"a000_251530");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251540.fbx", L"a000_251540");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251550.fbx", L"a000_251550");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251600.fbx", L"a000_251600");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251610.fbx", L"a000_251610");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251700.fbx", L"a000_251700");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251710.fbx", L"a000_251710");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251800.fbx", L"a000_251800");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251900.fbx", L"a000_251900");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251901.fbx", L"a000_251901");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251902.fbx", L"a000_251902");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251910.fbx", L"a000_251910");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251911.fbx", L"a000_251911");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_251912.fbx", L"a000_251912");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_252000.fbx", L"a000_252000");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_252010.fbx", L"a000_252010");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_259000.fbx", L"a000_259000");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_259010.fbx", L"a000_259010");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_259440.fbx", L"a000_259440");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_259441.fbx", L"a000_259441");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_259460.fbx", L"a000_259460");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_259461.fbx", L"a000_259461");


				meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a000_25xxxx.animationdata");
				*/
#pragma endregion

#pragma region a000_299xxx
		/*
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_299000.fbx", L"a000_299000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_299020.fbx", L"a000_299020");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_299200.fbx", L"a000_299200");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_299300.fbx", L"a000_299300");

				meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a000_25xxxx.animationdata");
		*/
#pragma endregion

#pragma region a000_412090
		/*
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_412090.fbx", L"a000_412090");
				meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"a000_412090.animationdata");
		*/
#pragma endregion

#pragma region	a000_700xxx
				/*
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700010.fbx", L"a000_700010");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700200.fbx", L"a000_700200");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700230.fbx", L"a000_700230");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700240.fbx", L"a000_700240");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700260.fbx", L"a000_700260");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700270.fbx", L"a000_700270");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700280.fbx", L"a000_700280");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700290.fbx", L"a000_700290");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700300.fbx", L"a000_700300");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700310.fbx", L"a000_700310");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700320.fbx", L"a000_700320");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700330.fbx", L"a000_700330");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700400.fbx", L"a000_700400");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700402.fbx", L"a000_700402");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700403.fbx", L"a000_700403");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700410.fbx", L"a000_700410");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700420.fbx", L"a000_700420");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700422.fbx", L"a000_700422");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700423.fbx", L"a000_700423");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700500.fbx", L"a000_700500");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700501.fbx", L"a000_700501");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700510.fbx", L"a000_700510");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_700511.fbx", L"a000_700511");

				meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"a000_700xxx.animationdata");
				*/
#pragma endregion

#pragma region	a000_71xxxx
			/*
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710000.fbx", L"a000_710000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710100.fbx", L"a000_710100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710101.fbx", L"a000_710101");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710102.fbx", L"a000_710102");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710200.fbx", L"a000_710200");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710201.fbx", L"a000_710201");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710203.fbx", L"a000_710203");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710310.fbx", L"a000_710310");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710320.fbx", L"a000_710320");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710330.fbx", L"a000_710330");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710340.fbx", L"a000_710340");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710360.fbx", L"a000_710360");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710370.fbx", L"a000_710370");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710400.fbx", L"a000_710400");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710410.fbx", L"a000_710410");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710420.fbx", L"a000_710420");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710430.fbx", L"a000_710430");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710500.fbx", L"a000_710500");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710510.fbx", L"a000_710510");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710520.fbx", L"a000_710520");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710530.fbx", L"a000_710530");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710600.fbx", L"a000_710600");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710700.fbx", L"a000_710700");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710800.fbx", L"a000_710800");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_710900.fbx", L"a000_710900");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711010.fbx", L"a000_711010");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711020.fbx", L"a000_711020");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711100.fbx", L"a000_711100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711101.fbx", L"a000_711101");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711110.fbx", L"a000_711110");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711200.fbx", L"a000_711200");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711210.fbx", L"a000_711210");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711211.fbx", L"a000_711211");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711212.fbx", L"a000_711212");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711221.fbx", L"a000_711221");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711300.fbx", L"a000_711300");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711310.fbx", L"a000_711310");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711311.fbx", L"a000_711311");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711312.fbx", L"a000_711312");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711315.fbx", L"a000_711315");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711320.fbx", L"a000_711320");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711321.fbx", L"a000_711321");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711322.fbx", L"a000_711322");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711330.fbx", L"a000_711330");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711331.fbx", L"a000_711331");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711332.fbx", L"a000_711332");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711333.fbx", L"a000_711333");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711400.fbx", L"a000_711400");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711500.fbx", L"a000_711500");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711510.fbx", L"a000_711510");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_711610.fbx", L"a000_711610");

				meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"a000_71xxxx.animationdata");
				*/
#pragma endregion

#pragma region	a000_790xxx
			/*
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790000.fbx", L"a000_790000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790010.fbx", L"a000_790010");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790020.fbx", L"a000_790020");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790030.fbx", L"a000_790030");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790040.fbx", L"a000_790040");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790050.fbx", L"a000_790050");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790060.fbx", L"a000_790060");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790070.fbx", L"a000_790070");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790080.fbx", L"a000_790080");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790090.fbx", L"a000_790090");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790100.fbx", L"a000_790100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790110.fbx", L"a000_790110");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790120.fbx", L"a000_790120");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790130.fbx", L"a000_790130");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790140.fbx", L"a000_790140");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790150.fbx", L"a000_790150");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790500.fbx", L"a000_790500");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790510.fbx", L"a000_790510");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a000_790520.fbx", L"a000_790520");


				meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"a000_79xxxx.animationdata");
			*/
#pragma endregion

#pragma region	a010_xxxxxx
			/*
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_000100.fbx", L"a010_000100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_000110.fbx", L"a010_000110");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_000120.fbx", L"a010_000120");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_000132.fbx", L"a010_000132");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_000133.fbx", L"a010_000133");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_000200.fbx", L"a010_000200");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_000400.fbx", L"a010_000400");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_000410.fbx", L"a010_000410");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_000420.fbx", L"a010_000420");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_000432.fbx", L"a010_000432");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_000433.fbx", L"a010_000433");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_000442.fbx", L"a010_000442");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_000443.fbx", L"a010_000443");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_000500.fbx", L"a010_000500");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_250500.fbx", L"a010_250500");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_251700.fbx", L"a010_251700");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_251710.fbx", L"a010_251710");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_700240.fbx", L"a010_700240");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_700250.fbx", L"a010_700250");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_710330.fbx", L"a010_710330");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a010_710800.fbx", L"a010_710800");

				meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a010_xxxxxx.animationdata");
			*/
#pragma endregion

#pragma region a050_0xxxxx 가드
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002000.fbx", L"a050_002000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002010.fbx", L"a050_002010");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002011.fbx", L"a050_002011");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002012.fbx", L"a050_002012");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002013.fbx", L"a050_002013");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002201.fbx", L"a050_002201");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_002203.fbx", L"a050_002203");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_032000.fbx", L"a050_032000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_042000.fbx", L"a050_042000");

			meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a050_0xxxxx.animationdata");
#pragma endregion

#pragma region	a050_1xxxxx 가드, 베기
				/*
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120000.fbx", L"a050_120000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120100.fbx", L"a050_120100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120101.fbx", L"a050_120101");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120102.fbx", L"a050_120102");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120103.fbx", L"a050_120103");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120104.fbx", L"a050_120104");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120105.fbx", L"a050_120105");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120200.fbx", L"a050_120200");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120201.fbx", L"a050_120201");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120202.fbx", L"a050_120202");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120203.fbx", L"a050_120203");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120205.fbx", L"a050_120205");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120300.fbx", L"a050_120300");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120301.fbx", L"a050_120301");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120302.fbx", L"a050_120302");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_120700.fbx", L"a050_120700");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_122101.fbx", L"a050_122101");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_122102.fbx", L"a050_122102");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_122111.fbx", L"a050_122111");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_122112.fbx", L"a050_122112");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_122302.fbx", L"a050_122302");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_122312.fbx", L"a050_122312");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_122700.fbx", L"a050_122700");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_122710.fbx", L"a050_122710");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_122900.fbx", L"a050_122900");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_122901.fbx", L"a050_122901");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_122910.fbx", L"a050_122910");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_122911.fbx", L"a050_122911");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_123100.fbx", L"a050_123100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_123300.fbx", L"a050_123300");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_124100.fbx", L"a050_124100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_124300.fbx", L"a050_124300");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_129920.fbx", L"a050_129920");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130100.fbx", L"a050_130100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130101.fbx", L"a050_130101");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130102.fbx", L"a050_130102");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130200.fbx", L"a050_130200");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130201.fbx", L"a050_130201");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130202.fbx", L"a050_130202");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130300.fbx", L"a050_130300");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130301.fbx", L"a050_130301");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130302.fbx", L"a050_130302");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_130700.fbx", L"a050_130700");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_132101.fbx", L"a050_132101");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_132102.fbx", L"a050_132102");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_132111.fbx", L"a050_132111");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_132112.fbx", L"a050_132112");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_132301.fbx", L"a050_132301");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_132302.fbx", L"a050_132302");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_132311.fbx", L"a050_132311");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_132312.fbx", L"a050_132312");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_132700.fbx", L"a050_132700");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_132710.fbx", L"a050_132710");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_133100.fbx", L"a050_133100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_133300.fbx", L"a050_133300");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_134100.fbx", L"a050_134100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_134300.fbx", L"a050_134300");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_139900.fbx", L"a050_139900");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_139910.fbx", L"a050_139910");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_139920.fbx", L"a050_139920");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_139930.fbx", L"a050_139930");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_140000.fbx", L"a050_140000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_140001.fbx", L"a050_140001");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_150000.fbx", L"a050_150000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_150001.fbx", L"a050_150001");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190100.fbx", L"a050_190100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190120.fbx", L"a050_190120");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190130.fbx", L"a050_190130");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190150.fbx", L"a050_190150");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190155.fbx", L"a050_190155");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190500.fbx", L"a050_190500");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190501.fbx", L"a050_190501");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_190502.fbx", L"a050_190502");

				meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a050_1xxxxx.animationdata");
				*/
#pragma endregion

#pragma region	a050_20xxxx 가드, 인살
			/*
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203000.fbx", L"a050_203000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203001.fbx", L"a050_203001");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203005.fbx", L"a050_203005");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203006.fbx", L"a050_203006");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203010.fbx", L"a050_203010");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203011.fbx", L"a050_203011");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203012.fbx", L"a050_203012");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203015.fbx", L"a050_203015");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203016.fbx", L"a050_203016");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203017.fbx", L"a050_203017");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203100.fbx", L"a050_203100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203300.fbx", L"a050_203300");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203310.fbx", L"a050_203310");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203320.fbx", L"a050_203320");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203330.fbx", L"a050_203330");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203400.fbx", L"a050_203400");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203500.fbx", L"a050_203500");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203510.fbx", L"a050_203510");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203520.fbx", L"a050_203520");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203540.fbx", L"a050_203540");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203541.fbx", L"a050_203541");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203600.fbx", L"a050_203600");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203601.fbx", L"a050_203601");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203602.fbx", L"a050_203602");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203610.fbx", L"a050_203610");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203620.fbx", L"a050_203620");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203621.fbx", L"a050_203621");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203622.fbx", L"a050_203622");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203640.fbx", L"a050_203640");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_203641.fbx", L"a050_203641");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_206200.fbx", L"a050_206200");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_206201.fbx", L"a050_206201");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_206202.fbx", L"a050_206202");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_206203.fbx", L"a050_206203");

				meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a050_20xxxx.animationdata");
			*/
#pragma endregion

#pragma region	a050_3xxxxx 베기, 찌르기
				/*
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300000.fbx", L"a050_300000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300001.fbx", L"a050_300001");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300010.fbx", L"a050_300010");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300011.fbx", L"a050_300011");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300020.fbx", L"a050_300020");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300030.fbx", L"a050_300030");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300040.fbx", L"a050_300040");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300100.fbx", L"a050_300100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300101.fbx", L"a050_300101");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300110.fbx", L"a050_300110");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_300111.fbx", L"a050_300111");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_301000.fbx", L"a050_301000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_301050.fbx", L"a050_301050");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_301100.fbx", L"a050_301100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_302010.fbx", L"a050_302010");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_302011.fbx", L"a050_302011");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_304011.fbx", L"a050_304011");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_304012.fbx", L"a050_304012");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_304021.fbx", L"a050_304021");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_304022.fbx", L"a050_304022");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_304121.fbx", L"a050_304121");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_304122.fbx", L"a050_304122");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_305000.fbx", L"a050_305000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_305001.fbx", L"a050_305001");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_305100.fbx", L"a050_305100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_305101.fbx", L"a050_305101");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308000.fbx", L"a050_308000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308010.fbx", L"a050_308010");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308020.fbx", L"a050_308020");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308050.fbx", L"a050_308050");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308060.fbx", L"a050_308060");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308070.fbx", L"a050_308070");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308900.fbx", L"a050_308900");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308901.fbx", L"a050_308901");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308910.fbx", L"a050_308910");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_308911.fbx", L"a050_308911");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_309000.fbx", L"a050_309000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_309050.fbx", L"a050_309050");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_310000.fbx", L"a050_310000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_310010.fbx", L"a050_310010");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_310020.fbx", L"a050_310020");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_310100.fbx", L"a050_310100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_310110.fbx", L"a050_310110");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_310200.fbx", L"a050_310200");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_310201.fbx", L"a050_310201");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_311000.fbx", L"a050_311000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_311010.fbx", L"a050_311010");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_311020.fbx", L"a050_311020");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_311100.fbx", L"a050_311100");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_311110.fbx", L"a050_311110");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_311200.fbx", L"a050_311200");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_311201.fbx", L"a050_311201");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_313001.fbx", L"a050_313001");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_313002.fbx", L"a050_313002");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_314000.fbx", L"a050_314000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_314010.fbx", L"a050_314010");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_315000.fbx", L"a050_315000");
				meshData->LoadAnimationFromFbx(L"Player\\Animation\\a050_315000.fbx", L"a050_390000");

				meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a050_3xxxxx.animationdata");
				*/
#pragma endregion

#pragma region	a200_5xxxxx 베기, 인살
			/*
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_500000.fbx", L"a200_500000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_500200.fbx", L"a200_500200");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_500600.fbx", L"a200_500600");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_500700.fbx", L"a200_500700");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_500800.fbx", L"a200_500800");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_500810.fbx", L"a200_500810");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_501900.fbx", L"a200_501900");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_502500.fbx", L"a200_502500");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_505000.fbx", L"a200_505000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510000.fbx", L"a200_510000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510100.fbx", L"a200_510100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510110.fbx", L"a200_510110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510120.fbx", L"a200_510120");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510200.fbx", L"a200_510200");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510210.fbx", L"a200_510210");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510220.fbx", L"a200_510220");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510230.fbx", L"a200_510230");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510300.fbx", L"a200_510300");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510310.fbx", L"a200_510310");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510400.fbx", L"a200_510400");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510410.fbx", L"a200_510410");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510600.fbx", L"a200_510600");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510700.fbx", L"a200_510700");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510800.fbx", L"a200_510800");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510810.fbx", L"a200_510810");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_510900.fbx", L"a200_510900");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_511100.fbx", L"a200_511100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_511110.fbx", L"a200_511110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_511120.fbx", L"a200_511120");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_511300.fbx", L"a200_511300");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_511310.fbx", L"a200_511310");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_511500.fbx", L"a200_511500");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_511510.fbx", L"a200_511510");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_511800.fbx", L"a200_511800");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_511900.fbx", L"a200_511900");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_512500.fbx", L"a200_512500");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a200_515000.fbx", L"a200_515000");

			meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a200_5xxxxx.animationdata");
			*/
#pragma endregion

#pragma region	a201_51xxxx 베기, 다양한 인살
/*
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_510000.fbx", L"a201_510000");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_510001.fbx", L"a201_510001");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_510100.fbx", L"a201_510100");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_510110.fbx", L"a201_510110");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_510111.fbx", L"a201_510111");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_510120.fbx", L"a201_510120");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_510200.fbx", L"a201_510200");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_510201.fbx", L"a201_510201");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_510210.fbx", L"a201_510210");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_510220.fbx", L"a201_510220");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_510230.fbx", L"a201_510230");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_510300.fbx", L"a201_510300");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_510310.fbx", L"a201_510310");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_510311.fbx", L"a201_510311");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_511100.fbx", L"a201_511100");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_511110.fbx", L"a201_511110");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_511111.fbx", L"a201_511111");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_511120.fbx", L"a201_511120");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_511800.fbx", L"a201_511800");
meshData->LoadAnimationFromFbx(L"Player\\Animation\\a201_511900.fbx", L"a201_511900");


				meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a201_51xxxx.animationdata");
				*/
#pragma endregion

#pragma region	a202_5xxxxx 베기, 다양한 인살
			/*
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_500000.fbx", L"a202_500000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_500200.fbx", L"a202_500200");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_500600.fbx", L"a202_500600");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_500700.fbx", L"a202_500700");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_500800.fbx", L"a202_500800");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_500810.fbx", L"a202_500810");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_502500.fbx", L"a202_502500");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_510000.fbx", L"a202_510000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_510100.fbx", L"a202_510100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_510110.fbx", L"a202_510110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_510200.fbx", L"a202_510200");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_510210.fbx", L"a202_510210");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_510220.fbx", L"a202_510220");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_510230.fbx", L"a202_510230");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_510600.fbx", L"a202_510600");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_510700.fbx", L"a202_510700");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_510800.fbx", L"a202_510800");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_510810.fbx", L"a202_510810");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a202_512500.fbx", L"a202_512500");

			meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a202_5xxxxx.animationdata");
	*/
#pragma endregion

#pragma region	a21x_xxxxxx
			/*
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a210_511800.fbx", L"a210_511800");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a210_600000.fbx", L"a210_600000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a210_600001.fbx", L"a210_600001");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a210_610001.fbx", L"a210_610001");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a211_501700.fbx", L"a211_501700");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a211_510000.fbx", L"a211_510000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a211_510100.fbx", L"a211_510100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a211_510110.fbx", L"a211_510110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a211_510300.fbx", L"a211_510300");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a211_510310.fbx", L"a211_510310");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a211_511100.fbx", L"a211_511100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a211_511110.fbx", L"a211_511110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a211_511120.fbx", L"a211_511120");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a211_511700.fbx", L"a211_511700");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a211_511800.fbx", L"a211_511800");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a214_510000.fbx", L"a214_510000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a214_600050.fbx", L"a214_600050");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a214_600051.fbx", L"a214_600051");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a214_610051.fbx", L"a214_610051");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a215_510000.fbx", L"a215_510000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a215_510100.fbx", L"a215_510100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a215_510110.fbx", L"a215_510110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a215_510300.fbx", L"a215_510300");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a215_510310.fbx", L"a215_510310");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a215_511500.fbx", L"a215_511500");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a216_511800.fbx", L"a216_511800");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a216_512500.fbx", L"a216_512500");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a216_512501.fbx", L"a216_512501");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a216_600060.fbx", L"a216_600060");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a216_600061.fbx", L"a216_600061");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a216_610061.fbx", L"a216_610061");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a216_610063.fbx", L"a216_610063");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a217_500000.fbx", L"a217_500000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a217_510000.fbx", L"a217_510000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a217_510100.fbx", L"a217_510100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a217_510110.fbx", L"a217_510110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a217_510120.fbx", L"a217_510120");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a217_510900.fbx", L"a217_510900");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a217_511100.fbx", L"a217_511100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a217_511110.fbx", L"a217_511110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a217_511120.fbx", L"a217_511120");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a217_511800.fbx", L"a217_511800");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a217_600070.fbx", L"a217_600070");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a217_600071.fbx", L"a217_600071");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a217_600080.fbx", L"a217_600080");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a217_600081.fbx", L"a217_600081");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a217_610071.fbx", L"a217_610071");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a217_610081.fbx", L"a217_610081");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a218_511300.fbx", L"a218_511300");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a218_511310.fbx", L"a218_511310");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a219_600090.fbx", L"a219_600090");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a219_600091.fbx", L"a219_600091");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a219_610091.fbx", L"a219_610091");

			meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a21x_xxxxxx.animationdata");
		*/
#pragma endregion

#pragma region	a22x_xxxxxx
			/*
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a220_600100.fbx", L"a220_600100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a220_600101.fbx", L"a220_600101");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a220_610101.fbx", L"a220_610101");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a221_500000.fbx", L"a221_500000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a221_501700.fbx", L"a221_501700");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a221_510000.fbx", L"a221_510000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a221_510100.fbx", L"a221_510100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a221_510110.fbx", L"a221_510110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a221_511500.fbx", L"a221_511500");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a221_511510.fbx", L"a221_511510");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a221_511700.fbx", L"a221_511700");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a221_600110.fbx", L"a221_600110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a221_600111.fbx", L"a221_600111");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a221_600120.fbx", L"a221_600120");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a221_600121.fbx", L"a221_600121");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a221_610111.fbx", L"a221_610111");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a221_610121.fbx", L"a221_610121");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a222_502000.fbx", L"a222_502000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a222_512000.fbx", L"a222_512000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a222_600130.fbx", L"a222_600130");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a222_600131.fbx", L"a222_600131");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a222_610131.fbx", L"a222_610131");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a223_600140.fbx", L"a223_600140");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a223_600141.fbx", L"a223_600141");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a223_600350.fbx", L"a223_600350");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a223_600351.fbx", L"a223_600351");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a223_600352.fbx", L"a223_600352");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a223_600370.fbx", L"a223_600370");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a223_600371.fbx", L"a223_600371");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a223_600372.fbx", L"a223_600372");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a223_610141.fbx", L"a223_610141");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a223_610351.fbx", L"a223_610351");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a223_610371.fbx", L"a223_610371");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a224_600150.fbx", L"a224_600150");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a224_600151.fbx", L"a224_600151");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a224_610151.fbx", L"a224_610151");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a226_510900.fbx", L"a226_510900");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a228_501700.fbx", L"a228_501700");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a228_510000.fbx", L"a228_510000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a228_510110.fbx", L"a228_510110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a228_510120.fbx", L"a228_510120");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a228_511110.fbx", L"a228_511110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a228_511120.fbx", L"a228_511120");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a228_511500.fbx", L"a228_511500");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a228_511510.fbx", L"a228_511510");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a228_600180.fbx", L"a228_600180");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a228_600181.fbx", L"a228_600181");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a228_610181.fbx", L"a228_610181");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a229_510000.fbx", L"a229_510000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a229_510001.fbx", L"a229_510001");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a229_600190.fbx", L"a229_600190");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a229_600191.fbx", L"a229_600191");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a229_610191.fbx", L"a229_610191");

			meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a22x_xxxxxx.animationdata");
		*/	
#pragma endregion

#pragma region a23x_xxxxxx
	/*
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a236_502400.fbx", L"a236_502400");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a236_512400.fbx", L"a236_512400");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a236_600310.fbx", L"a236_600310");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a236_600311.fbx", L"a236_600311");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a236_600340.fbx", L"a236_600340");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a236_600341.fbx", L"a236_600341");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a236_610311.fbx", L"a236_610311");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a237_600320.fbx", L"a237_600320");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a237_600321.fbx", L"a237_600321");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a237_610321.fbx", L"a237_610321");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a238_500000.fbx", L"a238_500000");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a238_510000.fbx", L"a238_510000");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a238_510300.fbx", L"a238_510300");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a238_510310.fbx", L"a238_510310");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a238_511200.fbx", L"a238_511200");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a238_511400.fbx", L"a238_511400");
		meshData->LoadAnimationFromFbx(L"Player\\Animation\\a238_511410.fbx", L"a238_511410");

		meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a23x_xxxxxx.animationdata");
	*/
#pragma endregion

#pragma region	a24x_xxxxxx
		/*
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a241_511110.fbx", L"a241_511110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a245_600330.fbx", L"a245_600330");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a245_600331.fbx", L"a245_600331");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a245_610331.fbx", L"a245_610331");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a247_510000.fbx", L"a247_510000");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a247_510200.fbx", L"a247_510200");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a247_511100.fbx", L"a247_511100");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a247_511110.fbx", L"a247_511110");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a247_511120.fbx", L"a247_511120");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a247_511200.fbx", L"a247_511200");

			meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a24x_xxxxxx.animationdata");
		*/
#pragma endregion

#pragma region	a250_6xxxxx
		/*
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a250_600360.fbx", L"a250_600360");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a250_600361.fbx", L"a250_600361");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a250_610361.fbx", L"a250_610361");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a250_600360.fbx", L"a250_600360");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a250_600361.fbx", L"a250_600361");
			meshData->LoadAnimationFromFbx(L"Player\\Animation\\a250_610361.fbx", L"a250_610361");

			meshData->AnimationSave(L"Player\\AnimationData\\" + name + L"_a25x_xxxxxx.animationdata");
		*/
#pragma endregion

	}
		}
		else
		{
			for (auto iter = mMeshDataMap.begin(); iter != mMeshDataMap.end(); iter++)
			{
				std::wstring name = iter->first;
				std::shared_ptr<MeshData> meshData = iter->second;
				meshData->AnimationLoad(L"Player\\AnimationData\\" + name + L"_a000_0015xx.animationdata");
			}
		}
	}
	void PlayerMeshScript::Play(std::wstring name)
	{
		for (auto iter = mMeshDataMap.begin(); iter != mMeshDataMap.end(); iter++)
		{
			std::shared_ptr<MeshData> meshData = iter->second;
			meshData->Play(name);
		}
	}
}
