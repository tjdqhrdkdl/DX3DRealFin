#include "yaMonsterUI.h"
#include "yaObject.h"
#include "yaMeshRenderer.h"
#include "yaResources.h"
#include "yaMonsterBase.h"
#include "yaCameraScript.h"
#include "yaHPMeterScript.h"
#include "yaMonsterUIScript.h"
#include "yaPlayerDagerUIScript.h"
#include "yaAlertnessUIScript.h"

#include "yaPlayer.h"

#define PERCENTAGE 0.5

namespace ya
{
	MonsterUI::MonsterUI()
		:mRenderTime(2.5)
		,mRenderTimeChecker(0)
		, mbRender(false)
		,mRedAlertTime(0.5f)
		,mRedAlertTimeChecker(0)
	{
	}
	MonsterUI::~MonsterUI()
	{
	}
	void MonsterUI::Initialize()
	{
		SetName(L"monUI");
		Transform* UITr = GetComponent<Transform>();

		{
			mMonsterHpLayout = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mMonsterHpLayout->SetName(L"MonsterHpLayout");
			mMonsterHpLayout->AddComponent<HPMeterScript>();

			Transform* hptr = mMonsterHpLayout->GetComponent<Transform>();
			hptr->SetParent(UITr);
			hptr->SetScale(Vector3(120.0f, 20.0f, 50.0f));
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
			hpbartr->SetScale(Vector3(100.0f, 5.0f, 50.0f));

			MeshRenderer* meshRenderer = mMonsterHpBar->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"MeterMaterial");

			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"HPBarTexture"));
		}
		{
			std::shared_ptr<Shader> postureLayoutShader = Resources::Find<Shader>(L"SpriteShader");
			std::shared_ptr<Material> postureLayoutMaterial = std::make_shared<Material>();
			postureLayoutMaterial->SetRenderingMode(eRenderingMode::Transparent);
			postureLayoutMaterial->SetShader(postureLayoutShader);
			Resources::Insert<Material>(L"PostureLayoutMaterial", postureLayoutMaterial);

			mMonsterPostureLayout = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mMonsterPostureLayout->SetName(L"postureLayout");

			Transform* postureLayouttr = mMonsterPostureLayout->GetComponent<Transform>();

			postureLayouttr->SetScale(Vector3(100.0f, 5.0f, 50.0f));
			postureLayouttr->SetPosition(Vector3(0, -10.0f, 0));
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

			postureBartr->SetScale(Vector3(50.f, 5.0f, 50.0f));
			postureBartr->SetPosition(Vector3(25, -10.0f, 0));

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
			postureBartr->SetPosition(Vector3(-25.5, -10.0f, 0));
			postureBartr->SetScale(Vector3(50.f, 5.0f, 50.0f));
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

			resCountObjTr->SetScale(Vector3(25, 25, 50.f));
			resCountObjTr->SetPosition(Vector3(-45, 12, 0));
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

			resCountObjTr->SetScale(Vector3(25, 25, 50.f));
			resCountObjTr->SetPosition(Vector3(-30, 12, 0));
			resCountObjTr->SetParent(UITr); 

			MeshRenderer* meshRenderer = mMonsterResurectionCount2->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"ResurectionCountMaterial");

			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"DeathBlowTexture"));
		}
		{
			mMonsterDeathBlow = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mMonsterDeathBlow->SetName(L"DeathBlow");

			Transform* resCountObjTr = mMonsterDeathBlow->GetComponent<Transform>();

			resCountObjTr->SetScale(Vector3(90, 90, 50.f));

			MeshRenderer* meshRenderer = mMonsterDeathBlow->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"ResurectionCountMaterial");

			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"DeathBlowTexture"));
		}
		{
			std::shared_ptr<Shader> lockOnShader = Resources::Find<Shader>(L"SpriteShader");
			std::shared_ptr<Material> lockOnMaterial = std::make_shared<Material>();
			lockOnMaterial->SetRenderingMode(eRenderingMode::Transparent);
			lockOnMaterial->SetShader(lockOnShader);
			Resources::Insert<Material>(L"LockOnMaterial", lockOnMaterial);

			mMonsterLockOn = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mMonsterLockOn->SetName(L"LockOnMark");

			Transform* tr = mMonsterLockOn->GetComponent<Transform>();

			tr->SetScale(Vector3(20, 20, 50.f));
			MeshRenderer* meshRenderer = mMonsterLockOn->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"LockOnMaterial");

			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"LockOnTexture"));
		}
		{
			std::shared_ptr<Shader> alertnessUIShader = Resources::Find<Shader>(L"AlertnessUIShader");
			std::shared_ptr<Material> alertnessUIMaterial = std::make_shared<Material>();
			alertnessUIMaterial->SetRenderingMode(eRenderingMode::Transparent);
			alertnessUIMaterial->SetShader(alertnessUIShader);
			Resources::Insert<Material>(L"AlertnessUIMaterial", alertnessUIMaterial);

			mMonsterAlertnessBoundary = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mMonsterAlertnessBoundary->SetName(L"AnertnessBoundary");
			mMonsterAlertnessBoundary->AddComponent<AlertnessUIScript>();
			Transform* tr = mMonsterAlertnessBoundary->GetComponent<Transform>();

			tr->SetScale(Vector3(25, 25, 50.f));
			MeshRenderer* meshRenderer = mMonsterAlertnessBoundary->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));

			meshRenderer->SetMaterial(alertnessUIMaterial, 0);
			alertnessUIMaterial->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"AlertnessBoundaryTexture"));
		} 
		{
			std::shared_ptr<Shader> alertnessUIShader = Resources::Find<Shader>(L"AlertnessUIShader");
			std::shared_ptr<Material> alertnessUIGageMaterial = std::make_shared<Material>();
			alertnessUIGageMaterial->SetRenderingMode(eRenderingMode::Transparent);
			alertnessUIGageMaterial->SetShader(alertnessUIShader);
			Resources::Insert<Material>(L"AlertnessUIGageMaterial", alertnessUIGageMaterial);

			mMonsterAlertnessGage = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mMonsterAlertnessGage->SetName(L"AnertnessGage");
			mMonsterAlertnessGage->AddComponent<AlertnessUIScript>();

			Transform* tr = mMonsterAlertnessGage->GetComponent<Transform>();

			tr->SetScale(Vector3(19, 19, 50.f));
			tr->SetPosition(Vector3(0, 10, 0.f));
			MeshRenderer* meshRenderer = mMonsterAlertnessGage->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));

			meshRenderer->SetMaterial(alertnessUIGageMaterial, 0);
			alertnessUIGageMaterial->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"AlertnessGageTexture"));
		}

		GameObject::Initialize();
	}
	void MonsterUI::Update()
	{
		if (mMonster)
		{
			MonsterMeterCheck();
			if (mMonster->GetPlayerObject() && !(mMonster->GetPlayerObject()->IsStateFlag(ePlayerState::Death)))
			{
				if (mbRender)
				{
					//ui는 3초간 보여진다.
					mRenderTimeChecker += Time::DeltaTime();
					if (mRenderTimeChecker > mRenderTime)
					{
						mRenderTimeChecker = 0;
						mbRender = false;
					}

					//ui 전체 포지션 값
					Matrix world = mMonster->GetComponent<Transform>()->GetWorldMatrix();
					world._42 += 1.5;
					Matrix fin = world * mainCamera->GetViewMatrix();
					fin *= mainCamera->GetProjectionMatrix();
					Vector4 ndc = Vector4::Transform(Vector4(0, 0, 0, 1), fin);
					ndc = ndc / ndc.w;

					Vector4 UIPos = Vector4::Transform(ndc, UICamera->GetProjectionMatrix().Invert());
					UIPos = Vector4::Transform(UIPos, UICamera->GetViewMatrix().Invert());
					GetComponent<Transform>()->SetPosition(Vector3(UIPos.x, UIPos.y, (float)0.00001));
					if (ndc.z > 1 or ndc.z < 0)
						GetComponent<Transform>()->SetPosition(Vector3(50000, -50000, 0));


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
					mMonsterDeathBlow->SetRender(false);
					mMonsterLockOn->SetRender(false);
					mMonsterAlertnessBoundary->SetRender(false);
					mMonsterAlertnessGage->SetRender(false);
				}

				if (mainCamera->GetOwner()->GetScript<CameraScript>()->GetLockOnTarget()
					== mMonster)
				{
					Matrix world = mMonster->GetComponent<Transform>()->GetWorldMatrix();
					world._42 += 0.25;
					Matrix fin = world * mainCamera->GetViewMatrix();
					fin *= mainCamera->GetProjectionMatrix();
					Vector4 ndc = Vector4::Transform(Vector4(0, 0, 0, 1), fin);
					ndc = ndc / ndc.w;

					Vector4 UIPos = Vector4::Transform(ndc, UICamera->GetProjectionMatrix().Invert());
					UIPos = Vector4::Transform(UIPos, UICamera->GetViewMatrix().Invert());
					mMonsterLockOn->GetComponent<Transform>()->SetPosition(Vector3(UIPos.x, UIPos.y, (float)0.00001));
					if (ndc.z > 1 or ndc.z < 0)
						mMonsterLockOn->GetComponent<Transform>()->SetPosition(Vector3(50000, -50000, 0));
					mMonsterLockOn->SetRender(true);

					mbRender = true;
					mRenderTimeChecker = 0;
				}
				else
				{
					mMonsterLockOn->SetRender(false);
				}



				{
					if (mMonster->IsDeathBlow())
					{
						Matrix world = mMonster->GetComponent<Transform>()->GetWorldMatrix();
						world._42 += -0.25;
						Matrix fin = world * mainCamera->GetViewMatrix();
						fin *= mainCamera->GetProjectionMatrix();
						Vector4 ndc = Vector4::Transform(Vector4(0, 0, 0, 1), fin);
						ndc = ndc / ndc.w;

						Vector4 UIPos = Vector4::Transform(ndc, UICamera->GetProjectionMatrix().Invert());
						UIPos = Vector4::Transform(UIPos, UICamera->GetViewMatrix().Invert());
						mMonsterDeathBlow->GetComponent<Transform>()->SetPosition(Vector3(UIPos.x, UIPos.y, (float)0.00001));
						if (ndc.z > 1 or ndc.z < 0)
							mMonsterDeathBlow->GetComponent<Transform>()->SetPosition(Vector3(50000, -50000, 0));
						mMonsterDeathBlow->SetRender(true);

					}
					else
					{
						mMonsterDeathBlow->SetRender(false);
					}
				}
				{
					if (mMonster->GetAlertnessCount() > 0
						|| (mMonster->IsRecognize() && mRedAlertTimeChecker < mRedAlertTime)
						)
					{
						Matrix world = mMonster->GetComponent<Transform>()->GetWorldMatrix();
						world._42 += 2.0;
						Matrix fin = world * mainCamera->GetViewMatrix();
						fin *= mainCamera->GetProjectionMatrix();
						Vector4 ndc = Vector4::Transform(Vector4(0, 0, 0, 1), fin);
						ndc = ndc / ndc.w;

						Vector4 UIPos = Vector4::Transform(ndc, UICamera->GetProjectionMatrix().Invert());
						UIPos = Vector4::Transform(UIPos, UICamera->GetViewMatrix().Invert());
						mMonsterAlertnessBoundary->GetComponent<Transform>()->SetPosition(Vector3(UIPos.x, UIPos.y, (float)0.00001));
						mMonsterAlertnessGage->GetComponent<Transform>()->SetPosition(Vector3(UIPos.x, UIPos.y, (float)0.00001));
						if (ndc.z > 1 or ndc.z < 0)
						{
							mMonsterAlertnessGage->GetComponent<Transform>()->SetPosition(Vector3(50000, -50000, 0));
							mMonsterAlertnessBoundary->GetComponent<Transform>()->SetPosition(Vector3(50000, -50000, 0));
						}
						mMonsterAlertnessBoundary->SetRender(true);
						mMonsterAlertnessGage->SetRender(true);
					}
					else
					{
						mMonsterAlertnessBoundary->SetRender(false);
						mMonsterAlertnessGage->SetRender(false);
					}

					if (mMonster->IsRecognize())
					{
						if (mRedAlertTimeChecker > mRedAlertTime)
							;
						else
							mRedAlertTimeChecker += Time::DeltaTime();

					}
					else
						mRedAlertTimeChecker = 0;
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
				mMonsterDeathBlow->SetRender(false);
				mMonsterLockOn->SetRender(false);
				mMonsterAlertnessBoundary->SetRender(false);
				mMonsterAlertnessGage->SetRender(false);
			}
		
			if(mMonster->IsMonsterState(MonsterBase::MonsterState_Dead))
			{
				mMonsterHpLayout->SetRender(false);
				mMonsterHpBar->SetRender(false);
				mMonsterPostureLayout->SetRender(false);
				mMonsterPostureBar1->SetRender(false);
				mMonsterPostureBar2->SetRender(false);

				mMonsterResurectionCount1->SetRender(false);
				mMonsterResurectionCount2->SetRender(false);
				mMonsterDeathBlow->SetRender(false);
				mMonsterLockOn->SetRender(false);
				mMonsterAlertnessBoundary->SetRender(false);
				mMonsterAlertnessGage->SetRender(false);
			}
		}
		GameObject::Update();
	}
	void MonsterUI::SetMonster(MonsterBase* mon)
	{
		mMonster = mon;
		mMonster->AddComponent<MonsterUIScript>()->SetMonsterUI(this);
	}
	void MonsterUI::MonsterMeterCheck()
	{

		float hp = (float)PERCENTAGE / (float)mMonster->GetState()->GetHPMax();	
		float culhp = (float)PERCENTAGE - (hp * (float)mMonster->GetState()->GetHP());
		float posture = (float)PERCENTAGE / (float)mMonster->GetState()->GetPostureMax();
		float culposture = (float)PERCENTAGE - (posture * (float)mMonster->GetState()->GetPosture());

		float alertness = mMonster->GetAlertnessCount();
		int recognize = 0;
		if (mMonster->IsRecognize())
			recognize = 1;

		renderer::MeterCB data;

		data.HpMeter = culhp;
		data.PostureMeter = culposture;

		mMonsterHpLayout->GetScript<HPMeterScript>()->SetCB(data);

		renderer::UniformDataCB data2;

		data2.int_0 = 1;
		data2.float_0 = alertness / 100;
		data2.int_1 = recognize;
		mMonsterAlertnessBoundary->GetScript<AlertnessUIScript>()->SetCB(data2);
		data2.int_0 = 0;
		mMonsterAlertnessGage->GetScript<AlertnessUIScript>()->SetCB(data2);
	}
}