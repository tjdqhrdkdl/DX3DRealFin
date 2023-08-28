#include "yaBoneAnimator.h"
#include "yaGameObject.h"
#include "yaMeshRenderer.h"
#include "yaStructedBuffer.h"
#include "yaResources.h"
#include "yaTime.h"
#include "yaFbxLoader.h"
#include "yaBoneShader.h"
#include "yaMeshData.h"
namespace ya
{
	BoneAnimator::BoneAnimator()
		: Component(eComponentType::BoneAnimator)
		, mBones{}
		, mAnimationClips{}
		, mAnimationUpdateTime(0.0f)
		, mAnimaitonBoneMatrix{}
		, mFrameCount(30)
		, mCurrentTime(0.0f)
		, mCurrentClip(0)
		, mFrameIdx(-1)
		, mNextFrameIdx(-1)
		, mRatio(0.0f)
		, mBoneMatrixBuffer(nullptr)
		, mbFinalMatrixUpdate(false)
		
	{
		mBoneMatrixBuffer = new graphics::StructedBuffer();
	}

	BoneAnimator::~BoneAnimator()
	{
		delete mBoneMatrixBuffer;
		mBoneMatrixBuffer = nullptr;

		for (auto evt : mEvents)
		{
			delete evt.second;
		}
	}

	void BoneAnimator::Initialize()
	{

	}

	void BoneAnimator::Update()
	{
	}

	void BoneAnimator::FixedUpdate()
	{
		mCurrentTime = 0.0f;
		mAnimationUpdateTime[mCurrentClip] += Time::DeltaTime();

		Events* events = FindEvents(mAnimationClips->at(mCurrentClip).name);

		if (mAnimationUpdateTime[mCurrentClip] >= mAnimationClips->at(mCurrentClip).timeLength)
		{
			//애니메이션 종료 + 루프 돎
			mAnimationUpdateTime[mCurrentClip] = 0.f;
			if (events)
				events->mCompleteEvent();
			
		}

		mCurrentTime = mAnimationClips->at(mCurrentClip).startTime + mAnimationUpdateTime[mCurrentClip];

		// 현재 프레임 인덱스 구하기
		double dFrameIdx = mCurrentTime * (double)mFrameCount;
		mFrameIdx = (int)(dFrameIdx);

		// 다음 프레임 인덱스
		if (mFrameIdx >= mAnimationClips->at(mCurrentClip).frameLength - 1)
			mNextFrameIdx = mFrameIdx;	// 끝이면 현재 인덱스를 유지
		else
		{
			mNextFrameIdx = mFrameIdx + 1;
			if (events->mFrameEvents[mNextFrameIdx].mEvent)
				events->mFrameEvents[mNextFrameIdx].mEvent();
		}

		// 프레임간의 시간에 따른 비율을 구해준다.
		mRatio = (float)(dFrameIdx - (double)mFrameIdx);

		// 컴퓨트 쉐이더 연산여부
		mbFinalMatrixUpdate = false;
	}

	void BoneAnimator::Binds()
	{
		if (!mbFinalMatrixUpdate)
		{
			//Compute shader
			std::shared_ptr<BoneShader> boneShader
				= Resources::Find<BoneShader>(L"BoneComputeShader");

			// Bone Data
			GameObject* gameObj = GetOwner();
			MeshRenderer* mr = gameObj->GetComponent<MeshRenderer>();
			std::shared_ptr<Mesh> mesh = mr->GetMesh();
			CheckBone();
			MeshData* meshData = mesh->GetParentMeshData();
			boneShader->SetFrameDataBuffer(meshData->GetBoneFrameData()[mCurrentClip]);
			boneShader->SetOffsetMatBuffer(meshData->GetBoneOffset());
			boneShader->SetOutputBuffer(mBoneMatrixBuffer);

			UINT iBoneCount = (UINT)mBones->size();
			boneShader->SetBoneCount(iBoneCount);
			boneShader->SetFrameIndex(mFrameIdx);
			boneShader->SetNextFrameIdx(mNextFrameIdx);
			boneShader->SetFrameRatio(mRatio);

			// 업데이트 쉐이더 실행
			boneShader->OnExcute();

			mbFinalMatrixUpdate = true;
		}

		mBoneMatrixBuffer->BindSRV(eShaderStage::VS, 30);
	}

	void BoneAnimator::Render()
	{
	}

	void BoneAnimator::Play(const std::wstring& animName)
	{
		//예외처리
		if (mAnimationNameAndIndexMap.find(animName) == mAnimationNameAndIndexMap.end())
			assert(L"존재하지 않는 애니메이션을 Play 중입니다.");


		//기존 애니메이션 정리
		mAnimationUpdateTime[mCurrentClip] = 0;

		Events* events = nullptr;
		events = FindEvents(mAnimationClips->at(mCurrentClip).name);

		if (events)
			events->mEndEvent();
		//새로운 애니메이션으로 변경
		mCurrentClip = mAnimationNameAndIndexMap[animName];
		events = FindEvents(mAnimationClips->at(mCurrentClip).name);

		if (events)
			events->mStartEvent();
	}

	void BoneAnimator::CheckBone()
	{
		//UINT boneCount = mesh->get 메시가 본 정보를 다 들고있게 해두자
		UINT iBoneCount = mBones->size();
		if (mBoneMatrixBuffer->GetStride() != iBoneCount)
		{
			mBoneMatrixBuffer->Create(sizeof(Matrix), iBoneCount, eSRVType::UAV, nullptr, false);
		}
	}

	void BoneAnimator::SetAnimaitionClip(const std::vector<BoneAnimationClip>* clips)
	{
		mAnimationClips = clips;
		mAnimationUpdateTime.resize(mAnimationClips->size());
		
		for (size_t i = 0; i < mAnimationUpdateTime.size(); i++)
		{
			mAnimationNameAndIndexMap.insert(std::pair(clips->at(i).name, i));
			mAnimationUpdateTime[i] = 0;

			Events* events = new Events();
			events->mFrameEvents.resize(clips->at(i).endFrame);
			mEvents.insert(std::make_pair(clips->at(i).name, events));
		}


	}

	BoneAnimator::Events* BoneAnimator::FindEvents(const std::wstring& name)
	{
		std::map<std::wstring, Events*>::iterator iter
			= mEvents.find(name);

		if (iter == mEvents.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	void BoneAnimator::ClearData()
	{
		mBoneMatrixBuffer->Clear();

		GameObject* gameObj = GetOwner();
		MeshRenderer* mr = gameObj->GetComponent<MeshRenderer>();
		UINT iMtrlCount = mr->GetMaterialCount();
		std::shared_ptr<Material> pMtrl = nullptr;
		for (UINT i = 0; i < iMtrlCount; ++i)
		{
			pMtrl = mr->GetMaterial(i);
			if (nullptr == pMtrl)
				continue;

			pMtrl->SetAnimation(false); // Animation Mesh 알리기
		}
	}


	std::function<void()>& BoneAnimator::GetStartEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);

		return events->mStartEvent.mEvent;
	}
	std::function<void()>& BoneAnimator::GetCompleteEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);

		return events->mCompleteEvent.mEvent;
	}
	std::function<void()>& BoneAnimator::GetEndEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);

		return events->mEndEvent.mEvent;
	}
	std::function<void()>& BoneAnimator::GetFrameEvent(const std::wstring& name, UINT index)
	{
		Events* events = FindEvents(name);

		return events->mFrameEvents[index].mEvent;
	}

}
