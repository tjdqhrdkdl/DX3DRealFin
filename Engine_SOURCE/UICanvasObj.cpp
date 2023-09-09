#include "UICanvasObj.h"
#include "yaMeshRenderer.h"
#include "yaResources.h"


#define UISBufferSize int_0

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
		mUIDrawSBuffer->Create((UINT)sizeof(tUIRenderInfo), 100u, eSRVType::SRV, nullptr, true);

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
		//렌더링할 UI의 갯수를 보내준다
		UniformDataCB cb{};
		cb.UISBufferSize = (int)mUIRenderQueue.size();
		renderer::constantBuffers[(int)eCBType::UniformData]->SetData(&cb);
		renderer::constantBuffers[(int)eCBType::UniformData]->Bind(eShaderStage::PS);

		//사이즈 초과 시 더 크게 생성
		if (mUIDrawSBuffer->GetSize() < mUIRenderQueue.size())
		{
			mUIDrawSBuffer->Create((UINT)mUIRenderQueue.size() * 2u, (UINT)sizeof(tUIRenderInfo), eSRVType::SRV, nullptr);
		}

		//GPU에 데이터 보내고
		mUIDrawSBuffer->SetData(mUIRenderQueue.data(), mUIRenderQueue.size());
		mUIDrawSBuffer->BindSRV(eShaderStage::PS, Register_UIBuffer);

		//렌더
		GameObject::Render();

		//데이터 삭제
		mUIRenderQueue.clear();
		mUIDrawSBuffer->Clear();
	}


	inline const tUIInfo* UICanvasObj::FindUIInfo(const std::wstring& _UIName)
	{
		const tUIInfo* uiInfo = nullptr;
		const auto& iter = mMapUI.find(_UIName);
		if (iter != mMapUI.end())
		{
			uiInfo = &iter->second;
		}

		return uiInfo;
	}
}