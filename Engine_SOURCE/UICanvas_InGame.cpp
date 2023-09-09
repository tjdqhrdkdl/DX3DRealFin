#include "UICanvas_InGame.h"
#include "yaResources.h"
#include "yaTexture.h"
#include "yaMeshRenderer.h"

namespace ya
{
	using namespace graphics;
	UICanvas_InGame::UICanvas_InGame()
	{
	}

	UICanvas_InGame::~UICanvas_InGame()
	{
	}

	void UICanvas_InGame::Initialize()
	{
		UICanvasObj::Initialize();

		auto InGameUITex = Resources::Load<Texture>(L"InGameMainHUDTexture", L"Texture\\UI\\SB_FE.png");

		assert(nullptr != InGameUITex);

		MeshRenderer* meshRenderer = GetComponent<MeshRenderer>();
		meshRenderer->GetMaterial(0u)->SetTexture((eTextureSlot)eInGameUITextureSlot::MainHudTexture, InGameUITex);

		tUIInfo hpbar{};
		hpbar.TextureSlot = (int)eInGameUITextureSlot::MainHudTexture;
		hpbar.UVBeginInTexture = Vector2(0.1f, 0.1f);
		hpbar.UVSizeInTexture = Vector2(0.5f, 0.5f);

		AddUIInfo(L"HPBar", hpbar);
	}

	void UICanvas_InGame::Render()
	{
		const tUIInfo* pUIInfo = FindUIInfo(L"HPBar");
		if (nullptr == pUIInfo)
		{
			return;
		}

		tUIRenderInfo renderInfo{};
		renderInfo.UIInfo = *pUIInfo;
		renderInfo.UVBeginInCanvas = Vector2(0.5f, 0.5f);
		renderInfo.UVSizeInCanvas = Vector2(0.2f, 0.2f);

		RenderUI(renderInfo);

		UICanvasObj::Render();
	}
}

