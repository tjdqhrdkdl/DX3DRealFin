#include "UICanvasObj.h"
#include "yaMeshRenderer.h"
#include "yaResources.h"

namespace ya
{
	UICanvasObj::UICanvasObj()
	{
	}
	UICanvasObj::~UICanvasObj()
	{
	}
	void UICanvasObj::Initialize()
	{
		MeshRenderer* renderer = AddComponent<MeshRenderer>();
		
		auto mesh = Resources::Find<Mesh>(L"RectMesh");
		auto material = Resources::Find<Material>(L"UIMaterial")->Clone();

		renderer->SetMesh(mesh);
		renderer->SetMaterial(material, 0);

		GameObject::Initialize();
	}
	void UICanvasObj::Update()
	{
		GameObject::Update();
	}
	void UICanvasObj::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	void UICanvasObj::Render()
	{
		GameObject::Render();
	}
}