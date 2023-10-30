#include "yaBossUI.h"
#include "yaObject.h"
#include "yaMeshRenderer.h"
#include "yaResources.h"
#include "yaMonsterBase.h"
#include "yaCameraScript.h"
#include "yaHPMeterScript.h"
#include "yaMonsterUIScript.h"
#include "yaPlayer.h"

#define PERCENTAGE 0.5

namespace ya
{
	BossUI::BossUI()
		:mbRender(false)
	{
	}
	BossUI::~BossUI()
	{
	}
	void BossUI::Initialize()
	{
		SetName(L"monUI");
		Transform* UITr = GetComponent<Transform>();
		UITr->SetPosition(Vector3(0, 400, 0));
		{
			mMonsterHpLayout = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mMonsterHpLayout->SetName(L"MonsterHpLayout");
			mMonsterHpLayout->AddComponent<HPMeterScript>();

			Transform* hptr = mMonsterHpLayout->GetComponent<Transform>();
			hptr->SetParent(UITr);
			hptr->SetScale(Vector3(300.f, 30.f, 50.0f));
			hptr->SetPosition(Vector3(-590.f, 0.0f, 50.0f));
			MeshRenderer* meshRenderer = mMonsterHpLayout->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"HpLayoutMaterial");
			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"HPLayoutTexture"));
		}
		{
			mMonsterHpBar = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mMonsterHpBar->SetName(L"PlayerHpBar");

			Transform* hpbartr = mMonsterHpBar->GetComponent<Transform>();
			hpbartr->SetParent(UITr);
			hpbartr->SetScale(Vector3(265.f, 10.f, 50.0f));
			hpbartr->SetPosition(Vector3(-591.0f, 0.0f, 50.0f));


			MeshRenderer* meshRenderer = mMonsterHpBar->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"MeterMaterial");

			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"HPBarTexture"));
		}
		{

			mMonsterPostureLayout = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mMonsterPostureLayout->SetName(L"postureLayout");

			Transform* postureLayouttr = mMonsterPostureLayout->GetComponent<Transform>();

			postureLayouttr->SetScale(Vector3(380.f, 20.f, 50.0f));
			postureLayouttr->SetPosition(Vector3(-30, 20.0f, 0));
			postureLayouttr->SetParent(UITr);

			MeshRenderer* meshRenderer = mMonsterPostureLayout->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"PostureLayoutMaterial");

			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"PostureLayout"));

		}

		{
			mMonsterPostureBar1 = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mMonsterPostureBar1->SetName(L"PostureBar1");

			Transform* postureBartr = mMonsterPostureBar1->GetComponent<Transform>();

			postureBartr->SetScale(Vector3(180.f, 10.f, 50.0f));
			postureBartr->SetPosition(Vector3(60, 20.0f, 0));

			postureBartr->SetParent(UITr);

			MeshRenderer* meshRenderer = mMonsterPostureBar1->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"PostureBarMaterial");

			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"PostureBar"));

		}
		{
			mMonsterPostureBar2 = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mMonsterPostureBar2->SetName(L"PostureBar2");

			Transform* postureBartr = mMonsterPostureBar2->GetComponent<Transform>();
			postureBartr->SetPosition(Vector3(-120, 20.0f, 0));
			postureBartr->SetScale(Vector3(180.f, 10.f, 50.0f));

			postureBartr->SetRotation(Vector3(0.0f, 180.0f, 0.0f));

			postureBartr->SetParent(UITr);

			MeshRenderer* meshRenderer = mMonsterPostureBar2->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"PostureBarMaterial");

			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"PostureBar"));

		}
		{
			std::shared_ptr<Shader> resurectionCountShader = Resources::Find<Shader>(L"SpriteShader");
			std::shared_ptr<Material> resurectionCountMaterial = std::make_shared<Material>();
			resurectionCountMaterial->SetRenderingMode(eRenderingMode::Transparent);
			resurectionCountMaterial->SetShader(resurectionCountShader);
			Resources::Insert<Material>(L"ResurectionCountMaterial", resurectionCountMaterial);

			mMonsterResurectionCount1 = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mMonsterResurectionCount1->SetName(L"resCount1");

			Transform* resCountObjTr = mMonsterResurectionCount1->GetComponent<Transform>();

			resCountObjTr->SetScale(Vector3(35, 35, 50.f));
			resCountObjTr->SetPosition(Vector3(-710, 20, 0));
			resCountObjTr->SetParent(UITr);

			MeshRenderer* meshRenderer = mMonsterResurectionCount1->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"ResurectionCountMaterial");

			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"DeathBlowTexture"));
		}
		{
			mMonsterResurectionCount2 = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mMonsterResurectionCount2->SetName(L"resCount2");

			Transform* resCountObjTr = mMonsterResurectionCount2->GetComponent<Transform>();

			resCountObjTr->SetScale(Vector3(35, 35, 50.f));
			resCountObjTr->SetPosition(Vector3(-685, 20, 0));
			resCountObjTr->SetParent(UITr);

			MeshRenderer* meshRenderer = mMonsterResurectionCount2->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"ResurectionCountMaterial");

			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"DeathBlowTexture"));
		}
		GameObject::Initialize();
	}
	void BossUI::Update()
	{
		if (mMonster)
		{
			if (mMonster->GetPlayerObject() && !(mMonster->GetPlayerObject()->IsStateFlag(ePlayerState::Death)))
			{
				if (mbRender)
				{
					//ui 전체 포지션 값


					//ui관련 상수버퍼 
					MonsterMeterCheck();

					//hp bar
					mMonsterHpLayout->SetRender(true);
					mMonsterHpBar->SetRender(true);


					//posture bar
					if (mMonster->GetPosture() == 0)
					{
						mMonsterPostureLayout->SetRender(false);
						mMonsterPostureBar1->SetRender(false);
						mMonsterPostureBar2->SetRender(false);
					}

					else
					{
						mMonsterPostureLayout->SetRender(true);
						mMonsterPostureBar1->SetRender(true);
						mMonsterPostureBar2->SetRender(true);
					}

					//레저렉션 카운트
					if (mMonster->GetResurrectionCountMax() > 1)
					{
						if (mMonster->GetResurrectionCount() == 0)
						{
							mMonsterResurectionCount1->SetRender(false);
							mMonsterResurectionCount2->SetRender(false);
						}
						else if (mMonster->GetResurrectionCount() == 1)
						{
							mMonsterResurectionCount1->SetRender(true);
							mMonsterResurectionCount2->SetRender(false);
						}
						else if (mMonster->GetResurrectionCount() == 2)
						{
							mMonsterResurectionCount1->SetRender(true);
							mMonsterResurectionCount2->SetRender(true);

						}
					}


				}
				else
				{
					mMonsterHpLayout->SetRender(false);
					mMonsterHpBar->SetRender(false);
					mMonsterPostureLayout->SetRender(false);
					mMonsterPostureBar1->SetRender(false);
					mMonsterPostureBar2->SetRender(false);

					mMonsterResurectionCount1->SetRender(false);
					mMonsterResurectionCount2->SetRender(false);
				}

			}
			else
			{
				mMonsterHpLayout->SetRender(false);
				mMonsterHpBar->SetRender(false);
				mMonsterPostureLayout->SetRender(false);
				mMonsterPostureBar1->SetRender(false);
				mMonsterPostureBar2->SetRender(false);

				mMonsterResurectionCount1->SetRender(false);
				mMonsterResurectionCount2->SetRender(false);
			}
		}
		GameObject::Update();
	}
	void BossUI::MonsterMeterCheck()
	{

		float hp = (float)PERCENTAGE / (float)mMonster->GetState()->GetHPMax();
		float culhp = (float)PERCENTAGE - (hp * (float)mMonster->GetState()->GetHP());
		float posture = (float)PERCENTAGE / (float)mMonster->GetState()->GetPostureMax();
		float culposture = (float)PERCENTAGE - (posture * (float)mMonster->GetState()->GetPosture());

		renderer::MeterCB data;

		data.HpMeter = culhp;
		data.PostureMeter = culposture;

		mMonsterHpLayout->GetScript<HPMeterScript>()->SetCB(data);
	}
}