#include "yaLight.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaRenderer.h"

namespace ya
{
	Light::Light()
		: Component(eComponentType::Light)
	{

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
		Vector3 position = tr->GetPosition();
		mAttribute.position = Vector4(position.x, position.y, position.z, 1.0f);
		mAttribute.direction = Vector4(tr->Forward().x, tr->Forward().y, tr->Forward().z, 0.0f);
		
		//mAttribute.type = enums::eLightType::Directional;

		renderer::PushLightAttribute(mAttribute);
	}

	void Light::Render()
	{

	}
}
