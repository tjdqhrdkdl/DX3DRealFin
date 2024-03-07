#include "yaGameObject.h"
#include "yaTransform.h"

namespace ya
{
	GameObject::GameObject()
		: mState(eState::Active)
		, mType(eLayerType::None)
		, mbDontDestroy(false)
		, mbRender(true)
		, mScene(nullptr)
	{
		mComponents.resize((UINT)eComponentType::End);
		AddComponent(new Transform());
	}

	GameObject::~GameObject()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;
			
			delete comp;
			comp = nullptr;
		}

		for (Component* scrComp : mScripts)
		{
			if (scrComp == nullptr)
				continue;

			delete scrComp;
			scrComp = nullptr;
		}

		for (Component* renderer : mRenderers)
		{
			if (renderer == nullptr)
				continue;

			delete renderer;
			renderer = nullptr;
		}
	}

	void GameObject::Initialize()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->Initialize();
		}

		for (Component* script : mScripts)
		{
			if (script == nullptr)
				continue;

			//script->Initialize();
		}
		for (Component* renderer : mRenderers)
		{
			if (renderer == nullptr)
				continue;

			//renderer->Update();
		}

	}

	void GameObject::Update()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->Update();
		}
		for (Component* renderer : mRenderers)
		{
			if (renderer == nullptr)
				continue;

			renderer->Update();
		}
		for (Component* script : mScripts)
		{
			if (script == nullptr)
				continue;

			script->Update();
		}


	}

	void GameObject::FixedUpdate()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->FixedUpdate();
		}
		for (Component* renderer : mRenderers)
		{
			if (renderer == nullptr)
				continue;

			renderer->FixedUpdate();
		}
		for (Component* script : mScripts)
		{
			if (script == nullptr)
				continue;

			script->FixedUpdate();
		}

	}

	void GameObject::PrevRender()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->PrevRender();
		}
	}

	void GameObject::Render()
	{
		if (mbRender)
		{
			for (Component* comp : mComponents)
			{
				if (comp == nullptr)
					continue;

				comp->Render();
			}
			for (Component* renderer : mRenderers)
			{
				if (renderer == nullptr)
					continue;

				renderer->Render();
			}
			for (Component* script : mScripts)
			{
				if (script == nullptr)
					continue;

				script->Render();
			}



		}
	}

	void GameObject::AddComponent(Component* comp)
	{
		eComponentType order = comp->GetOrder();

		if (order == eComponentType::Script)
		{

			mScripts.push_back(dynamic_cast<Script*>(comp));
			comp->SetOwner(this);
		}

		else if (order == eComponentType::MeshRenderer)
		{
			mRenderers.push_back(dynamic_cast<BaseRenderer*>(comp));
			comp->SetOwner(this);
		}
		else
		{

			mComponents[(UINT)order] = comp;
			mComponents[(UINT)order]->SetOwner(this);
		}
	}
}
