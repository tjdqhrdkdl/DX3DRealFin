
#include "yaBoneCollider.h"
#include "yaMeshData.h"
#include "yaCollider2D.h"
#include "yaBoneAnimator.h"
namespace ya
{
    BoneCollider::BoneCollider()
    {
    }
    BoneCollider::~BoneCollider()
    {
    }
    void BoneCollider::Initialize()
    {
        mCulPos = GetComponent<Transform>()->GetLocalPosition();
        GameObject::Initialize();
    }
    void BoneCollider::Update()
    {
        mCulPos = GetComponent<Transform>()->GetLocalPosition();
        Collider3D* col = GetComponent<Collider3D>();
        if (col)
        {
            BoneAnimator* animator = mMeshData->GetAnimator();
            if (animator)
            {
                int clip = animator->GetCurrentClipIdx();
                if (mBeforeClipIdx != clip)
                {
                    //콜라이더 센터 변경
                    col->setOffsetPosition(mAnimationOffsets[clip]);
                    mBeforeClipIdx = clip;
                }
                int frame = animator->GetCurrentFrameIdx();
                if (clip >= 0 && frame >= 0)
                {

                    bool active = false;
                    for (size_t i = 0; i < mAnimationColActiveFrame[clip].size(); i++)
                    {
                        if (mAnimationColActiveFrame[clip][i].start< (UINT)frame && mAnimationColActiveFrame[clip][i].fin > (UINT)frame)
                            active = true;

                    }
                    if (active)
                    {
                        //콜라이더 활성화된 구간
                        col->Active(true);

                        //본데이터 받아서 오브젝트 위치/회전 변경
                        Transform* tr = GetComponent<Transform>();
                        Vector3 pos = mBoneMat.keyFrames[clip][frame].translate;
                        tr->SetLocalPosition(pos);
                        Quaternion q = mBoneMat.keyFrames[clip][frame].rotation;
                        Vector3 rot = q.ToEuler();
                        tr->SetLocalRotation(rot * 180 / XM_PI);
                    }
                    else
                    {
                        if (mHitObjects.size() > 0)
                            mHitObjects.clear();
                        col->Active(false);
                    }

                }
            }
        }
        mPrevPos = mCulPos;
        GameObject::Update();
    }
    void BoneCollider::FixedUpdate()
    {
        GameObject::FixedUpdate();
    }
    void BoneCollider::Render()
    {
        GameObject::Render();
    }

    void BoneCollider::SetMeshAndBone(std::shared_ptr<MeshData> meshData, std::wstring bone, GameObject* owner)
    {
        mOwner = owner;
        mMeshData = meshData;
        MeshObject* meshObject = mMeshData->GetMeshObject();
        bool noBoneNameAssert = true;

        std::vector<BoneMatrix>* bones = mMeshData->GetBones();
        for (size_t i = 0; i < bones->size(); i++)
        {
            if (bones->at(i).name == bone)
            {
                mBoneMat = bones->at(i);
                noBoneNameAssert = false;
            }
        }

        if (noBoneNameAssert)
            assert(NULL);

        AddComponent<Collider3D>()->SetType(eColliderType::Box);
        SetName(L"Weapon");
        Transform* tr = GetComponent<Transform>();
        Transform* meshTr = meshObject->GetComponent<Transform>();
        tr->SetLocalScale(Vector3(3.f, 0.3f, 0.3f));
        tr->SetParent(meshTr);

        mAnimationOffsets.resize(mMeshData->GetAnimationClipCount());
        mAnimationColActiveFrame.resize(mMeshData->GetAnimationClipCount());
    }
    void BoneCollider::SetScale(Vector3 scale)
    {
        Transform* tr = GetComponent<Transform>();
        tr->SetLocalScale(scale);
    }
    void BoneCollider::SetAnimOffSet(UINT animIdx, Vector3 offset)
    {
        mAnimationOffsets[animIdx] = offset;
    }
    void BoneCollider::SetAnimOffSet(const std::wstring& animName, Vector3 offset)
    {
        BoneAnimator* animator =  mMeshData->GetAnimator();
        int idx = animator->GetAnimationIdxByName(animName);
        if (idx == -1)
            assert(NULL);
        mAnimationOffsets[idx] = offset;
    }
    void BoneCollider::SetColliderActiveFrame(UINT animIdx, UINT start, UINT finish)
    {
        mAnimationColActiveFrame[animIdx].push_back({ start,finish });

    }
    void BoneCollider::SetColliderActiveFrame(const std::wstring& animName, UINT start, UINT finish)
    {
        BoneAnimator* animator = mMeshData->GetAnimator();
        int idx = animator->GetAnimationIdxByName(animName);
        if (idx == -1)
            assert(NULL);
        mAnimationColActiveFrame[idx].push_back({start,finish});
    }
    bool BoneCollider::CheckHitObjects(GameObject* obj)
    {
        for (size_t i = 0; i < mHitObjects.size(); i++)
        {
            if (obj == mHitObjects[i])
                return true;
        }
        return false;
    }
}