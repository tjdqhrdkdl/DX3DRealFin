#pragma once
#include "yaGraphics.h"
#include "yaResource.h"
#include "yaFbxLoader.h"
#include "yaStructedBuffer.h"

namespace ya
{
	struct IndexInfo
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		D3D11_BUFFER_DESC desc;
		UINT indexCount;
		void* pIdxSysMem;
	};

	class MeshData;
	class Mesh : public Resource
	{
	public:
		Mesh();
		virtual ~Mesh();

		static std::vector<std::shared_ptr<Mesh>> CreateFromContainer(FbxLoader* loader);
		virtual HRESULT Load(const std::wstring& path) override;

		bool CreateVertexBuffer(void* data, UINT count);
		bool CreateIndexBuffer(void* data, UINT count);
		void BindBuffer(UINT subSet);
		void Render(UINT subSet);
		void RenderInstanced(UINT count, UINT subSet = 0);

		UINT GetSubSetCount() { return (UINT)mIndexInfos.size(); }
		graphics::Vertex* GetVtxSysMem() { return (graphics::Vertex*)pVtxSysMem; }

		void SetParentMeshData(MeshData* meshData) { mParentMeshData = meshData; }
		MeshData* GetParentMeshData() { return mParentMeshData; }

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		D3D11_BUFFER_DESC mVBDesc;
		UINT mVtxCount;
		void* pVtxSysMem;

		std::vector<IndexInfo> mIndexInfos;

		MeshData* mParentMeshData;
	};
}
