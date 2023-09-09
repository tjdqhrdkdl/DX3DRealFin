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

		CreateHP_HUD();
		CreateGeneralHUD();
	}

	void UICanvas_InGame::Render()
	{
		const tUIInfo* pUIInfo = FindUIInfo(L"HPBarBackGround");

		tUIRenderInfo renderInfo{};
		renderInfo.UIInfo = *pUIInfo;
		renderInfo.UVBeginInCanvas = Vector2(0.5f, 0.5f);
		renderInfo.UVSizeInCanvas = Vector2(0.3f, 0.1f);
		RenderUI(renderInfo);

		pUIInfo = FindUIInfo(L"HPBar");

		renderInfo = {};
		renderInfo.UIInfo = *pUIInfo;
		renderInfo.UVBeginInCanvas = Vector2(0.5f, 0.5f);
		renderInfo.UVSizeInCanvas = Vector2(0.3f, 0.1f);

		RenderUI(renderInfo);

		UICanvasObj::Render();
	}

	void UICanvas_InGame::CreateHP_HUD()
	{
		auto InGameUITex = Resources::Load<Texture>(L"InGameMainHUDTexture", L"Texture\\UI\\SB_In_Game.png");
		assert(nullptr != InGameUITex);
		MeshRenderer* meshRenderer = GetComponent<MeshRenderer>();
		meshRenderer->GetMaterial(0u)->SetTexture((eTextureSlot)InGameUITextures::HP_HUD, InGameUITex);

		UICanvasObj::AddUIInfo(L"HPBarBackGround", InGameUITextures::HP_HUD, Vector2(1512.f, 1974.f), Vector2(1890.f, 2013.f));

		UICanvasObj::AddUIInfo(L"HPBar", InGameUITextures::HP_HUD, Vector2(1222.f, 1349.f), Vector2(1634.f, 1383.f));

	}

	void UICanvas_InGame::CreateGeneralHUD()
	{
	}
}

