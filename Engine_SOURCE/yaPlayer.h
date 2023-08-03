#pragma once
#include "yaGameObject.h"
#include "yaSekiroEnums.h"
#include "yaState.h"

namespace ya
{
	class Player : public GameObject
	{
	public:
		Player();
		virtual ~Player();

		virtual void Initalize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	public:
		void SetCamera(GameObject* camera) { mCamera = camera; }
		GameObject* GetCamera() { return mCamera; }

		void SetProsthetic(eProsthetics prosthetic) { mProsthetic = prosthetic; }
		eProsthetics GetProsthetic() { return mProsthetic; }

	private:
		GameObject* mCamera;

		State state;
		eProsthetics mProsthetic;
	};
}
