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

		mUIDrawSBuffer = std::make_unique<graphics::StructedBuffer>();
		mUIDrawSBuffer->Create(100u, (UINT)sizeof(tUIRenderInfo), eSRVType::SRV, nullptr);

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
		//사이즈 초과 시 더 크게 생성
		if (mUIDrawSBuffer->GetSize() < mUIRenderQueue.size())
		{
			mUIDrawSBuffer->Create((UINT)mUIRenderQueue.size() * 2u, (UINT)sizeof(tUIRenderInfo), eSRVType::SRV, nullptr);
		}

		

		//GPU에 데이터 보내고
		mUIDrawSBuffer->SetData(mUIRenderQueue.data(), mUIRenderQueue.size());
		mUIDrawSBuffer->BindSRV(eShaderStage::PS, Register_UIBuffer);

		//데이터 삭제
		mUIRenderQueue.clear();

		//렌더
		GameObject::Render();

		mUIDrawSBuffer->Clear();
	}

}