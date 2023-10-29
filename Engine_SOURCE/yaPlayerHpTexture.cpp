#include "yaPlayerHpTexture.h"
#include "yaObject.h"
#include "yaMeshRenderer.h"
#include "yaResources.h"
#include "yaPlayer.h"

#include "yaInput.h"
#include "yaTime.h"
#include "yaHPMeterScript.h"
#define PERCENTAGE 0.5

namespace ya
{
	PlayerHpTexture::PlayerHpTexture()
		: GameObject()
		, mPlayer(nullptr)
		, mHpLayout(nullptr)
		, mHpBar(nullptr)
		, mResurrections{}
		, mResurrectionFalse(nullptr)
		, mPostureLayout(nullptr)
	{
		SetName(L"player hp ui");
	}

	PlayerHpTexture::~PlayerHpTexture()
	{
	}

	void PlayerHpTexture::Initialize()
	{
		Resources::Load<Texture>(L"HPLayoutTexture", L"UI\\Texture\\HpLayout.png");
		Resources::Load<Texture>(L"HPBarTexture", L"UI\\Texture\\Hp.png");
		Resources::Load<Texture>(L"Resurrection_True", L"UI\\Texture\\Resurrection_True.png");
		Resources::Load<Texture>(L"Resurrection_False", L"UI\\Texture\\Resurrection_False.png");
		Resources::Load<Texture>(L"PostureLayout", L"UI\\Texture\\PostureLayout.png");
		Resources::Load<Texture>(L"PostureBar", L"UI\\Texture\\PostureBar.png");

		{
			mHpLayout = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mHpLayout->SetName(L"PlayerHpLayout");
			mHpLayout->AddComponent<HPMeterScript>();

			Transform* hptr = mHpLayout->GetComponent<Transform>();

			hptr->SetPosition(Vector3(-650.0f, -350.0f, -10.f));
			hptr->SetScale(Vector3(180.0f, 40.0f, 50.0f));

			MeshRenderer* meshRenderer = mHpLayout->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"HpLayoutMaterial");
			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"HPLayoutTexture"));
		}

		{
			mHpBar = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mHpBar->SetName(L"PlayerHpBar");

			Transform* hpbartr = mHpBar->GetComponent<Transform>();

			hpbartr->SetPosition(Vector3(-650.0f, -350.0f, -10.f));
			hpbartr->SetScale(Vector3(155.0f, 10.0f, 50.0f));

			MeshRenderer* meshRenderer = mHpBar->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"MeterMaterial");

			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"HPBarTexture"));
		}

		
		{
			std::shared_ptr<Material> postureLayoutMaterial = std::make_shared<Material>();
			postureLayoutMaterial->SetRenderingMode(eRenderingMode::Transparent);
			postureLayoutMaterial->SetShader(Resources::Find<Shader>(L"SpriteShader"));
			Resources::Insert<Material>(L"PostureLayoutMaterial", postureLayoutMaterial);

			mPostureLayout = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mPostureLayout->SetName(L"postureLayout");

			Transform* postureLayouttr = mPostureLayout->GetComponent<Transform>();

			postureLayouttr->SetPosition(Vector3(-30.0f, -360.0f, -10.0f));
			postureLayouttr->SetScale(Vector3(230.0f, 20.0f, 50.0f));

			MeshRenderer* meshRenderer = mPostureLayout->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"PostureLayoutMaterial");

			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"PostureLayout"));

		}

		{
			GameObject* PlayerpostureBar = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			PlayerpostureBar->SetName(L"PostureBar1");

			Transform* postureBartr = PlayerpostureBar->GetComponent<Transform>();
			postureBartr->SetPosition(Vector3(25.0f, -360.0f, 0.0f));
			postureBartr->SetScale(Vector3(115.0f, 10.0f, 50.0f));

			MeshRenderer* meshRenderer = PlayerpostureBar->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"PostureBarMaterial");

			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"PostureBar"));

		}
		{
			GameObject* PlayerpostureBar = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			PlayerpostureBar->SetName(L"PostureBar2");

			Transform* postureBartr = PlayerpostureBar->GetComponent<Transform>();

			postureBartr->SetPosition(Vector3(-88.0f, -360.0f, 0.0f));
			postureBartr->SetScale(Vector3(115.0f, 10.0f, 50.0f));
			postureBartr->SetRotation(Vector3(0.0f, 180.0f, 0.0f));

			MeshRenderer* meshRenderer = PlayerpostureBar->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"PostureBarMaterial");

			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"PostureBar"));

		}
		GameObject::Initialize();
	}

	void PlayerHpTexture::Update()
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

		if (Input::GetKey(eKeyCode::U))
		{
			mPlayer->GetState()->AddHp(-Time::DeltaTime() * 10.0f);
		}
		if (Input::GetKey(eKeyCode::I))
		{
			mPlayer->GetState()->AddHp(Time::DeltaTime() * 10.0f);
		}
		if (Input::GetKey(eKeyCode::O))
		{
			mPlayer->GetState()->AddPosture(-Time::DeltaTime() * 30.0f);
		}
		if (Input::GetKey(eKeyCode::P))
		{
			mPlayer->GetState()->AddPosture(Time::DeltaTime() * 30.0f);
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

	void PlayerHpTexture::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void PlayerHpTexture::Render()
	{
		GameObject::Render();
	}

	void PlayerHpTexture::SetPlayer(Player* player)
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
			Resurrectiontr->SetPosition(Vector3(-720.0f + (40.0f * i), -310.0f, -10.0f));
			Resurrectiontr->SetScale(Vector3(35.0f, 30.0f, 30.0f));

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
	}
}