#pragma once
#include "yaScene.h"

namespace ya
{
	class MeshData;
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


		void Test() { int a = 0; }
	private:
		//for test
		std::shared_ptr<MeshData> mMeshData;

	};
}
