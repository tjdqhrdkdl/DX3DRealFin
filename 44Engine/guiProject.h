#pragma once
#include "guiWidget.h"
#include "guiTreeWidget.h"
#include "yaResources.h"
#include "yaResource.h"
#include "StrConverter.h"

namespace gui
{
	class Project : public Widget
	{
	public:
		Project();
		~Project();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void ResetContent();

	private:
		template <typename T>
		void AddResources(TreeWidget::Node* rootNode, const char* name)
		{
			const std::vector<std::shared_ptr<T>> resources
				= ya::Resources::Finds<T>();

			TreeWidget::Node* stemNode
				= mTreeWidget->AddNode(rootNode, name, 0, true);

			for (std::shared_ptr<T> resource : resources)
			{
				std::string name = StrConverter::ConvertUnicodeToUTF8(resource->GetName());
				//std::string name(resource->GetName().begin(), resource->GetName().end());
				mTreeWidget->AddNode(stemNode, name, resource.get());
			}
		}

		void toInspector(void* data);



	private:
		TreeWidget* mTreeWidget;
	};
}