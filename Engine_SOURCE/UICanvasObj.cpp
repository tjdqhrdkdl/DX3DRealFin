#include "UICanvasObj.h"
#include "yaMeshRenderer.h"
#include "yaResources.h"
#include "CommonInclude.h"

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

		

		std::sort(mUIRenderQueue.begin(), mUIRenderQueue.end(),
			[](const tUIRenderInfo& _left, const tUIRenderInfo& _right)->bool
			{
				return (_left.ZValue > _right.ZValue);
			}
		);

		//GPU에 데이터 보내고
		mUIDrawSBuffer->SetData(mUIRenderQueue.data(), (UINT)mUIRenderQueue.size());
		mUIDrawSBuffer->BindSRV(eShaderStage::PS, Register_UIBuffer);

		//렌더
		GameObject::Render();

		//데이터 삭제
		mUIRenderQueue.clear();
		mUIDrawSBuffer->Clear();
	}






	bool UICanvasObj::AddUIInfo(const std::wstring& _name, int _texSlot, const Vector2& _start, const Vector2& _end)
	{
		auto iter = mMapUIInfo.find(_name);
		if (iter != mMapUIInfo.end())
		{
			std::wstring err = _name;
			err += L"\n이미 추가되어 있습니다.";
			NOTIFICATION_W(err.c_str());
			return false;
		}


		MeshRenderer* renderer = GetRenderer<MeshRenderer>();
		assert(renderer);
		std::shared_ptr<Material> mtrl = renderer->GetMaterial(0);
		assert(mtrl.get());
		std::shared_ptr<Texture> tex = mtrl->GetTexture((eTextureSlot)_texSlot);
		
		if (nullptr == tex)
		{
			NOTIFICATION_W(L"해당 슬롯에 지정한 텍스처가 없습니다.");
			return false;
		}

		const Vector2& texSize = tex->GetSize();

		//문제 있는지 체크
		if (
			_start.x >= _end.x
			|| _start.y >= _end.y
			|| texSize.x <= _start.x
			|| texSize.y <= _start.y
			|| texSize.x <= _end.x
			|| texSize.y <= _end.y
			)
		{
			NOTIFICATION_W(L"지정한 좌표가 텍스처 실제 사이즈보다 큽니다.");
			return false;
		}

		tUIInfo info{};
		info.TextureSlot = _texSlot;
		info.UVBeginInTexture = _start / texSize;
		info.UVSizeInTexture = (_end - _start) / texSize;
		
		mMapUIInfo.insert(std::make_pair(_name, info));

		return true;
	}
}