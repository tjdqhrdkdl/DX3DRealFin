#include "MapObjects.h"
#include "yaObject.h"
#include "yaResources.h"
#include "yaMeshData.h"
#include "yaTransform.h"

namespace ya
{
	MapObjects::MapObjects()
	{
	}

	MapObjects::~MapObjects()
	{


	}

	void MapObjects::LoadMapObject(const std::wstring& _wstrPath,
		float _positionX, float _positionY, float _positionZ,
		float _rotationX, float _rotationY, float _rotationZ,
		float _scaleX = 0.f, float _scaleY = 0.f, float _scaleZ = 0.f)
	{
		std::shared_ptr<MeshData> meshdata = Resources::Load<MeshData>(_wstrPath, _wstrPath);
		assert(nullptr != meshdata);

		GameObject* obj = meshdata->Instantiate(eLayerType::Ground);
		Transform* myTr = GetComponent<Transform>();
		Transform* childTr = obj->GetComponent<Transform>();
		childTr->SetParent(myTr);

		childTr->SetPosition(_positionX, _positionZ, _positionY);
		childTr->SetRotation(-_rotationX, -_rotationZ, -_rotationY);
		childTr->SetScale(_scaleX, _scaleZ, _scaleY);
	}
}