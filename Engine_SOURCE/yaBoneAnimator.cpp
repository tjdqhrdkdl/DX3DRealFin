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
		, ComponentType(eComponentType::BoneAnimator)
		, mBones{}
		, mAnimationClips{}
		, mAnimationUpdateTime(0)
		, mAnimaitonBoneMatrix{}
		, mFrameCount(30)
		, mCurrentTime(0.0f)
		, mCurrentClip(0)
		, mFrameIdx(-1)
		, mNextFrameIdx(-1)
		, mRatio(0.0f)
		, mBoneMatrixBuffer(nullptr)
		, mbFinalMatrixUpdate(false)
		, mbAnimChanging(false)
		, mAnimChangeTime(0.2f)
		, mAnimChangeTimeChecker(0)
		, mAnimationTailTime(0.1f)
		
	{
	}

	BoneAnimator::~BoneAnimator()
	{
		if (mParentAnimator==nullptr)
		{
			delete mBoneMatrixBuffer;
			mBoneMatrixBuffer = nullptr;
		}
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
		if (!mbAnimChanging && !mParentAnimator && !mbStop)
		{
			mCurrentTime = 0.0f;
			mAnimationUpdateTime[mCurrentClip] += Time::DeltaTime();
			std::wstring currentName = mAnimationClips->at(mCurrentClip).name;
			Events* events = FindEvents(currentName);

			if (mAnimationUpdateTime[mCurrentClip] >= mAnimationClips->at(mCurrentClip).timeLength - mAnimationTailTime)
			{
				//애니메이션 종료 + 루프 돎
				if (mAnimationSelfChangeBools[mCurrentClip])
					mbAnimChanging = true;
				else
				{
					mAnimationUpdateTime[mCurrentClip] = 0;
					mbAnimChanging = false;
				}
				
				mNextAnimName = currentName;

				events->mCompleteEvent();
				events->mEndEvent();

				mbAnimationComplete = true;
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
		}
		
		else if (!mParentAnimator && !mbStop)
		{
			mAnimChangeTimeChecker += Time::DeltaTime();

			if (mAnimChangeTimeChecker > mAnimChangeTime)
			{
				mAnimChangeTimeChecker = 0;
				mAnimationUpdateTime[mCurrentClip] = 0;

				std::wstring endAnimName = mAnimationClips->at(mCurrentClip).name;

				Events* events = nullptr;
				events = FindEvents(mAnimationClips->at(mCurrentClip).name);

				if (mbAnimationComplete)
					;
				else if (events)
					events->mEndEvent();

				//새로운 애니메이션으로 변경
				mCurrentClip = mAnimationNameAndIndexMap[mNextAnimName];
				mFrameIdx = 0;

				events = FindEvents(mAnimationClips->at(mCurrentClip).name);

				if (mbAnimationComplete)
					mbAnimationComplete = false;
				else if (events)
					events->mStartEvent();


				mbAnimChanging = false;
				mRatio = 0;
			}
			else
				mRatio = mAnimChangeTimeChecker / mAnimChangeTime;
		}

		// 컴퓨트 쉐이더 연산여부
		mbFinalMatrixUpdate = false;
	}

	void BoneAnimator::Binds()
	{
		if (!mbFinalMatrixUpdate && !mParentAnimator)
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
			if (mbAnimChanging)
			{
				UINT nextClip = mAnimationNameAndIndexMap[mNextAnimName];
				boneShader->SetNextFrameDataBuffer(meshData->GetBoneFrameData()[nextClip]);
			}
			boneShader->SetFrameDataBuffer(meshData->GetBoneFrameData()[mCurrentClip]);
			boneShader->SetOffsetMatBuffer(meshData->GetBoneOffset());
			boneShader->SetOutputBuffer(mBoneMatrixBuffer);

			UINT iBoneCount = (UINT)mBones->size();
			boneShader->SetBoneCount(iBoneCount);
			boneShader->SetFrameIndex(mFrameIdx);
			boneShader->SetNextFrameIdx(mNextFrameIdx);
			boneShader->SetFrameRatio(mRatio);
			boneShader->SetAnimChange(mbAnimChanging);

			// 업데이트 쉐이더 실행
			boneShader->OnExcute();

			mbFinalMatrixUpdate = true;
		}
		else if (mParentAnimator)
			mBoneMatrixBuffer = mParentAnimator->mBoneMatrixBuffer;

		mBoneMatrixBuffer->BindSRV(eShaderStage::VS, 30);
	}

	void BoneAnimator::Render()
	{
	}

	void BoneAnimator::Play(const std::wstring& animName)
	{
		//예외처리
		if (mAnimationNameAndIndexMap.find(animName) == mAnimationNameAndIndexMap.end())
			assert(NULL);

		mNextAnimName = animName;
		mbAnimChanging = true;
		////기존 애니메이션 정리
		//mAnimationUpdateTime[mCurrentClip] = 0;

		//Events* events = nullptr;
		//events = FindEvents(mAnimationClips->at(mCurrentClip).name);

		//if (events)
		//	events->mEndEvent();

		////새로운 애니메이션으로 변경
		//mCurrentClip = mAnimationNameAndIndexMap[animName];
		//mFrameIdx = 0;
		//
		//events = FindEvents(mAnimationClips->at(mCurrentClip).name);
		//
		//if (events)
		//	events->mStartEvent();
	}

	void BoneAnimator::CheckBone()
	{
		//UINT boneCount = mesh->get 메시가 본 정보를 다 들고있게 해두자
		UINT iBoneCount = (UINT)mBones->size();
		if (mBoneMatrixBuffer->GetStride() != iBoneCount)
		{
			mBoneMatrixBuffer->Create(sizeof(Matrix), iBoneCount, eSRVType::UAV, nullptr, false);
		}
	}

	void BoneAnimator::SetAnimaitionClip(const std::vector<BoneAnimationClip>* clips)
	{
		mAnimationClips = clips;
		mAnimationUpdateTime.resize(mAnimationClips->size());
		mAnimationSelfChangeBools.resize(mAnimationClips->size());
		for (size_t i = 0; i < mAnimationUpdateTime.size(); i++)
		{
			mAnimationNameAndIndexMap.insert(std::pair(clips->at(i).name, (UINT)i));
			mAnimationUpdateTime[i] = 0;

			Events* events = new Events();
			events->mFrameEvents.resize(clips->at(i).endFrame);
			mEvents.insert(std::make_pair(clips->at(i).name, events));

			mAnimationSelfChangeBools[i] = true;
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
