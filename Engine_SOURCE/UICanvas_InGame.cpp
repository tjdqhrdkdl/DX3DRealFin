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

		auto InGameUITex = Resources::Load<Texture>(L"InGameUI", L"Texture\\UI\\SB_FE.png");

		assert(nullptr != InGameUITex);

		MeshRenderer* meshRenderer = GetComponent<MeshRenderer>();
		meshRenderer->GetMaterial(0u)->SetTexture((eTextureSlot)eInGameUITextureSlot::MainHudTexture, InGameUITex);

		tUI_UVPos hpbar{};
		hpbar.TextureSlot = eInGameUITextureSlot::MainHudTexture;
		hpbar.UVBegin = Vector2(0.1f, 0.1f);
		hpbar.UVEnd = Vector2(0.2f, 0.2f);

		mMapUI.insert(std::make_pair(L"HPBar", hpbar));
	}
}

