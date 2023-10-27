#include "guiHierarchy.h"
#include "yaApplication.h"
#include "yaScene.h"
#include "yaLayer.h"
#include "yaSceneManager.h"
#include "guiInspector.h"
#include "guiEditor.h"
#include "yaRenderer.h"
#include "guiInspector.h"
#include "StrConverter.h"

extern ya::Application application;
extern gui::Editor editor;

namespace gui
{
	Hierarchy::Hierarchy()
		: mTreeWidget(nullptr)
		, mCurInspectingScene(nullptr)
	{
		SetName("Hierarchy");
		SetSize(ImVec2(1600 / 2, 900 / 2));

		mTreeWidget = new TreeWidget();
		mTreeWidget->SetName("Scenes");
		AddWidget(mTreeWidget);

		mTreeWidget->SetEvent(this
			, std::bind(&Hierarchy::InitializeInspector, this, std::placeholders::_1));

		mTreeWidget->SetDummyRoot(true);
	}

	Hierarchy::~Hierarchy()
	{
		delete mTreeWidget;
		mTreeWidget = nullptr;
	}

	void Hierarchy::FixedUpdate()
	{
	}

	void Hierarchy::Update()
	{
		if (nullptr == mCurInspectingScene)
		{
			InitializeScene(eSceneType::Play);
		}
	}

	void Hierarchy::LateUpdate()
	{
	}

	void Hierarchy::InitializeInspector(void* data)
	{
		ya::renderer::inspectorGameObject 
			= static_cast<ya::GameObject*>(data);

		Inspector* inspector = editor.GetWidget<Inspector>("Inspector");
		inspector->SetTargetGameObject(ya::renderer::inspectorGameObject);
		inspector->InitializeTargetGameObject();
	}

	void Hierarchy::InitializeScene(ya::enums::eSceneType _sceneType)
	{
		ya::Scene* scene = ya::SceneManager::GetScene(ya::enums::eSceneType::Play);

		if (nullptr == scene || false == scene->IsLoadComplete())
		{
			//mCurInspectingScene = nullptr;
			return;
		}

		mCurInspectingScene = scene;
		mTreeWidget->Clear();

		//mSceneName  = StrConverter::ConvertUnicodeToANSI(scene->GetName());
		//std::string sceneName(scene->GetName().begin(), scene->GetName().end());

		TreeWidget::Node* root = mTreeWidget->AddNode(nullptr, mCurInspectingScene->GetNameChar(), 0, true);

		for (size_t i = 0; i < (UINT)ya::enums::eLayerType::End; i++)
		{
			ya::Layer& layer = mCurInspectingScene->GetLayer((ya::enums::eLayerType)i);
			const std::vector<ya::GameObject*>& gameObjs
				= layer.GetGameObjects();

			for (ya::GameObject* obj : gameObjs)
			{
				AddGameObject(root, obj);
			}
		}

	}

	void Hierarchy::AddGameObject(TreeWidget::Node* parent, ya::GameObject* gameObject)
	{
		std::string name = StrConverter::ConvertUnicodeToUTF8(gameObject->GetName());
		//std::string name(gameObject->GetName().begin(), gameObject->GetName().end());

		TreeWidget::Node* node = mTreeWidget->AddNode(parent, name, gameObject);
	}

}
