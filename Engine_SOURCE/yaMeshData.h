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
		
		void LoadAnimationFromFbx(const std::wstring& path, const std::wstring& name);

		virtual HRESULT Save(const std::wstring& path, FILE* file = nullptr);
		virtual HRESULT Load(const std::wstring& path, FILE* file = nullptr);

		virtual HRESULT AnimationSave(const std::wstring& path, const std::wstring& animationname, FILE* file = nullptr);
		virtual HRESULT AnimationLoad(const std::wstring& path, const std::wstring& animationname, FILE* file = nullptr);


		bool IsAnimMesh() { return !mBones.empty(); }

		MeshObject* Instantiate(eLayerType type);


		void Play(const std::wstring animName);

		std::function<void()>& GetAnimationStartEvent(const std::wstring& name);				
		std::function<void()>& GetAnimationCompleteEvent(const std::wstring& name);				
		std::function<void()>& GetAnimationEndEvent(const std::wstring& name);					
		std::function<void()>& GetAnimationFrameEvent(const std::wstring& name, UINT index);	


		std::vector<graphics::StructedBuffer*> GetBoneFrameData() { return mBoneFrameDataVector; }
		graphics::StructedBuffer* GetBoneOffset() { return mBoneOffset; }

		//Save --------- public
		std::vector<BoneMatrix>* GetBones() { return &mBones; }
		std::vector<BoneAnimationClip> GetAnimClips() { return mAnimClip; }

		std::vector<std::shared_ptr<Mesh>> GetMeshs() { return mMeshes; }

		std::vector<std::vector<std::shared_ptr<Material>>> GetMaterialsVec() { return mMaterialsVec; }

		void PushBackAnimClip(BoneAnimationClip& clip) { mAnimClip.push_back(clip); }

		void PushBackBoneFrameData(graphics::StructedBuffer* buffer) { mBoneFrameDataVector.push_back(buffer); }

		void SetBoneOffset(graphics::StructedBuffer* buffer) { mBoneOffset = buffer; }

		void SetAnimationClipCount(UINT num) { mAnimationClipCount = num; }
		UINT GetAnimationClipCount() { return mAnimationClipCount; }


		void SaveWString(const std::wstring& _str, FILE* _pFile);
		void LoadWString(std::wstring& _str, FILE* _pFile);


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

		class BoneAnimator* mRepresentBoneAnimator;
	};
}
