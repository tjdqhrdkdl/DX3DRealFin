#pragma once
#include "yaScene.h"

namespace ya
{
	class MonsterBase;
	class PlayScene : public Scene
	{
	public:
		PlayScene();
		virtual ~PlayScene();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void OnEnter() override;
		virtual void OnExit() override;

	public:
		void Reset();

	private:
		std::vector<MonsterBase*> mMonsters;

	};
}
