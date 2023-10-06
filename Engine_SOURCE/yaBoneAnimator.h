#pragma once
#include "yaComponent.h"
#include "yaTexture.h"
#include "yaMesh.h"
#include "yaMaterial.h"

using namespace ya::math;
using namespace ya::graphics;
namespace ya
{

	class StructedBuffer;
	class BoneAnimator : public Component
	{
	public:

		struct Events
		{
			struct Event
			{
				void operator=(std::function<void()> func)
				{
					mEvent = std::move(func);
				}
				void operator()()
				{
					if (mEvent)
						mEvent();
				}

				std::function<void()> mEvent;
			};

			Event mStartEvent;
			Event mCompleteEvent;
			Event mEndEvent;

			std::vector<Event> mFrameEvents;
		};
		BoneAnimator();
		~BoneAnimator();

		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;
		void Play(const std::wstring& animName);
		void Binds();
		void CheckBone();
		void SetAnimaitionClip(const std::vector<BoneAnimationClip>* clips);
		Events* FindEvents(const std::wstring& name);
		graphics::StructedBuffer* GetFinalBoneMatrix() { return mBoneMatrixBuffer; }
		UINT GetBoneCount() { return mBones->size(); }
		void ClearData();
		void SetBones(const std::vector<BoneMatrix>* bones)
		{
			mBoneMatrixBuffer = new graphics::StructedBuffer();
			mBones = bones;
			mAnimaitonBoneMatrix.resize(mBones->size());
		}

		void SetClipTime(int clipIdx, float time) { mAnimationUpdateTime[clipIdx] = time; };
		void SetAnimationSelfChange(UINT idx, bool change) { mAnimationSelfChangeBools[idx] = change; }
		void SetAnimationSelfChange(const std::wstring& name, bool change) { mAnimationSelfChangeBools[GetAnimationIdxByName(name)] = change; }
		void SetAnimationTailTime(float time) { mAnimationTailTime = time; }
		void SetParentAnimator(BoneAnimator* animator) { mParentAnimator = animator; }
		void SetAnimationChangeTime(float time) { mAnimChangeTime = time; }

		std::function<void()>& GetStartEvent(const std::wstring& name);
		std::function<void()>& GetCompleteEvent(const std::wstring& name);
		std::function<void()>& GetEndEvent(const std::wstring& name);
		std::function<void()>& GetFrameEvent(const std::wstring& name, UINT index);
		int GetCurrentClipIdx() { return mCurrentClip; }
		int GetCurrentFrameIdx() { return mFrameIdx; }
		int GetAnimationIdxByName(const std::wstring& name) { if (mAnimationNameAndIndexMap.find(name) == mAnimationNameAndIndexMap.end()) return -1; else return mAnimationNameAndIndexMap[name]; }
		std::wstring GetPlayAnimationName() { return mAnimationClips->at(mCurrentClip).name; }
		bool isChanging() { return mbAnimChanging; }

	private:
		const std::vector<BoneMatrix>* mBones;
		const std::vector<BoneAnimationClip>* mAnimationClips;


		std::vector<float> mAnimationUpdateTime;
		std::vector<Matrix> mAnimaitonBoneMatrix;	//텍스처에 전달될 행렬정보
		int mFrameCount;
		double mCurrentTime;
		int mCurrentClip; // 클립 인덱스

		int mFrameIdx; //클립의 현재프레임
		int mNextFrameIdx; // 클립의 다음 프레임
		float mRatio; //프레임 사이의 비율

		graphics::StructedBuffer* mBoneMatrixBuffer; // 특정프레임의 최종행렬
		bool mbFinalMatrixUpdate;		// 최종행렬 연산 수행여부

		//이름으로 애니메이션을 찾기 위해
		std::map<std::wstring, UINT> mAnimationNameAndIndexMap;
		
		//애니메이션 도중 event
		std::map<std::wstring, Events*> mEvents;

		
		//애니메이션 사이 보간을 위한 변수
		bool mbAnimChanging;
		float mAnimChangeTime;
		float mAnimChangeTimeChecker;
		std::wstring mNextAnimName;

		bool mbAnimationComplete;

		std::vector<bool> mAnimationSelfChangeBools;

		//애니메이션 끝부분 자르기
		float mAnimationTailTime;

		//부모 본애니메이터
		BoneAnimator* mParentAnimator;
	};
}