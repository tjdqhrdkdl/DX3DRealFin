#pragma once
#include "yaComponent.h"
#include "yaScript.h"
#include "yaEntity.h"

namespace ya
{
	class Scene;
	class GameObject : public Entity
	{
		
	public:
		enum eState
		{
			Active,
			Paused,
			Dead,
		};

		GameObject();
		virtual ~GameObject();

		virtual void Initialize();
		void Start();
		virtual void Update();
		void CollisionUpdate();
		virtual void FixedUpdate();
		virtual void PrevRender();
		virtual void Render();
		void FrameEnd();

		template <typename T>
		T* AddComponent()
		{
			T* comp = new T();
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

			comp->Initialize();

			return comp;
		}
		void AddComponent(Component* comp);

		template <typename T>
		T* GetComponent()
		{
			T* comp;
			for (auto c : mComponents)
			{
				comp = dynamic_cast<T*>(c);

				if (comp != nullptr)
					return comp;
			}

			return nullptr;
		}
		template <typename T>
		std::vector<T*> GetComponents()
		{
			std::vector<T*> components = {};
			
			T* comp;
			for (auto c : mComponents)
			{
				comp = dynamic_cast<T*>(c);

				if (comp != nullptr)
					components.push_back(comp);
			}

			return components;
		}

		template <typename T>
		T* GetScript()
		{
			T* script;
			for (auto c : mScripts)
			{
				script = dynamic_cast<T*>(c);

				if (script != nullptr)
					return script;
			}

			return nullptr;
		}

		const std::vector<Script*>& GetScripts() { return mScripts; }

		bool IsDead()
		{
			if (mState == eState::Dead)
				return true;
			
			return false;
		}
		void Pause() { mState = eState::Paused; }
		void Death() { mState = eState::Dead; }
		eState GetState() { return mState; }
		
		bool IsDontDestroy() { return mbDontDestroy; }
		void DontDestroy(bool enable) { mbDontDestroy = enable; }
		eLayerType GetLayerType() { return mType; }
		void SetLayerType(eLayerType type) { mType = type; }

		void SetRender(bool render) { mbRender = render; }

		Scene* GetScene() { return mScene; }
		void SetScene(Scene* scene) { mScene = scene; }

		bool IsStart() const { return mbStart; }

	protected:
		std::vector<Component*> mComponents;

	private:
		eState mState;
		eLayerType mType;
		std::vector<Script*> mScripts;
		bool mbDontDestroy;
		Scene* mScene;

		
		bool mbRender;
		bool mbStart;
	};
}

