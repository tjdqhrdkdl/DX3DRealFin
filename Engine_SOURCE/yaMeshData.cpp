#include "yaMeshData.h"
#include "yaMeshData.h"
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
#include "yaAnimationData.h"
namespace ya
{
	MeshData::MeshData()
		: Resource(eResourceType::MeshData)
		, mAnimationClipCount(0)
		, mIFrameCount(0)
		, mBoneOffset(nullptr)
		, mbBoundarySphere(false)
	{
	}
	MeshData::~MeshData()
	{
		for (size_t i = 0; i < mBoneFrameDataVector.size(); i++)
		{
			delete mBoneFrameDataVector[i];
		}
		if(mBoneOffset != nullptr)
			delete mBoneOffset;
	}
	std::shared_ptr<MeshData> MeshData::LoadFromFbx(const std::wstring& path)
	{
		std::filesystem::path fullPath = std::fs::absolute(gResPath);
		fullPath /= path;
		
		std::shared_ptr<MeshData> meshSharedPtr = std::make_shared<MeshData>();

		FbxLoader loader;
		loader.Initialize();
		loader.LoadFbx(fullPath);
		meshSharedPtr->mMeshCenter = loader.GetMeshCenter();

		// 메시들 가져오기
		std::vector<std::shared_ptr<Mesh>> meshes = Mesh::CreateFromContainer(&loader);
		std::vector<std::vector<std::shared_ptr<Material>>>  materialsVec = {};
		meshSharedPtr->mBoundarySphereRadius = loader.GetMaxDist();

		std::shared_ptr<Mesh> mesh = nullptr;
		for (int i = 0; i < (int)meshes.size(); i++)
		{
			mesh = meshes[i];

			// 리소스에 넣어주기
			
			std::wstring name = std::filesystem::path(fullPath).stem();
			name += L".mesh" + std::to_wstring(i);
			Resources::Insert(name, mesh);

			// 메테리얼 가져오기
			std::vector<std::shared_ptr<Material>> materials = {};
			for (size_t k = 0; k < loader.GetContainer(i).materials.size(); k++)
				{
				size_t test = loader.GetContainer(i).materials.size();
					std::shared_ptr<Material> material
						= Resources::Find<Material>(loader.GetContainer(i).materials[k].name);
					std::wstring wtest = loader.GetContainer(i).materials[k].name;
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

		meshSharedPtr->mMeshes = meshes;
		meshSharedPtr->mMaterialsVec = materialsVec;
		meshSharedPtr->mFullPath = fullPath;
		meshSharedPtr->mAnimClip = animClip;
		meshSharedPtr->mBones = bones;


		// Animation 이 있는 Mesh 경우 structuredbuffer 만들어두기
		if (meshSharedPtr->IsAnimMesh())
		{
			// BoneOffet 행렬
			std::vector<Matrix> vecOffset;
			//std::vector<BoneFrameTransform> vecFrameTrans;
			//vecFrameTrans.resize((UINT)meshData->mBones.size() * iFrameCount);

			for (size_t i = 0; i < meshSharedPtr->mBones.size(); ++i)
			{
				vecOffset.push_back(meshSharedPtr->mBones[i].offset);

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


			meshSharedPtr->mBoneOffset = new graphics::StructedBuffer();
			meshSharedPtr->mBoneOffset->Create(sizeof(Matrix), (UINT)vecOffset.size(), eSRVType::SRV, vecOffset.data(), false);
			meshSharedPtr->mBoneOffset->GetSize();

			//meshData->mBoneFrameData = new StructedBuffer();
			//meshData->mBoneFrameData->Create(sizeof(BoneFrameTransform), (UINT)vecOffset.size() * iFrameCount
			//	, eSRVType::SRV, vecFrameTrans.data(), false);
		}
		const std::wstring meshDataName
			= std::filesystem::path(fullPath).stem();


		Resources::Insert(meshDataName, meshSharedPtr);

		//std::wstring name = std::filesystem::path(fullPath).stem();
		//name += L".mesh" + std::to_wstring(i);
		
		meshSharedPtr->Save(path);

		loader.Release();
		return meshSharedPtr;
	}



	void MeshData::LoadAnimationFromFbx(const std::wstring& path, const std::wstring& name)
	{
		std::filesystem::path fullPath = std::fs::absolute(gResPath) / path;

		FbxLoader loader;
		loader.Initialize();
		loader.LoadAnimationFbx(fullPath);

		//애니메이션 데이터의 본
		std::vector<Bone*>& vecBone = loader.GetBones();
		UINT iFrameCount = 0;
		//메쉬 데이터에 저장된 본
		std::vector<BoneMatrix>* meshBones = &mBones;

		//애니메이션 데이터의 본
		
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
						tKeyframe.frame = (int)j;
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

						meshBones->at(j).keyFrames[mAnimationClipCount].push_back(tKeyframe);
						//mBonekeyFrame.push_back(tKeyframe);
					}
					break;
				}
			}
			iFrameCount = max(iFrameCount, (UINT)meshBones->at(j).keyFrames[mAnimationClipCount].size());
		}

		mBones;

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

			mAnimClip.push_back(tClip);
		}

		// Animation 이 있는 Mesh 경우 structuredbuffer 만들어두기
		std::vector<BoneFrameTransform> vecFrameTrans;
		vecFrameTrans.resize((UINT)meshBones->size() * iFrameCount);

		for (size_t i = 0; i < meshBones->size(); ++i)
		{
			for (size_t j = 0; j < meshBones->at(i).keyFrames[mAnimationClipCount].size(); ++j)
			{
				vecFrameTrans[(UINT)meshBones->size() * j + i]
					= BoneFrameTransform
				{
					Vector4(meshBones->at(i).keyFrames[mAnimationClipCount][j].translate.x
						, meshBones->at(i).keyFrames[mAnimationClipCount][j].translate.y
						, meshBones->at(i).keyFrames[mAnimationClipCount][j].translate.z, 0.f)
					, Vector4(meshBones->at(i).keyFrames[mAnimationClipCount][j].scale.x
						, meshBones->at(i).keyFrames[mAnimationClipCount][j].scale.y
						, meshBones->at(i).keyFrames[mAnimationClipCount][j].scale.z, 0.f)
					, meshBones->at(i).keyFrames[mAnimationClipCount][j].rotation
				};						
			}
		}


		//애니메이션당 한개.
		graphics::StructedBuffer* boneFrameData = new graphics::StructedBuffer();
		boneFrameData->Create(sizeof(BoneFrameTransform), (UINT)meshBones->size() * iFrameCount
			, eSRVType::SRV, vecFrameTrans.data(), false);
		mBoneFrameDataVector.push_back(boneFrameData);

		
		//AnimationSave(path, name);


		mAnimationClipCount += 1;

		loader.Release();
	}

	HRESULT MeshData::Save(const std::wstring& path, FILE* file)
	{	
		std::filesystem::path filePath = std::fs::absolute(gResPath);

		std::filesystem::path parentPath(path);
		parentPath = parentPath.parent_path().parent_path();		
		parentPath /= L"MeshData";

		filePath /= parentPath;
		if (false == std::fs::exists(filePath))
		{
			std::fs::create_directories(filePath);
		}

		filePath /= std::fs::path(path).filename();

		filePath.replace_extension(L".meshdata");


		file = nullptr;
		_wfopen_s(&file, filePath.wstring().c_str(), L"wb");
		if (file == nullptr)
			return S_FALSE;


		//메쉬 개수 저장
		UINT meshCount = (UINT)mMeshes.size();
		fwrite(&meshCount, sizeof(UINT), 1, file);		

		for (size_t i = 0; i < mMeshes.size(); i++)
		{			
			mMeshes[i]->Save(filePath.filename().wstring(), file);

			//메테리얼 개수 저장
			UINT  mMaterial_ICount = (UINT)mMaterialsVec.size();
			fwrite(&mMaterial_ICount, sizeof(UINT), 1, file);

			for (size_t j = 0; j < mMaterialsVec.size(); j++)
			{
				UINT  mMaterial_JCount = (UINT)mMaterialsVec[j].size();
				fwrite(&mMaterial_JCount, sizeof(UINT), 1, file);

				for (size_t k = 0; k < mMaterialsVec[j].size(); k++)
				{
					std::wstring matName = mMaterialsVec[j][k]->GetName();
					std::wstring matkey = mMaterialsVec[j][k]->GetKey();
					SaveWString(matkey, file);
					mMaterialsVec[j][k]->Save(filePath.filename().wstring(), file);
					eRenderingMode renderingmode = mMaterialsVec[j][k]->GetRenderingMode();
					fwrite(&renderingmode, sizeof(eRenderingMode), 1, file);
				}
			}
		}

		bool AniMesh = IsAnimMesh();
		fwrite(&AniMesh, sizeof(bool), 1, file);

		if (IsAnimMesh())
		{
			UINT boneSize = (UINT)mBones.size();
			fwrite(&boneSize, sizeof(UINT), 1, file);
			//std::vector<BoneFrameTransform> vecFrameTrans;
			//vecFrameTrans.resize((UINT)meshData->mBones.size() * iFrameCount);
			for (size_t i = 0; i < mBones.size(); ++i)
			{	
				fwrite(&mBones[i].offset, sizeof(Matrix), 1, file);
			}		
		}		

		fwrite(&mMeshCenter, sizeof(Vector3), 1, file);
		fwrite(&mBoundarySphereRadius, sizeof(float), 1, file);

		fclose(file);

		return S_OK;	

	}

	HRESULT MeshData::Load(const std::wstring& path, FILE* file)
	{
		std::fs::path filePath(path);
		std::wstring extension = filePath.extension();
		extension = utils::String::UpperCase(extension);

		//FBX일 경우 변환된 meshdata가 존재하는지 확인한다
		if (L".FBX" == extension)
		{
			std::fs::path meshDataPath = std::fs::absolute(gResPath) / filePath.parent_path().parent_path();
			meshDataPath /= L"MeshData";
			meshDataPath /= filePath.filename();
			meshDataPath.replace_extension(L".meshdata");

			//meshdata 파일 있으면 그걸로 로드, 아니면 FBX로부터 로드
			if (false == std::fs::exists(meshDataPath))
			{
				if(FAILED(LoadFromFbxToThis(std::fs::absolute(gResPath) / filePath)))
					return E_FAIL;
				
				return S_OK;
			}
			filePath = meshDataPath;
		}
		else
		{
			filePath = std::fs::absolute(gResPath) / filePath;
		}

		file = nullptr;
		_wfopen_s(&file, filePath.c_str(), L"rb");
		if (file == nullptr)
			return S_FALSE;


		UINT meshSize = 0;
		fread(&meshSize, sizeof(UINT), 1, file);	

		mMeshes.resize(meshSize);

		//MeshData* meshData = new MeshData();
		std::vector<std::vector<std::shared_ptr<Material>>>  materialsVec = {};

		for (size_t i = 0; i < meshSize; i++)
		{
			mMeshes[i] = std::make_shared<Mesh>();
			mMeshes[i]->Load(filePath, file);

			std::wstring name = std::filesystem::path(path).stem();
			name += L".mesh" + std::to_wstring(i);
			Resources::Insert(name, mMeshes[i]);


			UINT matICount = 0;
			fread(&matICount, sizeof(UINT), 1, file);

			mMaterialsVec.resize(matICount);

			for (size_t j = 0; j < mMaterialsVec.size(); j++)
			{
				UINT matJCount = 0;
				fread(&matJCount, sizeof(UINT), 1, file);

				mMaterialsVec[j].resize(matJCount);
				for (size_t k = 0; k < mMaterialsVec[j].size(); k++)
				{
					std::wstring matName;
					mMaterialsVec[j][k] = std::make_shared<Material>();					
					LoadWString(matName, file);
					Resources::Insert<Material>(matName, mMaterialsVec[j][k]);
					mMaterialsVec[j][k]->Load(name, file);
					eRenderingMode renderingmode;
					fread(&renderingmode, sizeof(eRenderingMode), 1, file);
					mMaterialsVec[j][k]->SetRenderingMode(renderingmode);
				}				
			}
		}	

		bool AniMesh;
		fread(&AniMesh, sizeof(bool), 1, file);
		std::vector<Matrix> vecOffset;

		if (AniMesh)
		{
			UINT boneSize;
			fread(&boneSize, sizeof(UINT), 1, file);
			
			
			
			for (size_t i = 0; i < boneSize; ++i)
			{
				Matrix matrix;
				fread(&matrix, sizeof(Matrix), 1, file);
				vecOffset.push_back(matrix);
			}


			mBoneOffset = new graphics::StructedBuffer();
			mBoneOffset->Create(sizeof(Matrix), (UINT)vecOffset.size(), eSRVType::SRV, vecOffset.data(), false);
			mBoneOffset->GetSize();
			
		}

		fread(&mMeshCenter, sizeof(Vector3), 1, file);
		fread(&mBoundarySphereRadius, sizeof(float), 1, file);

		fclose(file);

						
		mBones;
		mMeshes;
		mAnimClip;

		//mFullPath = CurparentPath;

		

		return S_OK;
	}

	HRESULT MeshData::AnimationSave(const std::wstring& path, FILE* file)
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
		_wfopen_s(&file, fullPath.c_str(), L"wb");
		if (file == nullptr)
			return S_FALSE;


		//본 사이즈 저장
		UINT boneSize = (UINT)mBones.size();
		fwrite(&boneSize, sizeof(UINT), 1, file);


		//애니 클립 사이즈 저장
		UINT AnimClipSize = mAnimationClipCount;
		fwrite(&AnimClipSize, sizeof(UINT), 1, file);

		
		for (size_t i = 0; i < AnimClipSize; i++)
		{
			SaveWString(mAnimClip[i].name, file);		
			fwrite(&mAnimClip[i].startTime, sizeof(double), 1, file);
			fwrite(&mAnimClip[i].endTime, sizeof(double), 1, file);
			fwrite(&mAnimClip[i].timeLength, sizeof(double), 1, file);
			fwrite(&mAnimClip[i].mode, sizeof(int), 1, file);
			fwrite(&mAnimClip[i].updateTime, sizeof(float), 1, file);
			fwrite(&mAnimClip[i].startFrame, sizeof(int), 1, file);
			fwrite(&mAnimClip[i].endFrame, sizeof(int), 1, file);
			fwrite(&mAnimClip[i].frameLength, sizeof(int), 1, file);
		}
		
		for (size_t i = 0; i < boneSize; i++)
		{
			SaveWString(mBones[i].name, file);
			fwrite(&mBones[i].depth, sizeof(int), 1, file);
			fwrite(&mBones[i].parentIdx, sizeof(int), 1, file);
			fwrite(&mBones[i].bone, sizeof(Matrix), 1, file);
			fwrite(&mBones[i].offset, sizeof(Matrix), 1, file);
						

			for (size_t j = 0; j < AnimClipSize; j++)
			{				

				UINT boneKeyFramesSize = (UINT)mBones[i].keyFrames[j].size();
				fwrite(&boneKeyFramesSize, sizeof(UINT), 1, file);
				for (size_t k = 0; k < boneKeyFramesSize; k++)
				{	
					fwrite(&mBones[i].keyFrames[j][k].time, sizeof(double), 1, file);
					fwrite(&mBones[i].keyFrames[j][k].frame, sizeof(int), 1, file);
					fwrite(&mBones[i].keyFrames[j][k].translate, sizeof(Vector3), 1, file);
					fwrite(&mBones[i].keyFrames[j][k].scale, sizeof(Vector3), 1, file);
					fwrite(&mBones[i].keyFrames[j][k].rotation, sizeof(Vector4), 1, file);
				}
			}
		}

		fclose(file);

		return S_OK;
	}

	HRESULT MeshData::AnimationLoad(const std::wstring& path, FILE* file, bool bLast)
	{
		if (mAnimationData == nullptr)
			mAnimationData = std::make_shared<AnimationData>();
		if (bLast)
		{		//Find 애니메이션 데이터
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
			std::shared_ptr<AnimationData> resource = Resources::Find<AnimationData>(key);

			if (resource != nullptr)
			{
				mAnimationData = resource;
			}
			else
			{
				mAnimationData->LoadFin(path);
				Resources::Insert<AnimationData>(fullPath, mAnimationData);

			}
		}
		else
			mAnimationData->LoadNotFin(path);

		return S_OK;

	}


	MeshObject* MeshData::Instantiate(eLayerType type, Scene* scene, const std::wstring& name)
	{
		std::wstring objName = name;
		if(name == L"")
			objName = std::filesystem::path(mFullPath).stem();
		
		std::vector<GameObject*> ret = {};

		MeshObject* meshObject = nullptr;
		if(scene == nullptr)
			meshObject = object::Instantiate<MeshObject>(type);	
		else
			meshObject = object::Instantiate<MeshObject>(type, scene);	

		meshObject->SetName(objName + L".All");

		GameObject* gameObj = nullptr;
		if (scene == nullptr)
			gameObj = object::Instantiate<GameObject>(type);
		else
			gameObj = object::Instantiate<GameObject>(type, scene);
		gameObj->SetName(objName);
		meshObject->PushBackObject(gameObj);
		mChildObjects.push_back(gameObj);

		for (size_t i = 0; i < mMeshes.size(); i++)
		{
			MeshRenderer* mr = gameObj->AddComponent<MeshRenderer>();
			mr->SetMesh(mMeshes[i]);
			mMeshes[i]->SetParentMeshData(this);

			for (size_t k = 0; k < mMaterialsVec[i].size(); k++)
			{
				mr->SetMaterial(mMaterialsVec[i][k], (int)k);
			}
		}

		if (mAnimationData != nullptr || mAnimationClipCount > 0)
		{
			BoneAnimator* animator = gameObj->AddComponent<BoneAnimator>();
			mRepresentBoneAnimator = animator;
			animator->SetBones(GetBones());
			animator->SetAnimaitionClip(GetAnimClips());
		}

		if(mbBoundarySphere)
		{
			BoundarySphere* sphere = meshObject->AddComponent<BoundarySphere>();
			sphere->SetCenter(mMeshCenter);
			sphere->SetRadius(mBoundarySphereRadius * 2);
		}

		meshObject->SetParent();
		mMeshObject = meshObject;
		return meshObject;
	}
	void MeshData::Play(const std::wstring animName)
	{
		mRepresentBoneAnimator->Play(animName);
	}
	std::wstring MeshData::GetPlayAnimationName()
	{
		if (mRepresentBoneAnimator)
			return mRepresentBoneAnimator->GetPlayAnimationName();

		return L"";
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

	std::vector<BoneMatrix>* MeshData::GetBones()
	{
		if (mAnimationData)
			return mAnimationData->GetBones();
		else
			return &mBones;
	}

	std::vector<BoneAnimationClip>* MeshData::GetAnimClips()
	{
		if (mAnimationData)
			return mAnimationData->GetAnimClips();
		else
			return &mAnimClip;
	}

	UINT MeshData::GetAnimationClipCount()
	{
		if (mAnimationData)
			return mAnimationData->GetAnimationClipCount();
		else
			return mAnimationClipCount;
	}

	std::vector<graphics::StructedBuffer*> MeshData::GetBoneFrameData()
	{
		if (mAnimationData)
			return mAnimationData->GetBoneFrameDataVector();
		else
			return mBoneFrameDataVector;
	}


	void MeshData::SaveWString(const std::wstring& _str, FILE* _pFile)
	{
		size_t iLen = _str.length();
		fwrite(&iLen, sizeof(size_t), 1, _pFile);
		fwrite(_str.c_str(), sizeof(wchar_t), iLen, _pFile);

	}
	void MeshData::LoadWString(std::wstring& _str, FILE* _pFile)
	{
		size_t iLen = 0;
		fread(&iLen, sizeof(size_t), 1, _pFile);

		wchar_t szBuff[256] = {};
		fread(szBuff, sizeof(wchar_t), iLen, _pFile);
		_str = szBuff;
	}


	HRESULT MeshData::LoadFromFbxToThis(const std::fs::path& _fullPath)
	{
		FbxLoader loader;
		loader.Initialize();

		if (false == loader.LoadFbx(_fullPath))
			return E_FAIL;

		// 메시들 가져오기
		std::vector<std::shared_ptr<Mesh>> meshes = Mesh::CreateFromContainer(&loader);
		std::vector<std::vector<std::shared_ptr<Material>>>  materialsVec = {};
		mMeshCenter = loader.GetMeshCenter();
		mBoundarySphereRadius = loader.GetMaxDist();

		std::shared_ptr<Mesh> mesh = nullptr;
		for (int i = 0; i < (int)meshes.size(); i++)
		{
			mesh = meshes[i];

			// 리소스에 넣어주기
			std::wstring name = _fullPath.stem();
			name += L".mesh" + std::to_wstring(i);
			Resources::Insert(name, mesh);

			// 메테리얼 가져오기
			std::vector<std::shared_ptr<Material>> materials = {};
			for (size_t k = 0; k < loader.GetContainer(i).materials.size(); k++)
			{
				size_t test = loader.GetContainer(i).materials.size();
				std::shared_ptr<Material> material
					= Resources::Find<Material>(loader.GetContainer(i).materials[k].name);
				std::wstring wtest = loader.GetContainer(i).materials[k].name;
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

		mMeshes = meshes;
		mMaterialsVec = materialsVec;
		mFullPath = _fullPath;
		mAnimClip = animClip;
		mBones = bones;


		// Animation 이 있는 Mesh 경우 structuredbuffer 만들어두기
		if (IsAnimMesh())
		{
			// BoneOffet 행렬
			std::vector<Matrix> vecOffset;
			//std::vector<BoneFrameTransform> vecFrameTrans;
			//vecFrameTrans.resize((UINT)meshData->mBones.size() * iFrameCount);

			for (size_t i = 0; i < mBones.size(); ++i)
			{
				vecOffset.push_back(mBones[i].offset);

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


			mBoneOffset = new graphics::StructedBuffer();
			mBoneOffset->Create(sizeof(Matrix), (UINT)vecOffset.size(), eSRVType::SRV, vecOffset.data(), false);
			mBoneOffset->GetSize();

			//meshData->mBoneFrameData = new StructedBuffer();
			//meshData->mBoneFrameData->Create(sizeof(BoneFrameTransform), (UINT)vecOffset.size() * iFrameCount
			//	, eSRVType::SRV, vecFrameTrans.data(), false);
		}
		const std::wstring meshDataName
			= _fullPath.stem().wstring();

		std::fs::path keyPath = _fullPath.lexically_relative(std::fs::absolute(gResPath));

		Save(keyPath);

		loader.Release();

		return S_OK;
	}
}
