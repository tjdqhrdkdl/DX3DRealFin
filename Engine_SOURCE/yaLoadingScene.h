#pragma once
#include "yaScene.h"

namespace ya
{
	class LoadingScene : public Scene
	{
	public:
		LoadingScene();
		virtual ~LoadingScene();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;

	public:
		void SetLoading(float loadingTime = 1.0f) { mLoadingTime = loadingTime; }

	private:
		float mLoadingTime;
		GameObject* mLoader;
	};
}