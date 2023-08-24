#include "yaAnimationData.h"

namespace ya
{
	AnimationData::AnimationData() 
		: Resource(eResourceType::Animation)
	{
	}
	AnimationData::~AnimationData()
	{
	}

	AnimationData* AnimationData::LoadFromFbx(MeshData* parentMeshData, const std::wstring& path, const std::wstring& name)
	{
		std::filesystem::path parentPath = std::filesystem::current_path().parent_path();
		std::wstring fullPath = parentPath.wstring() + L"\\Resources\\" + path;

		AnimationData* animData = new AnimationData();

		FbxLoader loader;
		loader.Initialize();
		loader.LoadAnimationFbx(fullPath);
		
		
		std::vector<Bone*>& vecBone = loader.GetBones();
		UINT iFrameCount = 0;
		std::vector<BoneMatrix>* meshBones = parentMeshData->GetBones();
		UINT animCount = parentMeshData->GetAnimationClipCount();
		parentMeshData->SetAnimationClipCount(animCount + 1);
		for (size_t j = 0; j < meshBones->size(); j++)
		{
			meshBones->at(j).keyFrames.push_back(std::vector<BoneKeyFrame>());
			for (UINT i = 0; i < vecBone.size(); ++i)
			{
				if (meshBones->at(j).name == vecBone[i]->name)
				{
					for (UINT k = 0; k < vecBone[i]->keyFrames.size(); ++k)
					{
						BoneKeyFrame tKeyframe = {};
						tKeyframe.time = vecBone[i]->keyFrames[k].time;
						tKeyframe.frame = j;
						tKeyframe.translate.x = (float)vecBone[i]->keyFrames[k].transform.GetT().mData[0];
						tKeyframe.translate.y = (float)vecBone[i]->keyFrames[k].transform.GetT().mData[1];
						tKeyframe.translate.z = (float)vecBone[i]->keyFrames[k].transform.GetT().mData[2];

						tKeyframe.scale.x = (float)vecBone[i]->keyFrames[k].transform.GetS().mData[0];
						tKeyframe.scale.y = (float)vecBone[i]->keyFrames[k].transform.GetS().mData[1];
						tKeyframe.scale.z = (float)vecBone[i]->keyFrames[k].transform.GetS().mData[2];

						tKeyframe.rotation.x = (float)vecBone[i]->keyFrames[k].transform.GetQ().mData[0];
						tKeyframe.rotation.y = (float)vecBone[i]->keyFrames[k].transform.GetQ().mData[1];
						tKeyframe.rotation.z = (float)vecBone[i]->keyFrames[k].transform.GetQ().mData[2];
						tKeyframe.rotation.w = (float)vecBone[i]->keyFrames[k].transform.GetQ().mData[3];

						meshBones->at(j).keyFrames[animCount].push_back(tKeyframe);


					}
					break;
				}
			}
			iFrameCount = max(iFrameCount, (UINT)meshBones->at(j).keyFrames[animCount].size());

		}

		std::vector<AnimationClip*>& vecAnimClip = loader.GetAnimClip();

		for (UINT i = 0; i < vecAnimClip.size(); ++i)
		{
			BoneAnimationClip tClip = {};

			tClip.name = name;
			tClip.startTime = vecAnimClip[i]->startTime.GetSecondDouble();
			tClip.endTime = vecAnimClip[i]->endTime.GetSecondDouble();
			tClip.timeLength = tClip.endTime - tClip.startTime;

			tClip.startFrame = (int)vecAnimClip[i]->startTime.GetFrameCount(vecAnimClip[i]->mode);
			tClip.endFrame = (int)vecAnimClip[i]->endTime.GetFrameCount(vecAnimClip[i]->mode);
			tClip.frameLength = tClip.endFrame - tClip.startFrame;
			tClip.mode = vecAnimClip[i]->mode;

			parentMeshData->PushBackAnimClip(tClip);
		}

		// Animation 이 있는 Mesh 경우 structuredbuffer 만들어두기
		std::vector<BoneFrameTransform> vecFrameTrans;
		vecFrameTrans.resize((UINT)meshBones->size() * iFrameCount);

		for (size_t i = 0; i < meshBones->size(); ++i)
		{

			for (size_t j = 0; j < meshBones->at(i).keyFrames[animCount].size(); ++j)
			{
				vecFrameTrans[(UINT)meshBones->size() * j + i]
					= BoneFrameTransform
				{
					Vector4(meshBones->at(i).keyFrames[animCount][j].translate.x
						, meshBones->at(i).keyFrames[animCount][j].translate.y
						, meshBones->at(i).keyFrames[animCount][j].translate.z, 0.f)
					, Vector4(meshBones->at(i).keyFrames[animCount][j].scale.x
						, meshBones->at(i).keyFrames[animCount][j].scale.y
						, meshBones->at(i).keyFrames[animCount][j].scale.z, 0.f)
					, meshBones->at(i).keyFrames[animCount][j].rotation
				};
			}
		}

		graphics::StructedBuffer* boneFrameData = new StructedBuffer();
		boneFrameData->Create(sizeof(BoneFrameTransform), (UINT)meshBones->size() * iFrameCount
			, eSRVType::SRV, vecFrameTrans.data(), false);
		parentMeshData->PushBackBoneFrameData(boneFrameData);

		loader.Release();
		return animData;
	}


}