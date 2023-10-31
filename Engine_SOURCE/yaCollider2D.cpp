#include "yaCollider2D.h"
#include "yaGameObject.h"
#include "yaRenderer.h"
#include "yaScript.h"

namespace ya
{
	UINT Collider2D::ColliderNumber = 0;
	Collider2D::Collider2D()
		: Component(eComponentType::Collider)
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

	void Collider2D::CollisionUpdate()
	{
		Vector3 scale = mTransform->GetScale();
		scale *= Vector3(mSize.x, mSize.y, mSize.z);

		Vector3 rotation = mTransform->GetRotation();
		rotation += mRotation;

		Vector3 position = mTransform->GetPosition();
		Vector3 colliderPos = position + Vector3(mCenter.x, mCenter.y, mCenter.z);
		mPosition = colliderPos;

		//Matrix scaleMatrix = Matrix::CreateScale(scale);
		//Matrix rotationMatrix;
		//rotationMatrix = Matrix::CreateRotationX(rotation.x);
		//rotationMatrix *= Matrix::CreateRotationY(rotation.y);
		//rotationMatrix *= Matrix::CreateRotationZ(rotation.z);

		//Matrix positionMatrix;
		//positionMatrix.Translation(Vector3(colliderPos.x, colliderPos.y, colliderPos.z));

		//Matrix worldMatrix = scaleMatrix * rotationMatrix * positionMatrix;

		if (renderer::bEnableDebugRender)
		{

			DebugMesh meshAttribute = {};
			meshAttribute.position = Vector3(colliderPos.x, colliderPos.y, colliderPos.z);
			meshAttribute.radius = mRadius;
			meshAttribute.rotation = rotation;
			meshAttribute.scale = scale;
			meshAttribute.parent = mTransform->GetParent();
			meshAttribute.type = mType;

			renderer::debugMeshes.push_back(meshAttribute);
		}
	}

	void Collider2D::FixedUpdate()
	{
	}

	void Collider2D::Render()
	{
	}

	void Collider2D::OnCollisionEnter(Collider2D* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionEnter(collider);
		}
	}

	void Collider2D::OnCollisionStay(Collider2D* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionStay(collider);
		}
	}

	void Collider2D::OnCollisionExit(Collider2D* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionExit(collider);
		}
	}

	void Collider2D::OnTriggerEnter(Collider2D* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerEnter(collider);
		}
	}

	void Collider2D::OnTriggerStay(Collider2D* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerStay(collider);
		}
	}

	void Collider2D::OnTriggerExit(Collider2D* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerExit(collider);
		}
	}

	Vector3 Collider2D::GetWorldScale()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		return tr->GetScale() * mSize;
	}

}
