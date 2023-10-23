#include "yaCollider2D.h"
#include "yaGameObject.h"
#include "yaRenderer.h"
#include "yaScript.h"

namespace ya
{
	UINT Collider2D::ColliderNumber = 0;
	Collider2D::Collider2D()
		: Collider(eComponentType::Collider)
		, mType(eColliderType::None)
		, mTransform(nullptr)
		, mSize(Vector3::One)
		, mCenter(Vector3::Zero)
		, mRotation(Vector3::Zero)
		, mbTrigger(false)
		, mID(0)
		, mRadius(0.0f)
		, mbActive(true)
	{
		mID = ColliderNumber++;
	}

	Collider2D::~Collider2D()
	{
	}

	void Collider2D::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();
	}

	void Collider2D::Update()
	{
	}

	void Collider2D::FixedUpdate()
	{
		if (!mbActive)
			return;
		Vector3 scale = mTransform->GetLocalScale();
		scale *= Vector3(mSize.x, mSize.y, mSize.z);

		Vector3 rotation = mTransform->GetLocalRotation();
		rotation += mRotation;

		Vector3 position = mTransform->GetLocalPosition();
		Vector3 colliderPos = position + Vector3(mCenter.x, mCenter.y, mCenter.z);
		mPosition = colliderPos;

		Matrix scaleMatrix = Matrix::CreateScale(scale);
		Matrix rotationMatrix;
		rotationMatrix = Matrix::CreateRotationX(rotation.x);
		rotationMatrix *= Matrix::CreateRotationY(rotation.y);
		rotationMatrix *= Matrix::CreateRotationZ(rotation.z);

		Matrix positionMatrix;
		positionMatrix.Translation(Vector3(colliderPos.x, colliderPos.y, colliderPos.z));

		Matrix worldMatrix = scaleMatrix * rotationMatrix * positionMatrix;

		DebugMesh meshAttribute = {};
		meshAttribute.position = Vector3(colliderPos.x, colliderPos.y, colliderPos.z);
		meshAttribute.radius = mRadius;
		meshAttribute.rotation = rotation;
		meshAttribute.scale = scale;
		meshAttribute.parent = mTransform->GetParent();
		meshAttribute.type = mType;

		renderer::debugMeshes.push_back(meshAttribute);
	}

	void Collider2D::Render()
	{
	}

	void Collider2D::OnCollisionEnter(Collider2D* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		GameObject* otherObj = collider->GetOwner();
		for (Script* script : scripts)
		{
			script->OnCollisionEnter(otherObj, Vector3::Zero);
		}
	}

	void Collider2D::OnCollisionStay(Collider2D* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		GameObject* otherObj = collider->GetOwner();
		for (Script* script : scripts)
		{
			script->OnCollisionStay(otherObj, Vector3::Zero);
		}
	}

	void Collider2D::OnCollisionExit(Collider2D* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		GameObject* otherObj = collider->GetOwner();
		for (Script* script : scripts)
		{
			script->OnCollisionExit(otherObj, Vector3::Zero);
		}
	}

	void Collider2D::OnTriggerEnter(Collider2D* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		GameObject* otherObj = collider->GetOwner();
		for (Script* script : scripts)
		{
			script->OnTriggerEnter(otherObj);
		}
	}

	//void Collider2D::OnTriggerStay(Collider2D* collider)
	//{
	//	const std::vector<Script*>& scripts = GetOwner()->GetScripts();
	//	for (Script* script : scripts)
	//	{
	//		script->OnTriggerStay(collider);
	//	}
	//}

	void Collider2D::OnTriggerExit(Collider2D* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		GameObject* otherObj = collider->GetOwner();
		for (Script* script : scripts)
		{
			script->OnTriggerExit(otherObj);
		}
	}

}
