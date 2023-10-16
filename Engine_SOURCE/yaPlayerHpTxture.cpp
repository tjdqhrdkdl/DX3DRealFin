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

				//Resurrectiontr->SetPosition(Vector3(-640.0f, -360.0f, -10.0f));
				//Resurrectiontr->SetScale(Vector3(200.0f, 20.0f, 50.0f));


				MeshRenderer* meshRenderer = mPlayerResurrection->AddComponent<MeshRenderer>();
				meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				std::shared_ptr<Material> mat = Resources::Find<Material>(L"ResurrectionMaterial");

				meshRenderer->SetMaterial(mat, 0);
				mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"re_True"));
				//mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"re_False"));


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
		PlayerHpCheak();





		if (Input::GetKey(eKeyCode::U))
		{
			mPlayer->GetState()->AddHp(-Time::DeltaTime() * 10.0f);
		}
		if (Input::GetKey(eKeyCode::I))
		{
			mPlayer->GetState()->AddHp(Time::DeltaTime() * 10.0f);
		}
		if (Input::GetKeyDown(eKeyCode::O))
		{
			PlayerResurrection_True();
		}
		if (Input::GetKeyDown(eKeyCode::P))
		{
			PlayerResurrection_False();
		}


		GameObject::Update();
	}

	void PlayerHpTxture::PlayerHpCheak()
	{
		float hp = (float)PERCENTAGE / (float)mPlayer->GetState()->GetHPMax();
		float culhp = (float)PERCENTAGE - (hp * (float)mPlayer->GetState()->GetHP());

		float test = mPlayer->GetState()->GetHP();

		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Meter];
		renderer::Meter data;

		data.metertime = culhp;

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