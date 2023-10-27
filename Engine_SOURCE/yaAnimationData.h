#pragma once
#include "yaResource.h"
#include "yaMeshData.h"


namespace ya
{
	class Scene;
	class AnimationData : public Resource
	{
	public:

		AnimationData();
		virtual ~AnimationData();
		virtual HRESULT Save(const std::wstring& path, FILE* file = nullptr) override;
		virtual HRESULT Load(const std::wstring& path, FILE* file = nullptr) override;

		virtual HRESULT LoadNotFin(const std::wstring& path, FILE* file = nullptr) ;
		virtual std::shared_ptr<AnimationData> LoadFin(const std::wstring& path, FILE* file = nullptr) ;

		void LoadWString(std::wstring& _str, FILE* _pFile);
		void PushBackBoneFrameData(graphics::StructedBuffer* buffer) { mBoneFrameDataVector.push_back(buffer); }

		std::vector<graphics::StructedBuffer*> GetBoneFrameDataVector() { return mBoneFrameDataVector; }
		UINT GetAnimationClipCount() { return mAnimationClipCount; }
		std::vector<BoneMatrix>* GetBones() { return &mBones; }
		std::vector<BoneAnimationClip>* GetAnimClips() { return &mAnimClip; }

	private:
		std::vector<BoneAnimationClip> mAnimClip;
		std::vector<BoneMatrix> mBones;
		std::vector<graphics::StructedBuffer*> mBoneFrameDataVector; // 전체 본 프레임 정보 ( 크기, 이름, 회전) 프레임 갯수만큼
		UINT mAnimationClipCount;
		UINT mIFrameCount;

	};
}

