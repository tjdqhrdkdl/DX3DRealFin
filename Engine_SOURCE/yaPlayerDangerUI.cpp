#include "yaPlayerDangerUI.h"
#include "yaObject.h"
#include "yaMeshRenderer.h"
#include "yaResources.h"
#include "yaPlayer.h"
#include "yaCameraScript.h"
#include "yaPlayerDagerUIScript.h"
#include "yaTime.h"
#include "yaInput.h"
#define PERCENTAGE 0.5

namespace ya
{
	PlayerDangerUI::PlayerDangerUI()
		:mRenderTime((float)1.5)
		, mRenderTimeChecker(0)
		, mbRender(false)
	{
	}
	PlayerDangerUI::~PlayerDangerUI()
	{
	}
	void PlayerDangerUI::Initialize()
	{
		SetName(L"plDangerUI");
		Transform* UITr = GetComponent<Transform>();
		Resources::Load<Texture>(L"DangerUITexture", L"UI\\Texture\\DangerUITexture.png");

		{
			mMainUI = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mMainUI->SetName(L"UI_Danger_Main");
			mMainUI->AddComponent<PlayerDangerUIScript>();

			Transform* hptr = mMainUI->GetComponent<Transform>();
			hptr->SetScale(Vector3(100, 100, 50.0f));

			hptr->SetParent(UITr);
			MeshRenderer* meshRenderer = mMainUI->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"DangerUIMaterial");
			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"DangerUITexture"));
		}
		{
			mEffectUI = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mEffectUI->SetName(L"UI_Danger_Effect");
			mEffectUI->AddComponent<PlayerDangerUIScript>();

			Transform* hptr = mEffectUI->GetComponent<Transform>();
			hptr->SetScale(Vector3(120));

			hptr->SetParent(UITr);
			MeshRenderer* meshRenderer = mEffectUI->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"DangerUIMaterial");
			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"DangerUITexture"));
		}

		GameObject::Initialize();
	}
	void PlayerDangerUI::Update()
	{
		if (mPlayer)
		{
			if (mbRender)
			{		//ui는 3초간 보여진다.
				mRenderTimeChecker += Time::DeltaTime();
				if (mRenderTimeChecker > mRenderTime)
				{
					mRenderTimeChecker = 0;
					mbRender = false;

					mEffectUI->SetRender(false);
					mMainUI->SetRender(false);
					return;
				}
				mEffectUI->GetComponent<Transform>()->SetScale(Vector3(120 + 80 * mRenderTimeChecker));
				//ui 전체 포지션 값
				Matrix world = mPlayer->GetComponent<Transform>()->GetWorldMatrix();
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

				//ui관련 상수버퍼 
				CBBind();
				mEffectUI->SetRender(true);
				mMainUI->SetRender(true);
			}

			else
			{
				mEffectUI->SetRender(false);
				mMainUI->SetRender(false);
			}
		}
		GameObject::Update();
	}
	void PlayerDangerUI::SetPlayer(Player* pl)
	{
		mPlayer = pl;
	}
	void PlayerDangerUI::CBBind()
	{
		renderer::TimeCB data;

		data.time = mRenderTimeChecker;
		
		data.isMain = false;
		mMainUI->GetScript<PlayerDangerUIScript>()->SetCB(data);


		data.isMain = true;
		mEffectUI->GetScript<PlayerDangerUIScript>()->SetCB(data);
	}

}