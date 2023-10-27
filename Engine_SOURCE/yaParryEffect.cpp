#include "yaParryEffect.h"
#include "yaObject.h"
#include "yaMeshRenderer.h"
#include "yaResources.h"
#include "yaPlayer.h"
#include "yaCameraScript.h"
#include "yaPlayerDagerUIScript.h"
#include "yaTime.h"
#include "yaInput.h"
#include "yaAnimator.h"
#include "yaSpriteRenderer.h"
#define PERCENTAGE 0.5

namespace ya
{
	ParryEffect::ParryEffect()
		:mRenderTime((float)0.25)
		, mRenderTimeChecker(0)
		, mbRender(false)
	{
	}
	ParryEffect::~ParryEffect()
	{
	}
	void ParryEffect::Initialize()
	{
		SetName(L"plDangerUI");
		Transform* UITr = GetComponent<Transform>();
		Resources::Load<Texture>(L"DangerUITexture", L"UI\\Texture\\DangerUITexture.png");
		std::shared_ptr<Texture> parryTexture = Resources::Load<Texture>(L"Parry", L"Effect\\Texture\\Parry.png");
		std::shared_ptr<Texture> horizonTexture = Resources::Load<Texture>(L"Horizon", L"Effect\\Texture\\Horizon.png");

		{
			std::shared_ptr<Shader> resurrectionShader = Resources::Find<Shader>(L"AlphaAnimationShader");
			std::shared_ptr<Material> resurrectionMaterial = std::make_shared<Material>();
			resurrectionMaterial->SetRenderingMode(eRenderingMode::Transparent);
			resurrectionMaterial->SetShader(resurrectionShader);
			Resources::Insert<Material>(L"ResurrectionMaterial2", resurrectionMaterial);

			mMainUI = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mMainUI->SetName(L"UI_Danger_Main");
			mMainUI->AddComponent<PlayerDangerUIScript>();
			SpriteRenderer* renderer = mMainUI->AddComponent<SpriteRenderer>();
			renderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			renderer->SetMaterial(resurrectionMaterial, 0);
			mAnimator = mMainUI->AddComponent<Animator>();
			mAnimator->Create(L"Parry", parryTexture, Vector2(0, 0), Vector2(256, 256), Vector2(0, 0), 1, 1, 0.05f);
			mAnimator->Play(L"Parry", false);
			Transform* hptr = mMainUI->GetComponent<Transform>();
			hptr->SetScale(Vector3(1000, 1000, 50.0f));

			hptr->SetParent(UITr);
			//MeshRenderer* meshRenderer = mMainUI->AddComponent<MeshRenderer>();
			//meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			//std::shared_ptr<Material> mat = Resources::Find<Material>(L"DangerUIMaterial");
			//meshRenderer->SetMaterial(mat, 0);
			//mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"DangerUITexture"));
		}
		{

			mEffectUI = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mEffectUI->SetName(L"UI_Danger_Main");
			mEffectUI->AddComponent<PlayerDangerUIScript>();
			SpriteRenderer* renderer = mEffectUI->AddComponent<SpriteRenderer>();
			renderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			renderer->SetMaterial(Resources::Find<Material>(L"ResurrectionMaterial2"), 0);
			mAnimator2 = mEffectUI->AddComponent<Animator>();
			mAnimator2->Create(L"Parry2", parryTexture, Vector2(0, 0), Vector2(256, 256), Vector2(0, 0), 1, 1, 0.05f);
			mAnimator2->Play(L"Parry2", false);
			Transform* hptr = mEffectUI->GetComponent<Transform>();
			hptr->SetScale(Vector3(1000, 1000, 50.0f));

			hptr->SetParent(UITr);
		}
		{

			mHorizonUI = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mHorizonUI->SetName(L"UI_Danger_Main");
			mHorizonUI->AddComponent<PlayerDangerUIScript>();
			SpriteRenderer* renderer = mHorizonUI->AddComponent<SpriteRenderer>();
			renderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			renderer->SetMaterial(Resources::Find<Material>(L"ResurrectionMaterial2"), 0);
			mAnimator3 = mHorizonUI->AddComponent<Animator>();
			mAnimator3->Create(L"horizon", horizonTexture, Vector2(0, 0), Vector2(364, 27), Vector2(0, 0), 1, 1, 0.05f);
			mAnimator3->Play(L"horizon", false);
			Transform* hptr = mHorizonUI->GetComponent<Transform>();
			hptr->SetScale(Vector3(1600, 80, 50.0f));

			hptr->SetParent(UITr);
		}
		{
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::None, GetScene());
			directionalLight->SetName(L"pointLight");

			directionalLight->GetComponent<Transform>()->SetRotation(Vector3(0, 0.0f, 0.0f));

			mLight= directionalLight->AddComponent<Light>();
			mLight->SetType(eLightType::Point);
			mLight->SetDiffuse(Vector4(1.0f, 0.7f, 0.f, 1.0f));
			mLight->SetRadius(0);
		}
		GameObject::Initialize();
	}
	void ParryEffect::Update()
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

					mHorizonUI->SetRender(false);
					mEffectUI->SetRender(false);
					mMainUI->SetRender(false);
					return;
				}
				mMainUI->GetComponent<Transform>()->SetScale(Vector3(80 + 1000 * mRenderTimeChecker));
				mEffectUI->GetComponent<Transform>()->SetScale(Vector3(50 + 800 * mRenderTimeChecker));
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
				mHorizonUI->SetRender(true);
				mMainUI->SetRender(true);

				Transform* plTr = mPlayer->GetComponent<Transform>();
				mLight->GetOwner()->GetComponent<Transform>()->SetPosition(plTr->GetPosition() + plTr->Forward());

				mLight->SetRadius(2);
			}

			else
			{
				mEffectUI->SetRender(false);
				mHorizonUI->SetRender(false);
				mMainUI->SetRender(false);
				mLight->SetRadius(0);

			}
		}
		if (Input::GetKeyDown(eKeyCode::K))
			UIOn();
		GameObject::Update();
	}
	void ParryEffect::SetPlayer(Player* pl)
	{
		mPlayer = pl;
	}
	void ParryEffect::CBBind()
	{
		renderer::TimeCB data;

		data.time = mRenderTimeChecker;

		data.isMain = true;

		mMainUI->GetScript<PlayerDangerUIScript>()->SetCB(data);


		data.isMain = false;

		mEffectUI->GetScript<PlayerDangerUIScript>()->SetCB(data);
		mHorizonUI->GetScript<PlayerDangerUIScript>()->SetCB(data);
	}

}