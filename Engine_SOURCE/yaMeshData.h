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
		bool IsAnimMesh() { return !mBones.empty(); }

		MeshObject* Instantiate(eLayerType type);

		std::vector<BoneMatrix>* GetBones() { return &mBones; }
		std::vector<BoneAnimationClip> GetAnimClips() { return mAnimClip; }
		void PushBackAnimClip(BoneAnimationClip& clip) { mAnimClip.push_back(clip); }

		void PushBackBoneFrameData(graphics::StructedBuffer* buffer) { mBoneFrameDataVector.push_back(buffer); }
		std::vector<graphics::StructedBuffer*> GetBoneFrameData() { return mBoneFrameDataVector; }

		void SetBoneOffset(graphics::StructedBuffer* buffer) { mBoneOffset = buffer; }
		graphics::StructedBuffer* GetBoneOffset() { return mBoneOffset; }

		void SetAnimationClipCount(UINT num) { mAnimationClipCount = num; }
		UINT GetAnimationClipCount() { return mAnimationClipCount; }

		void Play(const std::wstring animName);
	private:
		std::vector<std::shared_ptr<Mesh>>  mMeshes;
		std::vector<std::vector<std::shared_ptr<Material>>> mMaterialsVec;
		std::vector<GameObject*> mChildObjects;


		std::wstring mFullPath;

		//3D Animation 정보
		std::vector<BoneAnimationClip> mAnimClip;
		std::vector<BoneMatrix> mBones;

		std::vector<graphics::StructedBuffer*> mBoneFrameDataVector; // 전체 본 프레임 정보 ( 크기, 이름, 회전) 프레임 갯수만큼
		graphics::StructedBuffer* mBoneOffset; // 각 뼈의 offset 행렬 () 각뼈의 위치를 TPOSE로 되돌리는 행렬

		UINT mAnimationClipCount;
	};
}
