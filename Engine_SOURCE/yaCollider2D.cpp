//#include "yaCollider2D.h"
//#include "yaGameObject.h"
//#include "yaRenderer.h"
//#include "yaScript.h"
//
//namespace ya
//{
//	UINT Collider3D::ColliderNumber = 0;
//	Collider3D::Collider3D()
//		: Collider(eComponentType::Collider)
//		, mType(eColliderType::None)
//		, mTransform(nullptr)
//		, mSize(Vector3::One)
//		, mCenter(Vector3::Zero)
//		, mRotation(Vector3::Zero)
//		, mbTrigger(false)
//		, mID(0)
//		, mRadius(0.0f)
//		, mbActive(true)
//	{
//		mID = ColliderNumber++;
//	}
//
//	Collider3D::~Collider3D()
//	{
//	}
//
// 
//	void Collider3D::Initialize()
//	{
//		mTransform = GetOwner()->GetComponent<Transform>();
//	}
//
//	void Collider3D::Update()
//	{
//	}
//
//	void Collider3D::FixedUpdate()
//	{
//		if (!mbActive)
//			return;
//		Vector3 scale = mTransform->GetLocalScale();
//		scale *= Vector3(mSize.x, mSize.y, mSize.z);
//
//		Vector3 rotation = mTransform->GetLocalRotation();
//		rotation += mRotation;
//
//		Vector3 position = mTransform->GetLocalPosition();
//		Vector3 colliderPos = position + Vector3(mCenter.x, mCenter.y, mCenter.z);
//		mPosition = colliderPos;
//
//		Matrix scaleMatrix = Matrix::CreateScale(scale);
//		Matrix rotationMatrix;
//		rotationMatrix = Matrix::CreateRotationX(rotation.x);
//		rotationMatrix *= Matrix::CreateRotationY(rotation.y);
//		rotationMatrix *= Matrix::CreateRotationZ(rotation.z);
//
//		Matrix positionMatrix;
//		positionMatrix.Translation(Vector3(colliderPos.x, colliderPos.y, colliderPos.z));
//
//		Matrix worldMatrix = scaleMatrix * rotationMatrix * positionMatrix;
//
//		DebugMesh meshAttribute = {};
//		meshAttribute.position = Vector3(colliderPos.x, colliderPos.y, colliderPos.z);
//		meshAttribute.radius = mRadius;
//		meshAttribute.rotation = rotation;
//		meshAttribute.scale = scale;
//		meshAttribute.parent = mTransform->GetParent();
//		meshAttribute.type = mType;
//
//		renderer::debugMeshes.push_back(meshAttribute);
//	}
//
//	void Collider3D::Render()
//	{
//	}
//
//	void Collider3D::OnCollisionEnter(Collider3D* collider)
//	{
//		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
//		GameObject* otherObj = collider->GetOwner();
//		for (Script* script : scripts)
//		{
//			script->OnCollisionEnter(otherObj, Vector3::Zero);
//		}
//	}
//
//	void Collider3D::OnCollisionStay(Collider3D* collider)
//	{
//		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
//		GameObject* otherObj = collider->GetOwner();
//		for (Script* script : scripts)
//		{
//			script->OnCollisionStay(otherObj, Vector3::Zero);
//		}
//	}
//
//	void Collider3D::OnCollisionExit(Collider3D* collider)
//	{
//		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
//		GameObject* otherObj = collider->GetOwner();
//		for (Script* script : scripts)
//		{
//			script->OnCollisionExit(otherObj, Vector3::Zero);
//		}
//	}
//
//	void Collider3D::OnTriggerEnter(Collider3D* collider)
//	{
//		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
//		GameObject* otherObj = collider->GetOwner();
//		for (Script* script : scripts)
//		{
//			script->OnTriggerEnter(otherObj);
//		}
//	}
//
//	//void Collider3D::OnTriggerStay(Collider3D* collider)
//	//{
//	//	const std::vector<Script*>& scripts = GetOwner()->GetScripts();
//	//	for (Script* script : scripts)
//	//	{
//	//		script->OnTriggerStay(collider);
//	//	}
//	//}
//
//	void Collider3D::OnTriggerExit(Collider3D* collider)
//	{
//		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
//		GameObject* otherObj = collider->GetOwner();
//		for (Script* script : scripts)
//		{
//			script->OnTriggerExit(otherObj);
//		}
//	}
//
//}
