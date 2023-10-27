#pragma once
#include "guiWidget.h"
#include "yaGameObject.h"
#include "guiTreeWidget.h"

namespace gui
{
	class Hierarchy : public Widget
	{
	public:
		Hierarchy();
		~Hierarchy();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void InitializeInspector(void* data);
		void InitializeScene(ya::enums::eSceneType _sceneType);
		void AddGameObject(TreeWidget::Node* parent, ya::GameObject* gameObject);

	private:
		TreeWidget* mTreeWidget;
		ya::Scene* mCurInspectingScene;
	};
}