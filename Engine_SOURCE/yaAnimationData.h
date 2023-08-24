#pragma once
#include "yaResource.h"
#include "yaFbxLoader.h"
#include "yaMeshData.h"
#include "yaStructedBuffer.h"



namespace ya
{
	class AnimationData : public Resource
	{
	public:
		AnimationData();
		virtual ~AnimationData();

		static AnimationData* LoadFromFbx(MeshData* animOwner, const std::wstring& path, const std::wstring& name);

		//virtual void Save(const std::wstring& path);
		virtual HRESULT Load(const std::wstring& path) { return S_OK; }
	

	private:
		MeshData mMeshData;

		//3D Animation 정보
		std::vector<BoneAnimationClip> mAnimClip;

		graphics::StructedBuffer* mBoneFrameData; // 전체 본 프레임 정보 ( 크기, 이름, 회전) 프레임 갯수만큼
	};
}
