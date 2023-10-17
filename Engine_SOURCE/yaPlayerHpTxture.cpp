#include "yaPlayerHpTxture.h"
#include "yaObject.h"
#include "yaMeshRenderer.h"
#include "yaResources.h"
#include "yaPlayer.h"

#include "yaInput.h"
#include "yaTime.h"

#define PERCENTAGE 0.5

namespace ya
{
	PlayerHpTxture::PlayerHpTxture()
	{

		if(mPlayerHpLayout == nullptr)
		{			

			Resources::Load<Texture>(L"HPLayoutTexture", L"HpLayout.png");
			Resources::Load<Texture>(L"HPBarTexture", L"Hp.png");
			Resources::Load<Texture>(L"re_True", L"Resurrection_True.png");
			Resources::Load<Texture>(L"re_False", L"Resurrection_False.png");
			Resources::Load<Texture>(L"PostureLayout", L"PostureLayout.png");
			Resources::Load<Texture>(L"PostureBar", L"PostureBarTT.png");

			{
				mPlayerHpLayout = object::Instantiate<GameObject>(eLayerType::UI);
				mPlayerHpLayout->SetName(L"PlayerHpLayout");

				Transform* hptr = mPlayerHpLayout->GetComponent<Transform>();

				hptr->SetPosition(Vector3(-650.0f, -400.f, -10.f));

				hptr->SetScale(Vector3(180.0f, 50.0f, 50.0f));



				MeshRenderer* meshRenderer = mPlayerHpLayout->AddComponent<MeshRenderer>();
				meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				std::shared_ptr<Material> mat = Resources::Find<Material>(L"HpLayoutMaterial");

				meshRenderer->SetMaterial(mat, 0);
				mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"HPLayoutTexture"));
			}


			{
				mPlayerHpBar = object::Instantiate<GameObject>(eLayerType::UI);
				mPlayerHpBar->SetName(L"PlayerHpBar");

				Transform* hpbartr = mPlayerHpBar->GetComponent<Transform>();

				//hpbartr->SetPosition(Vector3(0.0f, 0.f, 0.f));
				hpbartr->SetPosition(Vector3(-652.0f, -400.f, -10.f));

				hpbartr->SetScale(Vector3(155.0f, 10.0f, 50.0f));
				MeshRenderer* meshRenderer = mPlayerHpBar->AddComponent<MeshRenderer>();
				meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				std::shared_ptr<Material> mat = Resources::Find<Material>(L"MeterMaterial");

				meshRenderer->SetMaterial(mat, 0);
				mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"HPBarTexture"));
			}

			{
				std::shared_ptr<Shader> resurrectionShader = Resources::Find<Shader>(L"SpriteShader");
				std::shared_ptr<Material> resurrectionMaterial = std::make_shared<Material>();
				resurrectionMaterial->SetRenderingMode(eRenderingMode::Transparent);
				resurrectionMaterial->SetShader(resurrectionShader);
				Resources::Insert<Material>(L"ResurrectionMaterial", resurrectionMaterial);

				mPlayerResurrection = object::Instantiate<GameObject>(eLayerType::UI);
				mPlayerResurrection->SetName(L"PlayerHpBar");

				Transform* Resurrectiontr = mPlayerResurrection->GetComponent<Transform>();

				Resurrectiontr->SetPosition(Vector3(-680.0f, -360.0f, -10.0f));
				Resurrectiontr->SetScale(Vector3(30.0f, 30.0f, 50.0f));

				MeshRenderer* meshRenderer = mPlayerResurrection->AddComponent<MeshRenderer>();
				meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				std::shared_ptr<Material> mat = Resources::Find<Material>(L"ResurrectionMaterial");

				meshRenderer->SetMaterial(mat, 0);
				mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"re_True"));
				
			}

			{
				std::shared_ptr<Shader> postureLayoutShader = Resources::Find<Shader>(L"SpriteShader");
				std::shared_ptr<Material> postureLayoutMaterial = std::make_shared<Material>();
				postureLayoutMaterial->SetRenderingMode(eRenderingMode::Transparent);
				postureLayoutMaterial->SetShader(postureLayoutShader);
				Resources::Insert<Material>(L"PostureLayoutMaterial", postureLayoutMaterial);

				mPlayerpostureLayout = object::Instantiate<GameObject>(eLayerType::UI);
				mPlayerpostureLayout->SetName(L"postureLayout");

				Transform* postureLayouttr = mPlayerpostureLayout->GetComponent<Transform>();

				postureLayouttr->SetPosition(Vector3(-30.0f, -360.0f, -10.0f));
				postureLayouttr->SetScale(Vector3(230.0f, 20.0f, 50.0f));

				MeshRenderer* meshRenderer = mPlayerpostureLayout->AddComponent<MeshRenderer>();
				meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				std::shared_ptr<Material> mat = Resources::Find<Material>(L"PostureLayoutMaterial");

				meshRenderer->SetMaterial(mat, 0);
				mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"PostureLayout"));

			}

			{
				GameObject* PlayerpostureBar = object::Instantiate<GameObject>(eLayerType::UI);
				PlayerpostureBar->SetName(L"PostureBar1");

				Transform* postureBartr = PlayerpostureBar->GetComponent<Transform>();

				postureBartr->SetPosition(Vector3(25.0f, -360.0f, -10.0f));
				postureBartr->SetScale(Vector3(115.0f, 10.0f, 50.0f));

				MeshRenderer* meshRenderer = PlayerpostureBar->AddComponent<MeshRenderer>();
				meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				std::shared_ptr<Material> mat = Resources::Find<Material>(L"PostureBarMaterial");

				meshRenderer->SetMaterial(mat, 0);
				mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"PostureBar"));
				
			}
			{
				GameObject* PlayerpostureBar = object::Instantiate<GameObject>(eLayerType::UI);
				PlayerpostureBar->SetName(L"PostureBar2");

				Transform* postureBartr = PlayerpostureBar->GetComponent<Transform>();

				postureBartr->SetPosition(Vector3(-88.0f, -360.0f, -10.0f));
				postureBartr->SetScale(Vector3(115.0f, 10.0f, 50.0f));
				postureBartr->SetRotation(Vector3(0.0f, 180.0f, 0.0f));

				MeshRenderer* meshRenderer = PlayerpostureBar->AddComponent<MeshRenderer>();
				meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				std::shared_ptr<Material> mat = Resources::Find<Material>(L"PostureBarMaterial");

				meshRenderer->SetMaterial(mat, 0);
				mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"PostureBar"));

			}
		}


		



	}

	PlayerHpTxture::~PlayerHpTxture()
	{


	}

	void PlayerHpTxture::Initialize()
	{

		

		GameObject::Initialize();
	}

	void PlayerHpTxture::Update()
	{
		if(mPlayer != nullptr)
		{
			PlayerMeterCheak();
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
			mPlayer->GetState()->AddPosture(-Time::DeltaTime() * 10.0f);
		}
		if (Input::GetKey(eKeyCode::P))
		{
			mPlayer->GetState()->AddPosture(Time::DeltaTime() * 10.0f);
		}


		GameObject::Update();
	}

	void PlayerHpTxture::PlayerMeterCheak()
	{
		float hp = (float)PERCENTAGE / (float)mPlayer->GetState()->GetHPMax();
		float culhp = (float)PERCENTAGE - (hp * (float)mPlayer->GetState()->GetHP());
		float posture = (float)PERCENTAGE / (float)mPlayer->GetState()->GetPostureMax();
		float culposture = (float)PERCENTAGE - (posture * (float)mPlayer->GetState()->GetPosture());


		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::HpMeter];
		renderer::Meter data;
		
		data.HpMeter = culhp;
		data.PostureMeter = culposture;		
		

		cb->SetData(&data);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);

	}



	void PlayerHpTxture::PlayerResurrection_True()
	{
		std::shared_ptr<Material> mat = Resources::Find<Material>(L"ResurrectionMaterial");
		mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"re_True"));


		Transform* Resurrectiontr = mPlayerResurrection->GetComponent<Transform>();
		Resurrectiontr->SetPosition(Vector3(-680.0f, -360.f, -10.f));
		Resurrectiontr->SetScale(Vector3(30.0f, 30.0f, 50.0f));

	}

	void PlayerHpTxture::PlayerResurrection_False()
	{	
		
		std::shared_ptr<Material> mat = Resources::Find<Material>(L"ResurrectionMaterial");				
		mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"re_False"));


		Transform* Resurrectiontr = mPlayerResurrection->GetComponent<Transform>();
		Resurrectiontr->SetPosition(Vector3(-640.0f, -360.f, -10.f));
		Resurrectiontr->SetScale(Vector3(200.0f, 20.0f, 50.0f));
	}


}