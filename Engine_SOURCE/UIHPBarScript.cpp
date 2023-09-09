#include "UIHPBarScript.h"
#include "yaSceneManager.h"
#include "yaPlayer.h"
#include "UICanvas_InGame.h"

namespace ya
{
	UIHPBarScript::UIHPBarScript()
		: mPlayer()
	{
	}
	UIHPBarScript::~UIHPBarScript()
	{
	}

	void UIHPBarScript::Initialize()
	{

	}

	void UIHPBarScript::Update()
	{
		Scene* scene = SceneManager::GetActiveScene();
		if (scene)
		{
			mPlayer = scene->GetPlayer();
		}


		if (mPlayer)
		{
			//HP
		}


		UICanvas_InGame* canvas = static_cast<UICanvas_InGame*>(GetOwner());
		canvas->RenderUI(UI::InGame::HPBar_BackGround, Vector2(100.f, 50.f), Vector2(100.f, 50.f), 0.1f);
		canvas->RenderUI(UI::InGame::HPBar, Vector2(100.f, 50.f), Vector2(100.f, 50.f), 0.2f);
	}
}