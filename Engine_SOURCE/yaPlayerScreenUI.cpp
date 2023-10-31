#include "yaPlayerScreenUI.h"
#include "yaObject.h"
#include "yaMeshRenderer.h"
#include "yaResources.h"
#include "yaPlayer.h"

#include "yaInput.h"
#include "yaTime.h"
#include "yaHPMeterScript.h"

#include "yaImageScript.h"

#define PERCENTAGE 0.5

namespace ya
{
	PlayerScreenUI::PlayerScreenUI()
		: GameObject()
		, mPlayer(nullptr)
		, mbDeathUI(false)
		, mDeathUITimer(1.0f)
		, mbGameOverUI(false)
		, mGameOverUITimer(1.5f)
		, mHpLayout(nullptr)
		, mHpBar(nullptr)
		, mResurrections{}
		, mResurrectionFalse(nullptr)
		, mPostureLayout(nullptr)
	{
		SetName(L"player hp ui");
	}

	PlayerScreenUI::~PlayerScreenUI()
	{
	}

	void PlayerScreenUI::Initialize()
	{
		Resources::Load<Texture>(L"HPLayoutTexture", L"UI\\Texture\\HpLayout.png");
		Resources::Load<Texture>(L"HPBarTexture", L"UI\\Texture\\Hp.png");
		Resources::Load<Texture>(L"Resurrection_True", L"UI\\Texture\\Resurrection_True.png");
		Resources::Load<Texture>(L"Resurrection_False", L"UI\\Texture\\Resurrection_False.png");
		Resources::Load<Texture>(L"PostureLayout", L"UI\\Texture\\PostureLayout.png");
		Resources::Load<Texture>(L"PostureBar", L"UI\\Texture\\PostureBar.png");
		Resources::Load<Texture>(L"SB_TextEffect_death", L"Texture\\Menu\\Common\\SB_TextEffect_death.png");
		Resources::Load<Texture>(L"SB_Dying_b", L"Texture\\Menu\\Common\\SB_Dying_b.png");
		Resources::Load<Texture>(L"SB_KeyTop_LBTN", L"Texture\\Menu\\Common\\SB_KeyTop_LBTN.png");
		Resources::Load<Texture>(L"SB_KeyTop_RBTN", L"Texture\\Menu\\Common\\SB_KeyTop_RBTN.png");

		{ // Blood Screen
			mBloodScreen = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mBloodScreen->SetName(L"Blood Screen");
			//ImageScript* bloodScreenImageScript = mBloodScreen->AddComponent<ImageScript>();
			//bloodScreenImageScript->SetAlpha(0.0f);
			//bloodScreenImageScript->SetAlphaType(ImageScript::eAlphaType::Transparent);

			Transform* bloodScreenTr = mBloodScreen->GetComponent<Transform>();
			bloodScreenTr->SetPosition(Vector3(0.f, 0.f, -10.f));
			bloodScreenTr->SetScale(Vector3(1600.f, 900.f, 0.f));
			bloodScreenTr->SetRotation(Vector3(90.f, 0.f, 0.f));

			std::shared_ptr<Material> bloodScreenMaterial = std::make_shared<Material>();
			bloodScreenMaterial->SetRenderingMode(eRenderingMode::Transparent);
			bloodScreenMaterial->SetShader(Resources::Find<Shader>(L"ImageShader"));
			bloodScreenMaterial->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"SB_Dying_b"));
			Resources::Insert<Material>(L"BloodScreenMaterial", bloodScreenMaterial);

			MeshRenderer* bloodScreenRd = mBloodScreen->AddComponent<MeshRenderer>();
			bloodScreenRd->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			bloodScreenRd->SetMaterial(bloodScreenMaterial, 0);
		}

		{ // Blood Death Screen
			mBloodDeathScreen = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mBloodDeathScreen->SetName(L"Blood Death Screen");
			//ImageScript* bloodDeathScreenImageScript = mBloodDeathScreen->AddComponent<ImageScript>();
			//bloodDeathScreenImageScript->SetAlpha(0.0f);
			//bloodDeathScreenImageScript->SetAlphaType(ImageScript::eAlphaType::Transparent);

			Transform* bloodScreenTr = mBloodDeathScreen->GetComponent<Transform>();
			bloodScreenTr->SetPosition(Vector3(0.f, 0.f, -10.f));
			bloodScreenTr->SetScale(Vector3(1600.f, 900.f, 0.f));
			bloodScreenTr->SetRotation(Vector3(90.f, 0.f, 0.f));

			std::shared_ptr<Material> bloodScreenMaterial = std::make_shared<Material>();
			bloodScreenMaterial->SetRenderingMode(eRenderingMode::Transparent);
			bloodScreenMaterial->SetShader(Resources::Find<Shader>(L"ImageShader"));
			bloodScreenMaterial->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"SB_Dying_a"));
			Resources::Insert<Material>(L"BloodDeathScreenMaterial", bloodScreenMaterial);

			MeshRenderer* bloodDeathScreenRd = mBloodDeathScreen->AddComponent<MeshRenderer>();
			bloodDeathScreenRd->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			bloodDeathScreenRd->SetMaterial(bloodScreenMaterial, 0);
		}

		{ // hp layout
			mHpLayout = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mHpLayout->SetName(L"PlayerHpLayout");
			mHpLayout->AddComponent<HPMeterScript>();

			Transform* hptr = mHpLayout->GetComponent<Transform>();

			hptr->SetPosition(Vector3(-650.0f, -350.0f, -10.f));
			hptr->SetScale(Vector3(180.0f, 40.0f, 50.0f));

			MeshRenderer* hpRd = mHpLayout->AddComponent<MeshRenderer>();
			hpRd->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"HpLayoutMaterial");
			hpRd->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"HPLayoutTexture"));
		}

		{ // hp
			mHpBar = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mHpBar->SetName(L"PlayerHpBar");

			Transform* hpbartr = mHpBar->GetComponent<Transform>();

			hpbartr->SetPosition(Vector3(-650.0f, -350.0f, -10.f));
			hpbartr->SetScale(Vector3(155.0f, 10.0f, 50.0f));

			MeshRenderer* hpBarRd = mHpBar->AddComponent<MeshRenderer>();
			hpBarRd->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"MeterMaterial");

			hpBarRd->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"HPBarTexture"));
		}

		{ // posture layout
			mPostureLayout = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mPostureLayout->SetName(L"postureLayout");

			std::shared_ptr<Material> postureLayoutMaterial = std::make_shared<Material>();
			postureLayoutMaterial->SetRenderingMode(eRenderingMode::Transparent);
			postureLayoutMaterial->SetShader(Resources::Find<Shader>(L"SpriteShader"));
			postureLayoutMaterial->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"PostureLayout"));
			Resources::Insert<Material>(L"PostureLayoutMaterial", postureLayoutMaterial);

			Transform* postureLayouttr = mPostureLayout->GetComponent<Transform>();
			postureLayouttr->SetPosition(Vector3(-30.0f, -360.0f, -10.0f));
			postureLayouttr->SetScale(Vector3(230.0f, 20.0f, 50.0f));

			MeshRenderer* postureRd = mPostureLayout->AddComponent<MeshRenderer>();
			postureRd->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			postureRd->SetMaterial(postureLayoutMaterial, 0);
		}

		{ // posturebar1
			GameObject* PlayerpostureBar = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			PlayerpostureBar->SetName(L"PostureBar1");

			Transform* postureBartr = PlayerpostureBar->GetComponent<Transform>();
			postureBartr->SetPosition(Vector3(25.0f, -360.0f, 0.0f));
			postureBartr->SetScale(Vector3(115.0f, 10.0f, 50.0f));

			MeshRenderer* postureBarRd = PlayerpostureBar->AddComponent<MeshRenderer>();
			postureBarRd->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"PostureBarMaterial");
			postureBarRd->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"PostureBar"));
		}
		{ // posturebar2
			GameObject* PlayerpostureBar = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			PlayerpostureBar->SetName(L"PostureBar2");

			Transform* postureBartr = PlayerpostureBar->GetComponent<Transform>();
			postureBartr->SetPosition(Vector3(-88.0f, -360.0f, 0.0f));
			postureBartr->SetScale(Vector3(115.0f, 10.0f, 50.0f));
			postureBartr->SetRotation(Vector3(0.0f, 180.0f, 0.0f));

			MeshRenderer* postureBarRd = PlayerpostureBar->AddComponent<MeshRenderer>();
			postureBarRd->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"PostureBarMaterial");

			postureBarRd->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"PostureBar"));
		}

		{ // SB_KeyTop_RBTN
			mKeyRBTN = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mKeyRBTN->SetName(L"UI key RBTN");
			//ImageScript* rbtnImageScript = mKeyRBTN->AddComponent<ImageScript>();
			//rbtnImageScript->SetAlpha(0.0f);
			//rbtnImageScript->SetAlphaType(ImageScript::eAlphaType::Transparent);

			Transform* keyRBTNTr = mKeyRBTN->GetComponent<Transform>();
			keyRBTNTr->SetPosition(Vector3(100.f, -350.f, -10.f));
			keyRBTNTr->SetScale(Vector3(30.f, 30.f, 0.f));
			keyRBTNTr->SetRotation(Vector3(90.f, 0.f, 0.f));

			std::shared_ptr<Material> keyRBTNMaterial = std::make_shared<Material>();
			keyRBTNMaterial->SetRenderingMode(eRenderingMode::Transparent);
			keyRBTNMaterial->SetShader(Resources::Find<Shader>(L"ImageShader"));
			keyRBTNMaterial->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"SB_KeyTop_RBTN"));
			Resources::Insert<Material>(L"KeyRBTNMaterial", keyRBTNMaterial);

			MeshRenderer* keyRbtnRd = mKeyRBTN->AddComponent<MeshRenderer>();
			keyRbtnRd->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			keyRbtnRd->SetMaterial(keyRBTNMaterial, 0);
		}
		{ // SB_KeyTop_LBTN
			mKeyLBTN = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mKeyLBTN->SetName(L"UI key LBTN");
			//ImageScript* lbtnImageScript = mKeyLBTN->AddComponent<ImageScript>();
			//lbtnImageScript->SetAlpha(0.0f);
			//lbtnImageScript->SetAlphaType(ImageScript::eAlphaType::Transparent);

			Transform* keyLBTNTr = mKeyLBTN->GetComponent<Transform>();
			keyLBTNTr->SetPosition(Vector3(-100.f, -350.f, -10.f));
			keyLBTNTr->SetScale(Vector3(30.f, 30.f, 0.f));
			keyLBTNTr->SetRotation(Vector3(90.f, 0.f, 0.f));

			std::shared_ptr<Material> keyLBTNMaterial = std::make_shared<Material>();
			keyLBTNMaterial->SetRenderingMode(eRenderingMode::Transparent);
			keyLBTNMaterial->SetShader(Resources::Find<Shader>(L"ImageShader"));
			keyLBTNMaterial->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"SB_KeyTop_LBTN"));
			Resources::Insert<Material>(L"KeyLBTNMaterial", keyLBTNMaterial);

			MeshRenderer* keyLbtnRd = mKeyLBTN->AddComponent<MeshRenderer>();
			keyLbtnRd->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			keyLbtnRd->SetMaterial(keyLBTNMaterial, 0);
		}
	
		GameObject::Initialize();
	}

	void PlayerScreenUI::Update()
	{
		if (mPlayer != nullptr)
		{
			State* state = mPlayer->GetState();
			int resurrectionCountMax = state->GetResurrectionCountMax();
			int resurrectionCount = state->GetResurrectionCount();

			for (size_t i = 0; i < resurrectionCountMax; i++)
			{
				Transform* resurrectionTr = mResurrections[i]->GetComponent<Transform>();

				if (i < resurrectionCount)
					resurrectionTr->SetScale(Vector3(35.0f, 30.0f, 30.0f));
				else
					resurrectionTr->SetScale(Vector3::Zero);
			}

			Transform* resurrectionFalseTr = mResurrectionFalse->GetComponent<Transform>();
			if(resurrectionCount == 0)
				resurrectionFalseTr->SetScale(Vector3(230.0f, 15.0f, 10.0f));
			else
				resurrectionFalseTr->SetScale(Vector3::Zero);
		}

		if (mbDeathUI)
		{
			if (mDeathUITimer > 0.0f)
			{
				mDeathUITimer -= Time::DeltaTime();
			}
			else
			{
				mbDeathUI = false;
				DeathUI(true);
			}
		}

		if (mbGameOverUI)
		{
			if (mGameOverUITimer > 0.0f)
			{
				mGameOverUITimer -= Time::DeltaTime();
			}
			else
			{
				mbGameOverUI = false;
				GameOverUI(true);
			}
		}

		if (mPlayer != nullptr)
		{
			State* state = mPlayer->GetState();

			float hp = (float)PERCENTAGE / (float)state->GetHPMax();
			float culhp = (float)PERCENTAGE - (hp * (float)state->GetHP());
			float posture = (float)PERCENTAGE / (float)state->GetPostureMax();
			float culposture = (float)PERCENTAGE - (posture * (float)state->GetPosture());

			renderer::MeterCB data;
			data.HpMeter = culhp;
			data.PostureMeter = culposture;
			mHpLayout->GetScript<HPMeterScript>()->SetCB(data);
		}

		GameObject::Update();
	}

	void PlayerScreenUI::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void PlayerScreenUI::Render()
	{
		GameObject::Render();
	}

	void PlayerScreenUI::SetPlayer(Player* player)
	{
		mPlayer = player;

		std::shared_ptr<Material> resurrectionMaterial = std::make_shared<Material>();
		resurrectionMaterial->SetRenderingMode(eRenderingMode::Transparent);
		resurrectionMaterial->SetShader(Resources::Find<Shader>(L"SpriteShader"));
		resurrectionMaterial->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"Resurrection_True"));
		Resources::Insert<Material>(L"ResurrectionMaterial", resurrectionMaterial);

		State* state = mPlayer->GetState();
		for (size_t i = 0; i < state->GetResurrectionCountMax(); i++)
		{
			GameObject* resurrection = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			resurrection->SetName(L"resurrection_" + std::to_wstring(i));

			Transform* Resurrectiontr = resurrection->GetComponent<Transform>();
			Resurrectiontr->SetPosition(Vector3(-720.0f + (36.0f * i), -310.0f, -10.0f));
			Resurrectiontr->SetScale(Vector3(30.0f, 30.0f, 30.0f));

			MeshRenderer* meshRenderer = resurrection->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			meshRenderer->SetMaterial(resurrectionMaterial, 0);

			mResurrections.push_back(resurrection);
		}

		{
			mResurrectionFalse = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mResurrectionFalse->SetName(L"resurrectionFalse");

			Transform* resurrectionFalse = mResurrectionFalse->GetComponent<Transform>();
			resurrectionFalse->SetPosition(Vector3(-640.0f, -310.0f, -10.0f));
			//resurrectionFalse->SetScale(Vector3(230.0f, 15.0f, 10.0f));
			resurrectionFalse->SetScale(Vector3::Zero);

			std::shared_ptr<Material> resurrectionFalseMaterial = std::make_shared<Material>();
			resurrectionFalseMaterial->SetRenderingMode(eRenderingMode::Transparent);
			resurrectionFalseMaterial->SetShader(Resources::Find<Shader>(L"SpriteShader"));
			resurrectionFalseMaterial->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"Resurrection_False"));
			Resources::Insert<Material>(L"ResurrectionFalseMaterial", resurrectionFalseMaterial);

			MeshRenderer* meshRenderer = mResurrectionFalse->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			meshRenderer->SetMaterial(resurrectionFalseMaterial, 0);
		}

		{ // fade Object
			mFadeObject = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mFadeObject->SetName(L"FadeObject");
			//ImageScript* imageScript = mFadeObject->AddComponent<ImageScript>();
			//imageScript->SetAlpha(0.0f);
			//imageScript->SetAlphaType(ImageScript::eAlphaType::Transparent);

			Transform* fadeObjectTr = mFadeObject->GetComponent<Transform>();
			fadeObjectTr->SetPosition(Vector3(0.0f, 0.0f, -100.0f));
			fadeObjectTr->SetScale(Vector3(1600.0f, 900.0f, 1.0f));
			fadeObjectTr->SetRotation(Vector3(90.f, 0.f, 0.f));

			std::shared_ptr<Material> fadeMaterial = std::make_shared<Material>();
			fadeMaterial->SetRenderingMode(eRenderingMode::Transparent);
			fadeMaterial->SetShader(Resources::Find<Shader>(L"ImageShader"));
			fadeMaterial->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"bgblack"));
			Resources::Insert<Material>(L"FadeMaterial", fadeMaterial);

			MeshRenderer* fadeRd = mFadeObject->AddComponent<MeshRenderer>();
			fadeRd->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			fadeRd->SetMaterial(fadeMaterial, 0);
		}

		{ // death text
			mDeathText = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mDeathText->SetName(L"UI Death Text");
			ImageScript* deathTextimageScript = mDeathText->AddComponent<ImageScript>();
			deathTextimageScript->SetAlpha(0.0f);
			//deathTextimageScript->SetAlphaType(ImageScript::eAlphaType::Opacity);

			Transform* deathTextTr = mDeathText->GetComponent<Transform>();
			deathTextTr->SetPosition(Vector3(0.f, 0.f, -150.f));
			deathTextTr->SetScale(Vector3(0, 0, 0.f));

			std::shared_ptr<Material> deathTextMaterial = std::make_shared<Material>();
			deathTextMaterial->SetRenderingMode(eRenderingMode::Transparent);
			deathTextMaterial->SetShader(Resources::Find<Shader>(L"ImageShader"));
			deathTextMaterial->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"SB_TextEffect_death"));
			Resources::Insert<Material>(L"DeathTextMaterial", deathTextMaterial);

			MeshRenderer* deathTextRd = mDeathText->AddComponent<MeshRenderer>();
			deathTextRd->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			deathTextRd->SetMaterial(deathTextMaterial, 0);
		}
	}

	void PlayerScreenUI::OnDeathUI(bool on)
	{
		if (on)
		{
			mbDeathUI = true;
			mDeathUITimer = 1.0f;
		}
		else
		{
			DeathUI(false);
		}
	}

	void PlayerScreenUI::OnGameOverUI(bool on)
	{
		if (on)
		{
			mbGameOverUI = true;
			mGameOverUITimer = 2.0f;
		}
		else
		{
			GameOverUI(false);
		}
	}

	void PlayerScreenUI::DeathUI(bool on)
	{
		mBloodDeathScreen->GetComponent<Transform>()->SetRotation(Vector3(90.f, 0.f, 0.f));
		mFadeObject->GetComponent<Transform>()->SetRotation(Vector3(90.f, 0.f, 0.f));

		if (on)
		{
			mBloodScreen->GetComponent<Transform>()->SetRotation(Vector3::Zero);
			mKeyLBTN->GetComponent<Transform>()->SetRotation(Vector3::Zero);
			mKeyRBTN->GetComponent<Transform>()->SetRotation(Vector3::Zero);

			mDeathText->GetScript<ImageScript>()->FadeIn();
			//mDeathText->GetScript<ImageScript>()->SetColor(Vector3(1.f, 1.f, 1.f), 0.0f);
			//mBloodScreen->GetScript<ImageScript>()->FadeIn();
			//mKeyLBTN->GetScript<ImageScript>()->FadeIn();
			//mKeyRBTN->GetScript<ImageScript>()->FadeIn();
		}
		else
		{
			mBloodScreen->GetComponent<Transform>()->SetRotation(Vector3(90.f, 0.f, 0.f));
			mKeyLBTN->GetComponent<Transform>()->SetRotation(Vector3(90.f, 0.f, 0.f));
			mKeyRBTN->GetComponent<Transform>()->SetRotation(Vector3(90.f, 0.f, 0.f));

			//mDeathText->GetScript<ImageScript>()->FadeOut(100.0f);
			mDeathText->GetScript<ImageScript>()->SetAlpha(0.0f);
			//mBloodScreen->GetScript<ImageScript>()->FadeOut(10.0f);
			//mKeyLBTN->GetScript<ImageScript>()->FadeOut(10.0f);
			//mKeyRBTN->GetScript<ImageScript>()->FadeOut(10.0f);
		}
	}

	void PlayerScreenUI::GameOverUI(bool on)
	{
		mBloodScreen->GetComponent<Transform>()->SetRotation(Vector3(90.f, 0.f, 0.f));
		mKeyLBTN->GetComponent<Transform>()->SetRotation(Vector3(90.f, 0.f, 0.f));
		mKeyRBTN->GetComponent<Transform>()->SetRotation(Vector3(90.f, 0.f, 0.f));

		if (on)
		{
			mBloodDeathScreen->GetComponent<Transform>()->SetRotation(Vector3::Zero);
			mFadeObject->GetComponent<Transform>()->SetRotation(Vector3::Zero);

			mDeathText->GetScript<ImageScript>()->FadeIn(0.4);
			//mDeathText->GetScript<ImageScript>()->SetColor(Vector3(1.f, 0.f, 0.f), 0.8f);
			//mFadeObject->GetScript<ImageScript>()->FadeIn();
			//mDeathText->GetScript<ImageScript>()->SetColor(Vector3(1.f, 0.f, 0.f), 0.8f);
			//mDeathText->GetScript<ImageScript>()->FadeIn();
			//mBloodDeathScreen->GetScript<ImageScript>()->FadeIn();
		}
		else
		{
			mBloodDeathScreen->GetComponent<Transform>()->SetRotation(Vector3(90.f, 0.f, 0.f));
			mFadeObject->GetComponent<Transform>()->SetRotation(Vector3(90.f, 0.f, 0.f));
			//mDeathText->GetScript<ImageScript>()->FadeOut(100.0f);
			mDeathText->GetScript<ImageScript>()->SetAlpha(0.0f);
			//mFadeObject->GetScript<ImageScript>()->FadeOut(10.0f);
			//mDeathText->GetScript<ImageScript>()->FadeOut(10.0f);
			//mBloodDeathScreen->GetScript<ImageScript>()->FadeOut(10.0f);
		}
	}
}