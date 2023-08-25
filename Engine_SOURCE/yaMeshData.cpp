#include "yaMeshData.h"
#include "yaGameObject.h"
#include "yaFbxLoader.h"
#include "yaResources.h"
#include "yaObject.h"
#include "yaMeshRenderer.h"
#include "yaMeshObject.h"
#include "yaBoneAnimator.h"


namespace ya
{
	MeshData::MeshData()
		: Resource(eResourceType::MeshData)
		, mAnimationClipCount(0)
	{
	}
	MeshData::~MeshData()
	{
	}
	MeshData* MeshData::LoadFromFbx(const std::wstring& path)
	{
		std::filesystem::path parentPath = std::filesystem::current_path().parent_path();
		std::wstring fullPath = parentPath.wstring() + L"\\Resources\\" + path;
		
		MeshData* meshData = new MeshData();

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
			mesh->SetParentMeshData(meshData);
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
		

		//3d Animation 로직
		std::vector<BoneAnimationClip> animClip;
		std::vector<BoneMatrix> bones;

		std::vector<Bone*>& vecBone = loader.GetBones();
		UINT iFrameCount = 0;
		for (UINT i = 0; i < vecBone.size(); ++i)
		{
			BoneMatrix bone = {};
			bone.depth = vecBone[i]->depth;
			bone.parentIdx = vecBone[i]->parentIdx;
			bone.bone = GetMatrixFromFbxMatrix(vecBone[i]->boneMatrix);
			bone.offset = GetMatrixFromFbxMatrix(vecBone[i]->offsetMatrix);
			bone.name = vecBone[i]->name;

			//for (UINT j = 0; j < vecBone[i]->keyFrames.size(); ++j)
			//{
			//	BoneKeyFrame tKeyframe = {};
			//	tKeyframe.time = vecBone[i]->keyFrames[j].time;
			//	tKeyframe.frame = j;
			//	tKeyframe.translate.x = (float)vecBone[i]->keyFrames[j].transform.GetT().mData[0];
			//	tKeyframe.translate.y = (float)vecBone[i]->keyFrames[j].transform.GetT().mData[1];
			//	tKeyframe.translate.z = (float)vecBone[i]->keyFrames[j].transform.GetT().mData[2];

			//	tKeyframe.scale.x = (float)vecBone[i]->keyFrames[j].transform.GetS().mData[0];
			//	tKeyframe.scale.y = (float)vecBone[i]->keyFrames[j].transform.GetS().mData[1];
			//	tKeyframe.scale.z = (float)vecBone[i]->keyFrames[j].transform.GetS().mData[2];

			//	tKeyframe.rotation.x = (float)vecBone[i]->keyFrames[j].transform.GetQ().mData[0];
			//	tKeyframe.rotation.y = (float)vecBone[i]->keyFrames[j].transform.GetQ().mData[1];
			//	tKeyframe.rotation.z = (float)vecBone[i]->keyFrames[j].transform.GetQ().mData[2];
			//	tKeyframe.rotation.w = (float)vecBone[i]->keyFrames[j].transform.GetQ().mData[3];

			//	bone.keyFrames.push_back(tKeyframe);
			//}

			//iFrameCount = max(iFrameCount, (UINT)bone.keyFrames.size());

			bones.push_back(bone);
		}
		std::vector<AnimationClip*>& vecAnimClip = loader.GetAnimClip();

		for (UINT i = 0; i < vecAnimClip.size(); ++i)
		{
			BoneAnimationClip tClip = {};

			tClip.name = vecAnimClip[i]->name;
			tClip.startTime = vecAnimClip[i]->startTime.GetSecondDouble();
			tClip.endTime = vecAnimClip[i]->endTime.GetSecondDouble();
			tClip.timeLength = tClip.endTime - tClip.startTime;

			tClip.startFrame = (int)vecAnimClip[i]->startTime.GetFrameCount(vecAnimClip[i]->mode);
			tClip.endFrame = (int)vecAnimClip[i]->endTime.GetFrameCount(vecAnimClip[i]->mode);
			tClip.frameLength = tClip.endFrame - tClip.startFrame;
			tClip.mode = vecAnimClip[i]->mode;

			animClip.push_back(tClip);
		}

		meshData->mMeshes = meshes;
		meshData->mMaterialsVec = materialsVec;
		meshData->mFullPath = fullPath;
		meshData->mAnimClip = animClip;
		meshData->mBones = bones;

		// Animation 이 있는 Mesh 경우 structuredbuffer 만들어두기
		if (meshData->IsAnimMesh())
		{
			// BoneOffet 행렬
			std::vector<Matrix> vecOffset;
			//std::vector<BoneFrameTransform> vecFrameTrans;
			//vecFrameTrans.resize((UINT)meshData->mBones.size() * iFrameCount);

			for (size_t i = 0; i < meshData->mBones.size(); ++i)
			{
				vecOffset.push_back(meshData->mBones[i].offset);

	/*			for (size_t j = 0; j < meshData->mBones[i].keyFrames.size(); ++j)
				{
					vecFrameTrans[(UINT)meshData->mBones.size() * j + i]
						= BoneFrameTransform
					{
						Vector4(meshData->mBones[i].keyFrames[j].translate.x
							, meshData->mBones[i].keyFrames[j].translate.y
							, meshData->mBones[i].keyFrames[j].translate.z, 0.f)
						, Vector4(meshData->mBones[i].keyFrames[j].scale.x
							, meshData->mBones[i].keyFrames[j].scale.y
							, meshData->mBones[i].keyFrames[j].scale.z, 0.f)
						, meshData->mBones[i].keyFrames[j].rotation
					};
				}*/
			}

			meshData->mBoneOffset = new graphics::StructedBuffer();
			meshData->mBoneOffset->Create(sizeof(Matrix), (UINT)vecOffset.size(), eSRVType::SRV, vecOffset.data(), false);
			meshData->mBoneOffset->GetSize();
			//meshData->mBoneFrameData = new StructedBuffer();
			//meshData->mBoneFrameData->Create(sizeof(BoneFrameTransform), (UINT)vecOffset.size() * iFrameCount
			//	, eSRVType::SRV, vecFrameTrans.data(), false);
		}


		loader.Release();
		return meshData;
	}

	void MeshData::LoadAnimationFromFbx(const std::wstring& path, const std::wstring& name)
	{

		std::filesystem::path parentPath = std::filesystem::current_path().parent_path();
		std::wstring fullPath = parentPath.wstring() + L"\\Resources\\" + path;

		FbxLoader loader;
		loader.Initialize();
		loader.LoadAnimationFbx(fullPath);


		std::vector<Bone*>& vecBone = loader.GetBones();
		UINT iFrameCount = 0;
		std::vector<BoneMatrix>* meshBones = GetBones();
		UINT animCount = GetAnimationClipCount();
		SetAnimationClipCount(animCount + 1);
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

			PushBackAnimClip(tClip);
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

		graphics::StructedBuffer* boneFrameData = new graphics::StructedBuffer();
		boneFrameData->Create(sizeof(BoneFrameTransform), (UINT)meshBones->size() * iFrameCount
			, eSRVType::SRV, vecFrameTrans.data(), false);
		PushBackBoneFrameData(boneFrameData);

		loader.Release();
	}

	void MeshData::Save(const std::wstring& path)
	{
	}
	MeshObject* MeshData::Instantiate(eLayerType type)
	{

		std::vector<GameObject*> ret = {};
		std::wstring name = std::filesystem::path(mFullPath).stem();
		MeshObject* meshObject = object::Instantiate<MeshObject>(type);
		meshObject->SetName(name + L".All");
		for (size_t i = 0; i < mMeshes.size(); i++)
		{
			GameObject* gameObj = object::Instantiate<GameObject>(type);
			gameObj->SetName(name +L"." + std::to_wstring(i));
			MeshRenderer* mr = gameObj->AddComponent<MeshRenderer>();
			mr->SetMesh(mMeshes[i]);

			for (size_t k = 0; k < mMaterialsVec[i].size(); k++)
			{
				mr->SetMaterial(mMaterialsVec[i][k], k);
			}
			meshObject->PushBackObject(gameObj);
			mChildObjects.push_back(gameObj);
			if (mAnimationClipCount > 0)
			{
				BoneAnimator* animator = gameObj->AddComponent<BoneAnimator>();
				animator->SetBones(&mBones);
				animator->SetAnimaitionClip(&mAnimClip);

				if (i == 0)
					mRepresentBoneAnimator = animator;
			}
		}

		return meshObject;
	}
	void MeshData::Play(const std::wstring animName)
	{
		for (size_t i = 0; i < mChildObjects.size(); i++)
		{
			BoneAnimator* animator = mChildObjects[i]->GetComponent<BoneAnimator>();
			animator->Play(animName);
		}
	}
	std::function<void()>& MeshData::GetAnimationStartEvent(const std::wstring& name)
	{
		return mRepresentBoneAnimator->GetStartEvent(name);
	}
	std::function<void()>& MeshData::GetAnimationCompleteEvent(const std::wstring& name)
	{
		return mRepresentBoneAnimator->GetCompleteEvent(name);

	}
	std::function<void()>& MeshData::GetAnimationEndEvent(const std::wstring& name)
	{
		return mRepresentBoneAnimator->GetEndEvent(name);
	}
	std::function<void()>& MeshData::GetAnimationFrameEvent(const std::wstring& name, UINT index)
	{
		return mRepresentBoneAnimator->GetFrameEvent(name, index);
	}
}
