#include "yaBoundarySphere.h"
#include "yaGameObject.h"
#include "yaRenderer.h"
#include "yaTransform.h"

namespace ya
{
	BoundarySphere::BoundarySphere() 
		: Component(eComponentType::BoundarySphere)
		, mRadius(2.0f)
		, mCenter(Vector3::Zero)
	{
	}
	BoundarySphere::~BoundarySphere()
	{
	}
	void BoundarySphere::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();

	}
	void BoundarySphere::FixedUpdate()
	{
		Vector3 scale = mTransform->GetScale();
		float maxScale = scale.x;
		maxScale = max(maxScale, scale.y);
		maxScale = max(maxScale, scale.z);

		Vector3 position = mTransform->GetPosition();
		Vector3 centerPos = position + Vector3(mCenter.x, mCenter.y, mCenter.z);

		DebugMesh meshAttribute = {};
		meshAttribute.position = Vector3(centerPos.x, centerPos.y, centerPos.z);
		meshAttribute.radius = mRadius * maxScale;
		meshAttribute.parent = mTransform->GetParent();
		meshAttribute.type = eColliderType::Sphere;

		renderer::debugMeshes.push_back(meshAttribute);
	}
	void BoundarySphere::Render()
	{
	}
	void BoundarySphere::Update()
	{
	}
}