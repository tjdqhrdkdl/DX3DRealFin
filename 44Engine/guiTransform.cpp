#include "guiTransform.h"
#include "yaTransform.h"

namespace gui
{
	Transform::Transform()
		: Component(eComponentType::Transform)
	{
		SetName("Transform");
		SetSize(ImVec2(200.0f, 200.0f));
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

		mPosisition = tr->GetPosition();
		mRotation = tr->GetRotation();
		mScale = tr->GetScale();

		mForward = tr->Forward();
		mRight = tr->Right();
		mUp = tr->Up();
	}

	void Transform::Update()
	{
		Component::Update();

		ImGui::Text("Position"); ImGui::SameLine();
		ImGui::InputFloat3("##Position", (float*)&mPosisition);

		ImGui::Text("Rotation"); ImGui::SameLine();
		ImGui::InputFloat3("##Rotation", (float*)&mRotation);

		ImGui::Text("Scale"); ImGui::SameLine();
		ImGui::InputFloat3("##Scale", (float*)&mScale);

		ImGui::NewLine();

		ImGui::Text("Forward"); ImGui::SameLine();
		ImGui::InputFloat3("##Forward", (float*)&mForward);

		ImGui::Text("Right"); ImGui::SameLine();
		ImGui::InputFloat3("##Right", (float*)&mRight);

		ImGui::Text("Up"); ImGui::SameLine();
		ImGui::InputFloat3("##Up", (float*)&mUp);

		if (GetTarget())
		{
			ya::Transform* tr = GetTarget()->GetComponent<ya::Transform>();

			tr->SetPosition(mPosisition);
			tr->SetRotation(mRotation);
			tr->SetScale(mScale);

			tr->SetForward(mForward);
			tr->SetRight(mRight);
			tr->SetUp(mUp);
		}
	}

	void Transform::LateUpdate()
	{
		Component::LateUpdate();

	}
}
