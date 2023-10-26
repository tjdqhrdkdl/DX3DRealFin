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
		, mbStart(false)
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

	}

	void GameObject::Start()
	{
		mbStart = true;
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;


			comp->Start();
		}
	}

	void GameObject::Update()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr || false == comp->IsActive())
				continue;

			comp->Update();
		}

		for (Component* script : mScripts)
		{
			if (script == nullptr || false == script->IsActive())
				continue;

			script->Update();
		}
	}

	void GameObject::FixedUpdate()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr || false == comp->IsActive())
				continue;

			comp->FixedUpdate();
		}

		for (Component* script : mScripts)
		{
			if (script == nullptr || false == script->IsActive())
				continue;

			script->FixedUpdate();
		}
	}

	void GameObject::PrevRender()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr || false == comp->IsActive())
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
				if (comp == nullptr || false == comp->IsActive())
					continue;

				comp->Render();
			}

			for (Component* script : mScripts )
			{
				if (script == nullptr || false == script->IsActive())
					continue;

				script->Render();
			}
		}
	}

	void GameObject::FrameEnd()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr || false == comp->IsActive())
				continue;

			comp->FrameEnd();
		}
	}

	void GameObject::AddComponent(Component* comp)
	{
		eComponentType order = comp->GetOrder();

		if (order != eComponentType::Script)
		{
			mComponents[(UINT)order] = comp;
			mComponents[(UINT)order]->SetOwner(this);
		}
		else
		{
			mScripts.push_back(dynamic_cast<Script*>(comp));
			comp->SetOwner(this);
		}
	}
}
