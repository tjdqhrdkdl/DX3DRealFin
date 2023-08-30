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
        GameObject::Initialize();
    }
    void BoneCollider::Update()
    {

        Collider2D* col = GetComponent<Collider2D>();
        if (col)
        {
            BoneAnimator* animator = mMeshData->GetAnimator();
            if (animator)
            {
                int clip = animator->GetCurrentClipIdx();
                if (mBeforeClipIdx != clip)
                {
                    //콜라이더 센터 변경
                    col->SetCenter(mAnimationOffsets[clip]);
                    mBeforeClipIdx = clip;
                }
                int frame = animator->GetCurrentFrameIdx();
                if (clip >= 0 && frame >= 0)
                {
                    if (mAnimationColActiveFrame[clip].start < frame && mAnimationColActiveFrame[clip].fin > frame)
                    {
                        //콜라이더 활성화된 구간
                        col->Active(true);

                        //본데이터 받아서 오브젝트 위치/회전 변경
                        Transform* tr = GetComponent<Transform>();
                        Vector3 pos = mBoneMat.keyFrames[clip][frame].translate;
                        pos.y = -pos.y;
                        pos.x = -pos.x;
                        tr->SetPosition(pos);
                        Quaternion q = mBoneMat.keyFrames[clip][frame].rotation;
                        Vector3 rot = q.ToEuler();
                        float temp = rot.y;
                        rot.y = -rot.x;
                        rot.x = -temp;
                        tr->SetRotation(rot * 180 / XM_PI);
                    }
                    else
                        col->Active(false);

                }
            }
        }
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

    void BoneCollider::SetMeshAndBone(MeshData* meshData, std::wstring bone)
    {
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

        AddComponent<Collider2D>()->SetType(eColliderType::Box);
        SetName(L"Weapon");
        Transform* tr = GetComponent<Transform>();
        Transform* meshTr = meshObject->GetComponent<Transform>();
        tr->SetScale(Vector3(3, 0.3, 0.3));
        tr->SetParent(meshTr);

        mAnimationOffsets.resize(mMeshData->GetAnimationClipCount());
        mAnimationColActiveFrame.resize(mMeshData->GetAnimationClipCount());
    }
    void BoneCollider::SetScale(Vector3 scale)
    {
        Transform* tr = GetComponent<Transform>();
        tr->SetScale(scale);
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
        mAnimationColActiveFrame[animIdx] = { start,finish };

    }
    void BoneCollider::SetColliderActiveFrame(const std::wstring& animName, UINT start, UINT finish)
    {
        BoneAnimator* animator = mMeshData->GetAnimator();
        int idx = animator->GetAnimationIdxByName(animName);
        if (idx == -1)
            assert(NULL);
        mAnimationColActiveFrame[idx] = {start,finish};
    }
}