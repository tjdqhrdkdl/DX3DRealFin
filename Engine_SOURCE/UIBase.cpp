#include "UIBase.h"

namespace ya
{
	UIBase::UIBase()
	{
	}
	UIBase::~UIBase()
	{
	}
	void UIBase::Initialize()
	{
		GameObject::Initialize();




	}
	void UIBase::Update()
	{
		GameObject::Update();
	}
	void UIBase::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	void UIBase::Render()
	{
		GameObject::Render();
	}
}