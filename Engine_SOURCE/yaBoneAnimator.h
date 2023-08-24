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
		BoneAnimator();
		~BoneAnimator();

		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

		void Play(const std::wstring& animName);


		void Binds();

		void CheckBone();
		void SetBones(const std::vector<BoneMatrix>* bones)
		{
			mBones = bones;
			mAnimaitonBoneMatrix.resize(mBones->size());
		}
		void SetAnimaitionClip(const std::vector<BoneAnimationClip>* clips);
		void SetClipTime(int clipIdx, float time) { mAnimationUpdateTime[clipIdx] = time; };

		graphics::StructedBuffer* GetFinalBoneMatrix() { return mBoneMatrixBuffer; }
		UINT GetBoneCount() { return mBones->size(); }
		void ClearData();

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

		std::map<std::wstring, UINT> mAnimationNameAndIndexMap;
	};
}