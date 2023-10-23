#include "yaLight.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaRenderer.h"
#include "yaResources.h"
#include "yaMaterial.h"
namespace ya
{
	Light::Light()
		: Component(eComponentType::Light)
	{
		renderer::lights.push_back(this);
	}

	Light::~Light()
	{

	}

	void Light::Initialize()
	{

	}

	void Light::Update()
	{

	}

	void Light::FixedUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		if (eLightType::Point == mAttribute.type)
		{
			tr->SetLocalScale(Vector3(mAttribute.radius * 5.f, mAttribute.radius * 5.f, mAttribute.radius * 5.f));
		}
		Vector3 position = tr->GetLocalPosition();
		mAttribute.position = Vector4(position.x, position.y, position.z, 1.0f);
		mAttribute.direction = Vector4(tr->Forward().x, tr->Forward().y, tr->Forward().z, 0.0f);
		
		//mAttribute.type = enums::eLightType::Directional;

		renderer::PushLightAttribute(mAttribute);
	}

	void Light::Render()
	{

		std::shared_ptr<Material> material = nullptr;

		if (mAttribute.type == eLightType::Directional)
		{
			material = Resources::Find<Material>(L"LightMaterial");
		}
		else if (mAttribute.type == eLightType::Point)
		{
			material = Resources::Find<Material>(L"LightPointMaterial");
		}


		if (material == nullptr)
			return;

		Transform* tr = GetOwner()->GetComponent<Transform>();
		tr->SetConstantBuffer();

		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Light];
		LightCB data = {};
		data.numberOfLight = (UINT)renderer::lights.size();
		data.indexOfLight = mIndex;

		cb->SetData(&data);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);


		mVolumeMesh->BindBuffer(0);
		material->Bind();
		mVolumeMesh->Render(0);
	}
	void Light::SetType(eLightType type)
	{
		mAttribute.type = type;
		if (mAttribute.type == eLightType::Directional)
		{
			mVolumeMesh = Resources::Find<Mesh>(L"RectMesh");
		}
		else if (mAttribute.type == eLightType::Point)
		{
			mVolumeMesh = Resources::Find<Mesh>(L"SphereMesh");
		}
		else if (mAttribute.type == eLightType::Spot)
		{
			//
		}
	}
}
