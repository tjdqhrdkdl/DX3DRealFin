#include "yaMeshData.h"
#include "yaGameObject.h"
#include "yaFbxLoader.h"
#include "yaResources.h"
#include "yaObject.h"
#include "yaMeshRenderer.h"

namespace ya
{
	MeshData::MeshData()
		: Resource(eResourceType::MeshData)
	{
	}
	MeshData::~MeshData()
	{
	}
	MeshData* MeshData::LoadFromFbx(const std::wstring& path)
	{
		std::filesystem::path parentPath = std::filesystem::current_path().parent_path();
		std::wstring fullPath = parentPath.wstring() + L"\\Resources\\" + path;
		

		FbxLoader loader;
		loader.Initialize();
		loader.LoadFbx(fullPath);

		// 메시들 가져오기
		std::vector<std::shared_ptr<Mesh>> meshes = Mesh::CreateFromContainer(&loader);
		std::vector<std::vector<std::shared_ptr<Material>>>  materialsVec = {};

		std::shared_ptr<Mesh> mesh = nullptr;
		for (size_t i = 0; i < meshes.size(); i++)
		{
			mesh = meshes[i];

			// 리소스에 넣어주기

			std::wstring name = std::filesystem::path(fullPath).stem();
			name += L".mesh" + std::to_wstring(i);
			Resources::Insert(name, mesh);

			// 메테리얼 가져오기

			std::vector<std::shared_ptr<Material>> materials = {};
			for (size_t k = 0; k < loader.GetContainer(i).materials.size(); k++)
				{
					std::shared_ptr<Material> material
						= Resources::Find<Material>(loader.GetContainer(i).materials[k].name);

					materials.push_back(material);
				}
			materialsVec.push_back(materials);
		}
		
		 



		MeshData* meshData = new MeshData();
		meshData->mMeshes = meshes;
		meshData->mMaterialsVec = materialsVec;
		meshData->mFullPath = fullPath;
		return meshData;
	}

	void MeshData::Save(const std::wstring& path)
	{
	}
	std::vector<GameObject*> MeshData::Instantiate()
	{

		std::vector<GameObject*> ret = {};
		for (size_t i = 0; i < mMeshes.size(); i++)
		{
			GameObject* gameObj = object::Instantiate<GameObject>(eLayerType::Player);
			gameObj->SetName(mFullPath + std::to_wstring(i));
			MeshRenderer* mr = gameObj->AddComponent<MeshRenderer>();
			mr->SetMesh(mMeshes[i]);

			for (size_t k = 0; k < mMaterialsVec[i].size(); k++)
			{
				mr->SetMaterial(mMaterialsVec[i][k], k);
			}
			ret.push_back(gameObj);
		}


		return ret;
	}
}
