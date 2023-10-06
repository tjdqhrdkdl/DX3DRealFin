#pragma once
#include "yaGameObject.h"
#include "yaGraphics.h"

namespace ya
{
	
	class MeshData;
	class BoneCollider : public GameObject
	{
		struct StartAndFin
		{
			UINT start;
			UINT fin;
		};
	public:
		BoneCollider();
		~BoneCollider();


		void Initialize()	override;
		void Update()		override;
		void FixedUpdate()	override;
		void Render()		override;


		void SetMeshAndBone(std::shared_ptr<MeshData> meshData, std::wstring bone, GameObject* owner = nullptr);
		void SetScale(Vector3 scale);
		void SetAnimOffSet(UINT animIdx, Vector3 offset);
		void SetAnimOffSet(const std::wstring& animName, Vector3 offset);

		void SetColliderActiveFrame(UINT animIdx,  UINT start, UINT finish);
		void SetColliderActiveFrame(const std::wstring& animName, UINT start, UINT finish);
		
		void AddHitObjects(GameObject* obj) { mHitObjects.push_back(obj); }
		bool CheckHitObjects(GameObject* obj);

		GameObject* GetBoneColliderOwner() { return mOwner; }
	private:
		GameObject* mOwner;
		std::shared_ptr<MeshData> mMeshData;
		graphics::BoneMatrix mBoneMat;
		int mBeforeClipIdx;

		std::vector<Vector3> mAnimationOffsets;
		std::vector<std::vector<StartAndFin>> mAnimationColActiveFrame;

		std::vector<GameObject*> mHitObjects;
	};
}
