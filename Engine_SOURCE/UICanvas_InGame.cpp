#include "UICanvas_InGame.h"
#include "yaResources.h"
#include "yaTexture.h"
#include "yaMeshRenderer.h"

#include "UIHPBarScript.h"

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

		AddComponent<UIHPBarScript>();
	}

	void UICanvas_InGame::Render()
	{

	}


	void UICanvas_InGame::CreateHP_HUD()
	{
		auto InGameUITex = Resources::Load<Texture>(L"InGameMainHUDTexture", L"Texture\\UI\\SB_In_Game.png");
		assert(nullptr != InGameUITex);
		MeshRenderer* meshRenderer = GetRenderer<MeshRenderer>();
		meshRenderer->GetMaterial(0u)->SetTexture((eTextureSlot)InGameUITextures::HUD_Player, InGameUITex);

		UICanvasObj::AddUIInfo(UI::InGame::HPBar_BackGround, InGameUITextures::HUD_Player, Vector2(1512.f, 1974.f), Vector2(1890.f, 2013.f));

		UICanvasObj::AddUIInfo(UI::InGame::HPBar, InGameUITextures::HUD_Player, Vector2(1222.f, 1349.f), Vector2(1634.f, 1383.f));
	}

	void UICanvas_InGame::CreateGeneralHUD()
	{
	}
}

