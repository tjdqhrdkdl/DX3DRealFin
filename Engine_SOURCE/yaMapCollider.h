#pragma once
#include "yaGameObject.h"

namespace ya
{
	class MapCollider : public GameObject
	{
	public:

		MapCollider();
		virtual ~MapCollider();

		virtual void Initialize() override;

		void AddWallCollider(Vector3 transform, Vector3 rotation = Vector3::Zero, Vector3 size = Vector3::One);
		void AddGroundCollider(Vector3 transform, Vector3 rotation = Vector3::Zero, Vector3 size = Vector3::One);

	private:
		UINT mWallCount;
		UINT mGroundCount;

		std::wstring mWalkerName;
	};
}
