#include "yaAnimationData.h"
#include "yaGameObject.h"
#include "yaFbxLoader.h"
#include "yaResources.h"
#include "yaObject.h"
#include "yaMeshRenderer.h"
#include "yaMeshObject.h"
#include "yaBoneAnimator.h"
#include "Utils.h"
#include "CommonInclude.h"
#include "yaBoundarySphere.h"
#include "yaScene.h"
#include "StrConverter.h"

using namespace std;
namespace ya
{
	AnimationData::AnimationData()
		: Resource(eResourceType::AnimationData)
		, mAnimationClipCount(0)
		, mIFrameCount(0)
	{

	}

	AnimationData::~AnimationData()
	{
		for (size_t i = 0; i < mBoneFrameDataVector.size(); i++)
		{
			delete mBoneFrameDataVector[i];
		}

	}
	HRESULT AnimationData::Save(const std::wstring& path, FILE* file)
	{
		return S_OK;
	}
	HRESULT AnimationData::Load(const std::wstring& path, FILE* file)
	{

		return S_OK;
	}
	HRESULT AnimationData::LoadNotFin(const std::wstring& path, FILE* file)
	{
		std::string strPath = StrConverter::ConvertUnicodeToUTF8(path);

		std::filesystem::path CurparentPath = std::fs::absolute(gResPath);

		std::filesystem::path parentPath = strPath;
		parentPath = parentPath.parent_path().parent_path();
		parentPath += L"\\AnimationData\\";

		CurparentPath /= parentPath;

		std::wstring name = std::filesystem::path(path).stem();
		name += L".animationdata";

		CurparentPath += name;

		std::wstring fullPath = CurparentPath;

		file = nullptr;
		_wfopen_s(&file, fullPath.c_str(), L"rb");
		if (file == nullptr)
			return S_FALSE;

		//본 사이즈 
		UINT boneSize = 0;
		fread(&boneSize, sizeof(UINT), 1, file);


		//애니 클립 사이즈 저장
		UINT AnimClipSize = 0;
		fread(&AnimClipSize, sizeof(UINT), 1, file);
		mAnimationClipCount += AnimClipSize;

		std::vector<BoneAnimationClip> animClip;

		animClip.resize(AnimClipSize);
		for (size_t i = 0; i < AnimClipSize; i++)
		{
			LoadWString(animClip[i].name, file);

			fread(&animClip[i].startTime, sizeof(double), 1, file);
			fread(&animClip[i].endTime, sizeof(double), 1, file);
			fread(&animClip[i].timeLength, sizeof(double), 1, file);
			fread(&animClip[i].mode, sizeof(int), 1, file);
			fread(&animClip[i].updateTime, sizeof(float), 1, file);
			fread(&animClip[i].startFrame, sizeof(int), 1, file);
			fread(&animClip[i].endFrame, sizeof(int), 1, file);
			fread(&animClip[i].frameLength, sizeof(int), 1, file);
		}

		// 본정보들 전부 저장	
		//UINT iFrameCount = 0;
		mBones.resize(boneSize);
		for (size_t i = 0; i < boneSize; i++)
		{
			LoadWString(mBones[i].name, file);
			fread(&mBones[i].depth, sizeof(int), 1, file);
			fread(&mBones[i].parentIdx, sizeof(int), 1, file);
			fread(&mBones[i].bone, sizeof(Matrix), 1, file);
			fread(&mBones[i].offset, sizeof(Matrix), 1, file);

			std::vector<std::vector<BoneKeyFrame>> keyFrames;
			keyFrames.resize(AnimClipSize);
			//mBones[i].keyFrames.resize(AnimClipSize);
			for (size_t j = 0; j < AnimClipSize; j++)
			{
				UINT boneKeyFramesSize;
				fread(&boneKeyFramesSize, sizeof(UINT), 1, file);

				keyFrames[j].resize(boneKeyFramesSize);

				for (UINT k = 0; k < keyFrames[j].size(); k++)
				{
					fread(&keyFrames[j][k].time, sizeof(double), 1, file);
					fread(&keyFrames[j][k].frame, sizeof(int), 1, file);
					fread(&keyFrames[j][k].translate, sizeof(Vector3), 1, file);
					fread(&keyFrames[j][k].scale, sizeof(Vector3), 1, file);
					fread(&keyFrames[j][k].rotation, sizeof(Vector4), 1, file);
				}
				mIFrameCount = max(mIFrameCount, (UINT)keyFrames[j].size());

				/*
				mBones[i].keyFrames[j].resize(boneKeyFramesSize);
				for (UINT k = 0; k < mBones[i].keyFrames[j].size(); k++)
				{
					fread(&mBones[i].keyFrames[j][k].time, sizeof(double), 1, file);
					fread(&mBones[i].keyFrames[j][k].frame, sizeof(int), 1, file);
					fread(&mBones[i].keyFrames[j][k].translate, sizeof(Vector3), 1, file);
					fread(&mBones[i].keyFrames[j][k].scale, sizeof(Vector3), 1, file);
					fread(&mBones[i].keyFrames[j][k].rotation, sizeof(Vector4), 1, file);
				}
				iFrameCount = max(iFrameCount, (UINT)mBones[i].keyFrames[j].size());
				*/
			}
			mBones[i].keyFrames.insert(mBones[i].keyFrames.end(), keyFrames.begin(), keyFrames.end());
		}

		fclose(file);

		mAnimClip.insert(mAnimClip.end(), animClip.begin(), animClip.end());

		return S_OK;
	}
	std::shared_ptr<AnimationData> AnimationData::LoadFin(const std::wstring& path, FILE* file)
	{
		std::string strPath = StrConverter::ConvertUnicodeToUTF8(path);

		std::filesystem::path CurparentPath = std::fs::absolute(gResPath);

		std::filesystem::path parentPath = strPath;
		parentPath = parentPath.parent_path().parent_path();
		parentPath += L"\\AnimationData\\";

		CurparentPath /= parentPath;

		std::wstring name = std::filesystem::path(path).stem();
		name += L".animationdata";

		CurparentPath += name;

		std::wstring fullPath = CurparentPath;
		std::wstring key = fullPath;

		file = nullptr;
		_wfopen_s(&file, fullPath.c_str(), L"rb");
		if (file == nullptr)
			assert(NULL);

		//본 사이즈 
		UINT boneSize = 0;
		fread(&boneSize, sizeof(UINT), 1, file);


		//애니 클립 사이즈 저장
		UINT AnimClipSize = 0;
		fread(&AnimClipSize, sizeof(UINT), 1, file);
		mAnimationClipCount += AnimClipSize;

		std::vector<BoneAnimationClip> animClip;

		animClip.resize(AnimClipSize);
		for (size_t i = 0; i < AnimClipSize; i++)
		{
			LoadWString(animClip[i].name, file);

			fread(&animClip[i].startTime, sizeof(double), 1, file);
			fread(&animClip[i].endTime, sizeof(double), 1, file);
			fread(&animClip[i].timeLength, sizeof(double), 1, file);
			fread(&animClip[i].mode, sizeof(int), 1, file);
			fread(&animClip[i].updateTime, sizeof(float), 1, file);
			fread(&animClip[i].startFrame, sizeof(int), 1, file);
			fread(&animClip[i].endFrame, sizeof(int), 1, file);
			fread(&animClip[i].frameLength, sizeof(int), 1, file);
		}

		// 본정보들 전부 저장	
		//UINT iFrameCount = 0;
		mBones.resize(boneSize);
		for (size_t i = 0; i < boneSize; i++)
		{
			LoadWString(mBones[i].name, file);
			fread(&mBones[i].depth, sizeof(int), 1, file);
			fread(&mBones[i].parentIdx, sizeof(int), 1, file);
			fread(&mBones[i].bone, sizeof(Matrix), 1, file);
			fread(&mBones[i].offset, sizeof(Matrix), 1, file);

			std::vector<std::vector<BoneKeyFrame>> keyFrames;
			keyFrames.resize(AnimClipSize);
			//mBones[i].keyFrames.resize(AnimClipSize);
			for (size_t j = 0; j < AnimClipSize; j++)
			{
				UINT boneKeyFramesSize;
				fread(&boneKeyFramesSize, sizeof(UINT), 1, file);

				keyFrames[j].resize(boneKeyFramesSize);

				for (UINT k = 0; k < keyFrames[j].size(); k++)
				{
					fread(&keyFrames[j][k].time, sizeof(double), 1, file);
					fread(&keyFrames[j][k].frame, sizeof(int), 1, file);
					fread(&keyFrames[j][k].translate, sizeof(Vector3), 1, file);
					fread(&keyFrames[j][k].scale, sizeof(Vector3), 1, file);
					fread(&keyFrames[j][k].rotation, sizeof(Vector4), 1, file);
				}
				mIFrameCount = max(mIFrameCount, (UINT)keyFrames[j].size());

			}
			mBones[i].keyFrames.insert(mBones[i].keyFrames.end(), keyFrames.begin(), keyFrames.end());
		}

		fclose(file);

		mAnimClip.insert(mAnimClip.end(), animClip.begin(), animClip.end());

		{
			std::vector<std::vector<BoneFrameTransform>> vecFrameTrans;
			vecFrameTrans.resize(mAnimationClipCount);

			for (size_t i = 0; i < mBones.size(); ++i)
			{
				for (size_t k = 0; k < mAnimationClipCount; k++)
				{
					vecFrameTrans[k].resize((UINT)mBones.size() * mIFrameCount);
					for (size_t j = 0; j < mBones[i].keyFrames[k].size(); ++j)
					{
						vecFrameTrans[k][(UINT)mBones.size() * j + i]
							= BoneFrameTransform
						{
							Vector4(mBones[i].keyFrames[k][j].translate.x 
							, mBones[i].keyFrames[k][j].translate.y  
								, mBones[i].keyFrames[k][j].translate.z  , 0.f)
							, Vector4(mBones[i].keyFrames[k][j].scale.x
								, mBones[i].keyFrames[k][j].scale.y
								, mBones[i].keyFrames[k][j].scale.z, 0.f)
							, mBones[i].keyFrames[k][j].rotation
						};
					}
				}
			}

			for (size_t i = 0; i < mAnimationClipCount; i++)
			{
				graphics::StructedBuffer* boneFrameData = new graphics::StructedBuffer();
				boneFrameData->Create(sizeof(BoneFrameTransform), (UINT)mBones.size() * mIFrameCount
					, eSRVType::SRV, vecFrameTrans[i].data(), false);
				PushBackBoneFrameData(boneFrameData);
			}
		}

		return nullptr;

	}
	void AnimationData::LoadWString(std::wstring& _str, FILE* _pFile)
	{
		size_t iLen = 0;
		fread(&iLen, sizeof(size_t), 1, _pFile);

		wchar_t szBuff[256] = {};
		fread(szBuff, sizeof(wchar_t), iLen, _pFile);
		_str = szBuff;
	}
}