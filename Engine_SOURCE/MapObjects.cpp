#include "MapObjects.h"
#include "yaObject.h"
#include "yaResources.h"
#include "yaMeshData.h"
#include "yaTransform.h"
#include "yaCollider2D.h"
#include "CSVEditor.h"
#include "Utils.h"


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
		CSVEditor csv{};	
		csv.ReadFile(csvPath);

		size_t size = csv.GetRowCount();
		//첫줄 description 제외
		for (size_t i = 1; i < size; ++i)
		{
			const std::vector<std::string>& row = csv.GetRow(i);

			std::wstring fileName =	utils::String::ConvertUTF8ToUnicode(row[0]);
			std::wstring objID =	utils::String::ConvertUTF8ToUnicode(row[1]);

			float posX = row[2].empty() ? 0.f : std::stof(row[2]);
			float posY = row[3].empty() ? 0.f : std::stof(row[3]);
			float posZ = row[4].empty() ? 0.f : std::stof(row[4]);

			float rotX = row[5].empty() ? 0.f : std::stof(row[5]);
			float rotY = row[6].empty() ? 0.f : std::stof(row[6]);
			float rotZ = row[7].empty() ? 0.f : std::stof(row[7]);

			float scaleX = row[8].empty() ? 1.f : std::stof(row[8]);
			float scaleY = row[9].empty() ? 1.f : std::stof(row[9]);
			float scaleZ = row[10].empty() ? 1.f : std::stof(row[10]);

			GameObject* created = LoadMapObject(fileName, posX, posY, posZ, rotX, rotY, rotZ, scaleX, scaleY, scaleZ);
			created->SetName(objID);
		}

		//LoadMapObject(L"m11_00_00_00_260000.fbx",
		//	0, 0, 0,		 
		//	0, 20, 0);
		//LoadMapObject(L"m11_00_00_00_002300.fbx",	
		//	186.870, -58.937, 119.039,
		//	0, -96.000, 0);
		//LoadMapObject(L"m11_00_00_00_002301.fbx",	
		//	186.870, -58.924, 119.039,
		//	0, 84.000, 0);
		GameObject::Initialize();
	}

	GameObject* MapObjects::LoadMapObject(const std::wstring& _wstrPath,
		float _positionX, float _positionY, float _positionZ,
		float _rotationX, float _rotationY, float _rotationZ,
		float _scaleX, float _scaleY, float _scaleZ)
	{
		std::fs::path mapMeshPath = gMapPath;
		mapMeshPath /= _wstrPath;
		mapMeshPath.replace_extension(L".fbx");
		std::shared_ptr<MeshData> meshdata = Resources::Load<MeshData>(mapMeshPath.filename(), mapMeshPath);
		assert(nullptr != meshdata);

		GameObject* obj = meshdata->Instantiate(eLayerType::Ground);
		Transform* myTr = GetComponent<Transform>();
		Transform* childTr = obj->GetComponent<Transform>();
		childTr->SetParent(myTr);

		childTr->SetPosition(_positionX, _positionZ, _positionY);
		childTr->SetRotation(-_rotationX, -_rotationZ, -_rotationY);
		childTr->SetScale(_scaleX, _scaleZ, _scaleY);
		return obj;
	}

	void MapObjects::AddCollider(GameObject* obj,
		float _rotationX, float _rotationY, float _rotationZ,
		float _scaleX, float _scaleY, float _scaleZ)
	{
		assert(nullptr != obj);

		Collider2D* col = obj->AddComponent<Collider2D>();
		
		col->SetType(eColliderType::Box);
		col->SetRotation(Vector3(_rotationX, _rotationZ, _rotationY));
		col->SetSize(Vector3(_scaleX, _scaleZ, _scaleY));
	}
}