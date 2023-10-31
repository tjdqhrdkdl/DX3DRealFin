#pragma once
#include "yaScene.h"

namespace ya
{
	class SceneManager
	{
	public:
		static void Initialize();
		static void Update();
		static void CollisionUpdate();
		static void FixedUpdate();
		static void Render();
		static void Destroy();
		static void Release();

		static void LoadScene(eSceneType type);
		static Scene* GetActiveScene() { return mActiveScene; }
		static Scene* GetScene(eSceneType type) { return mScenes[(UINT)type]; }

	private:
		static std::vector<Scene*> mScenes;
		static Scene* mActiveScene;
	};
}
