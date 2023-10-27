#include "guiTransform.h"
#include "yaTransform.h"
#include "yaCollider2D.h"
#include "yaRenderer.h"
namespace gui
{
	Transform::Transform()
		: Component(eComponentType::Transform)
	{
		SetName("Transform");
		SetSize(ImVec2(400.0f, 300.0f));
	}

	Transform::~Transform()
	{

	}

	void Transform::FixedUpdate()
	{
		Component::FixedUpdate();

	
		if (GetTarget() == nullptr)
			return;

		ya::Transform* tr = GetTarget()->GetComponent<ya::Transform>();
		ya::Collider3D* col = GetTarget()->GetComponent<ya::Collider3D>();

		mPosition = tr->GetLocalPosition();
		mRotation = tr->GetLocalRotation();
		mScale = tr->GetLocalScale();
		mRotationOffset = tr->GetRotationOffset();

		mForward = tr->Forward();
		mRight = tr->Right();
		mUp = tr->Up();

		mColliderCenter = ya::math::Vector3::Zero;
		if (col)
			mColliderCenter = col->GetCenter();
	}

	void Transform::Update()
	{
		Component::Update();

		ImGui::Text("Position"); ImGui::SameLine();
		ImGui::InputFloat3("##Position", (float*)&mPosition);

		ImGui::Text("Rotation"); ImGui::SameLine();
		ImGui::InputFloat3("##Rotation", (float*)&mRotation);

		ImGui::Text("Scale"); ImGui::SameLine();
		ImGui::InputFloat3("##Scale", (float*)&mScale);

		ImGui::Text("RotOffset"); ImGui::SameLine();
		ImGui::InputFloat3("##RotOffset", (float*)&mRotationOffset);
		
		ImGui::Text("Foward"); ImGui::SameLine();
		ImGui::InputFloat3("##Forward", (float*)&mForward);

		ImGui::Text("Right"); ImGui::SameLine();
		ImGui::InputFloat3("##Right", (float*)&mRight);

		ImGui::Text("Up"); ImGui::SameLine();
		ImGui::InputFloat3("##Up", (float*)&mUp);

		ImGui::Text("ColliderCenter"); ImGui::SameLine();
		ImGui::InputFloat3("##ColliderCenter", (float*)&mColliderCenter);

		if (ya::renderer::mainCamera)
		{
			ya::Transform* camTr = ya::renderer::mainCamera->GetOwner()->GetComponent<ya::Transform>();
			ya::math::Vector3 camPos = camTr->GetLocalPosition();
			ImGui::Text("CameraPosition"); ImGui::SameLine();
			ImGui::InputFloat3("##CameraPosition", (float*)&(camPos));
		}


		if (GetTarget())
		{
			ya::Transform* tr = GetTarget()->GetComponent<ya::Transform>();
			//ya::Collider2D* col = GetTarget()->GetComponent<ya::Collider2D>();

			tr->SetLocalPosition(mPosition);
			tr->SetLocalRotation(mRotation);
			tr->SetLocalScale(mScale);
			tr->SetRotationOffset(mRotationOffset);

			tr->SetForward(mForward);
			tr->SetRight(mRight);
			tr->SetUp(mUp);

			//if (col)
			{
				//col->SetSize();
				//col->SetCenter(mColliderCenter);
			}
		}
	}

	void Transform::LateUpdate()
	{
		Component::LateUpdate();

	}
}
