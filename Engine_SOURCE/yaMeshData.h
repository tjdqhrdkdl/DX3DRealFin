#pragma once
#include "yaResource.h"
#include "yaMaterial.h"
#include "yaMeshObject.h"
#include "yaMesh.h"


namespace ya
{
	class MeshData : public Resource
	{
	public:
		MeshData();
		~MeshData();

		static MeshData* LoadFromFbx(const std::wstring& path);

		virtual void Save(const std::wstring& path);
		virtual HRESULT Load(const std::wstring& path) { return S_OK; }

		MeshObject* Instantiate(eLayerType type);


	private:
		std::vector<std::shared_ptr<Mesh>>  mMeshes;
		std::vector<std::vector<std::shared_ptr<Material>>> mMaterialsVec;

		//개발중 변수
		std::wstring mFullPath;
	};
}
