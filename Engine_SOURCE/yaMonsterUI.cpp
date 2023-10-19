#include "yaMonsterUI.h"
#include "yaObject.h"
#include "yaMeshRenderer.h"
#include "yaResources.h"
#include "yaMonsterBase.h"
#include "yaHPMeterScript.h"

#define PERCENTAGE 0.5

namespace ya
{
	MonsterUI::MonsterUI()
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

			mMonsterpostureLayout = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			mMonsterpostureLayout->SetName(L"postureLayout");

			Transform* postureLayouttr = mMonsterpostureLayout->GetComponent<Transform>();

			postureLayouttr->SetScale(Vector3(100.0f, 5.0f, 50.0f));
			postureLayouttr->SetPosition(Vector3(0, -10.0f, 0));
			postureLayouttr->SetParent(UITr);

			MeshRenderer* meshRenderer = mMonsterpostureLayout->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"PostureLayoutMaterial");

			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"PostureLayout"));

		}

		{
			GameObject* PlayerpostureBar = object::Instantiate<GameObject>(eLayerType::UI, GetScene());
			PlayerpostureBar->SetName(L"PostureBar1");

			Transform* postureBartr = PlayerpostureBar->GetComponent<Transform>();

			postureBartr->SetScale(Vector3(115.0f, 5.0f, 50.0f));
			postureBartr->SetPosition(Vector3(0, -10.0f, 0));
			postureBartr->SetRotation(Vector3(0.0f, 180.0f, 0.0f));

			postureBartr->SetParent(UITr);

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
			postureBartr->SetPosition(Vector3(0, -10.0f, 0));

			postureBartr->SetScale(Vector3(115.0f, 5.0f, 50.0f));
			postureBartr->SetParent(UITr);

			MeshRenderer* meshRenderer = PlayerpostureBar->AddComponent<MeshRenderer>();
			meshRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			std::shared_ptr<Material> mat = Resources::Find<Material>(L"PostureBarMaterial");

			meshRenderer->SetMaterial(mat, 0);
			mat->SetTexture(eTextureSlot::Albedo, Resources::Find<Texture>(L"PostureBar"));

		}
		GameObject::Initialize();
	}
	void MonsterUI::Update()
	{
		if (mMonster)
		{

			Matrix world = mMonster->GetComponent<Transform>()->GetWorldMatrix();
			world._42 += 3;
			Matrix fin = world * mainCamera->GetViewMatrix();
			fin *= mainCamera->GetProjectionMatrix();
			Vector4 ndc = Vector4::Transform(Vector4(0, 0, 0, 1), fin);
			ndc = ndc / ndc.w;

			Vector4 UIPos = Vector4::Transform(ndc, UICamera->GetProjectionMatrix().Invert());
			UIPos = Vector4::Transform(UIPos, UICamera->GetViewMatrix().Invert());
			GetComponent<Transform>()->SetPosition(Vector3(UIPos.x, UIPos.y, 0.00001));
			if (ndc.z > 1 or ndc.z < 0)
				GetComponent<Transform>()->SetPosition(Vector3(50000, -50000, 0));

			MonsterMeterCheak();
		}
		GameObject::Update();
	}
	void MonsterUI::MonsterMeterCheak()
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