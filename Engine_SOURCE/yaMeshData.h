#pragma once
#include "yaResource.h"
#include "yaMaterial.h"
#include "yaMeshObject.h"
#include "yaMesh.h"


namespace ya
{
	class Scene;
	class AnimationData;
	class MeshData : public Resource
	{
	public:
		MeshData();
		~MeshData();

		static std::shared_ptr<MeshData> LoadFromFbx(const std::wstring& path);
		
		void LoadAnimationFromFbx(const std::wstring& path, const std::wstring& name);

		virtual HRESULT Save(const std::wstring& path, FILE* file = nullptr);
		virtual HRESULT Load(const std::wstring& path, FILE* file = nullptr);

		virtual HRESULT AnimationSave(const std::wstring& path, FILE* file = nullptr);
		virtual HRESULT AnimationLoad(const std::wstring& path, FILE* file = nullptr, bool bLast = true);


		bool IsAnimMesh() { return !mBones.empty(); }

		MeshObject* Instantiate(eLayerType type, Scene* scene = nullptr, const std::wstring& name = L"");


		void Play(const std::wstring animName);
		std::wstring GetPlayAnimationName();

		std::function<void()>& GetAnimationStartEvent(const std::wstring& name);				
		std::function<void()>& GetAnimationCompleteEvent(const std::wstring& name);				
		std::function<void()>& GetAnimationEndEvent(const std::wstring& name);					
		std::function<void()>& GetAnimationFrameEvent(const std::wstring& name, UINT index);	


		graphics::StructedBuffer* GetBoneOffset() { return mBoneOffset; }

		//Save --------- public
		std::vector<BoneMatrix>* GetBones();
		std::vector<BoneAnimationClip>* GetAnimClips();
		UINT GetAnimationClipCount();
		std::vector<graphics::StructedBuffer*> GetBoneFrameData();



		std::vector<std::shared_ptr<Mesh>> GetMeshs() { return mMeshes; }

		std::vector<std::vector<std::shared_ptr<Material>>> GetMaterialsVec() { return mMaterialsVec; }
		void SetBoneOffset(graphics::StructedBuffer* buffer) { mBoneOffset = buffer; }
		class BoneAnimator* GetAnimator() { return mRepresentBoneAnimator; }
		MeshObject* GetMeshObject() { return mMeshObject; }
		std::vector<GameObject*> GetChildObjects() { return mChildObjects; }
		


		void SaveWString(const std::wstring& _str, FILE* _pFile);
		void LoadWString(std::wstring& _str, FILE* _pFile);
		
		
		void SetBoundarySphere(bool b) { mbBoundarySphere = b; }
	private:
		HRESULT LoadFromFbxToThis(const std::fs::path& _fullPath);

	private:
		std::vector<std::shared_ptr<Mesh>>  mMeshes;
		std::vector<std::vector<std::shared_ptr<Material>>> mMaterialsVec;
		std::vector<GameObject*> mChildObjects;

		std::wstring mFullPath;
		graphics::StructedBuffer* mBoneOffset; // 각 뼈의 offset 행렬 () 각뼈의 위치를 TPOSE로 되돌리는 행렬

		//3D Animation 정보
		std::vector<BoneAnimationClip> mAnimClip;
		std::vector<BoneMatrix> mBones;
		std::vector<graphics::StructedBuffer*> mBoneFrameDataVector; // 전체 본 프레임 정보 ( 크기, 이름, 회전) 프레임 갯수만큼
		UINT mAnimationClipCount;
		UINT mIFrameCount;



		Vector3 mMeshCenter;
		float mBoundarySphereRadius;
		class BoneAnimator* mRepresentBoneAnimator;
		MeshObject* mMeshObject;

		//Frustum Culling 여부
		bool mbBoundarySphere;

		std::shared_ptr<AnimationData> mAnimationData;

	};
}
