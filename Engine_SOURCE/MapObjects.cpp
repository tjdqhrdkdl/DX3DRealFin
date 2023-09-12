#include "MapObjects.h"
#include "yaObject.h"
#include "yaResources.h"
#include "yaMeshData.h"
#include "yaTransform.h"
#include "CSVEditor.h"


namespace ya
{
	MapObjects::MapObjects()
	{
	}

	MapObjects::~MapObjects()
	{


	}

	void MapObjects::Initialize()
	{
		SetName(L"map");

		std::fs::path csvPath(gResPath);
		csvPath /= L"Map";
		csvPath /= L"SekiroMapMeshData.csv";
		std::fs::path abs = std::fs::absolute(csvPath);
		CSVEditor csv{};
		csv.ReadFile(csvPath);

		LoadMapObject(L"Map\\Mesh\\m11_00_00_00_260000.fbx",
			90, 3, -40,		 
			0, 20, 0);
		LoadMapObject(L"Map\\Mesh\\m11_00_00_00_002300.fbx",	
			186.870, -58.937, 119.039,
			0, -96.000, 0);
		LoadMapObject(L"Map\\Mesh\\m11_00_00_00_002301.fbx",	
			186.870, -58.924, 119.039,
			0, 84.000, 0);
		GameObject::Initialize();
	}

	void MapObjects::LoadMapObject(const std::wstring& _wstrPath,
		float _positionX, float _positionY, float _positionZ,
		float _rotationX, float _rotationY, float _rotationZ,
		float _scaleX, float _scaleY, float _scaleZ)
	{
		std::shared_ptr<MeshData> meshdata = MeshData::LoadFromFbx(_wstrPath);
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