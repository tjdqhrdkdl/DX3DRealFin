#pragma once
#include "yaGameObject.h"

namespace ya
{
	class MapObjects :
		public GameObject
	{
	public:
		MapObjects();
		virtual ~MapObjects();


		void LoadMapObject(const std::wstring& _wstrPath, 
			float _positionX, float _positionY, float _positionZ, 
			float _rotationX, float _rotationY, float _rotationZ, 
			float _scaleX = 1.f, float _scaleY = 1.f, float _scaleZ = 1.f);
	};
}


