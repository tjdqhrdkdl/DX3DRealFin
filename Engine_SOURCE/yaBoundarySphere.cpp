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
		mMaxScale = scale.x;
		mMaxScale = max(mMaxScale, scale.y);
		mMaxScale = max(mMaxScale, scale.z);
		Vector3 rotation = mTransform->GetRotation();

		Vector3 position = mTransform->GetPosition();
		Vector3 centerPos = position + Vector3(mCenter.x * scale.x, mCenter.y * scale.y, mCenter.z * scale.z);

		DebugMesh meshAttribute = {};
		meshAttribute.position = position;
		meshAttribute.radius = mRadius * mMaxScale;
		meshAttribute.rotation = rotation;
		meshAttribute.rotationOffset = Vector3(mCenter.x * scale.x, mCenter.y * scale.y, mCenter.z * scale.z);
		meshAttribute.parent = mTransform->GetParent();
		meshAttribute.type = eColliderType::Sphere;

		Vector4 pos = Vector4::Transform(Vector4(mCenter.x, mCenter.y, mCenter.z, 1), mTransform->GetWorldMatrix());
		mPosition = Vector3(pos.x, pos.y, pos.z);
		scale = mTransform->GetFinalScale();

		mMaxScale = scale.x;
		mMaxScale = max(mMaxScale, scale.y);
		mMaxScale = max(mMaxScale, scale.z);
		//renderer::debugMeshes.push_back(meshAttribute);
	}
	void BoundarySphere::Render()
	{
	}
	void BoundarySphere::Update()
	{
	}
}