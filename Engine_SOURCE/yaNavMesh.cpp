#include "yaNavMesh.h"
#include "yaNavMeshTool.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaActionScript.h"
#include "yaInput.h"
#include "yaRigidbody.h"
#include "yaTime.h"
namespace ya
{
	NavMesh::NavMesh()
		: Component(eComponentType::NavMesh)
		, ComponentType(eComponentType::NavMesh)
	{
	}
	NavMesh::~NavMesh()
	{
	}
	void NavMesh::Initialize()
	{
		mbNavOn = false;
		mDestPos = Vector3((float)100.532, (float)-23.113, (float)-3.576);

		mbTraceOn = true;
		mSpeed = 80;
		mCurPathIdx = 0;
	}
	void NavMesh::Update()
	{

		if (mbNavOn)
		{
			NavMeshTool* tool = NavMeshTool::GetInst();
			Transform* mapTr = tool->GetMapMeshTr();

			//툴에 시작점과 도착점 전달하기
			//우리 엔진의 포지션을 네비 메쉬의 포지션값으로 변경시켜서 보내줘야함.
			//시작점은 나의 위치
			Transform* tr = GetOwner()->GetComponent<Transform>();
			Vector3 posV3 = tr->GetPosition();
			Vector4 posV4 = Vector4(posV3.x, posV3.y, posV3.z, 1.0f);
			Matrix mapWorldMat = mapTr->GetWorldMatrix();
			Matrix inverseMat = mapWorldMat.Invert();
			posV4 = Vector4::Transform(posV4, inverseMat);
			float pos[3];
			pos[0] = posV4.x;
			pos[1] = posV4.z;
			pos[2] = posV4.y;
			tool->SetStartPos(pos);

			//도착점은 외부에서 정해줌.

			posV4 = Vector4(mDestPos.x, mDestPos.y, mDestPos.z, 1.0f);
			posV4 = Vector4::Transform(posV4, inverseMat);
			pos[0] = posV4.x;
			pos[1] = posV4.z;
			pos[2] = posV4.y;
			tool->SetEndPos(pos);

			//툴에서 포지션을 받아서 경로를 계산해준다.
			//
			tool->recalc();
			float* fPath = tool->GetPath();
			mPath.clear();
			mCurPathIdx = 0;
			for (size_t i = 0; i < tool->GetPathSize(); i++)
			{
				Vector4 pathV4 = Vector4(fPath[i * 3], fPath[i * 3 + 2], fPath[i * 3 + 1],1);
				pathV4 = Vector4::Transform(pathV4, mapWorldMat);
				Vector3 path = Vector3(pathV4.x, pathV4.y, pathV4.z);
				mPath.push_back(path);
			}
			mbNavOn = false;
		}
		if (mbTraceOn && mCurPathIdx < mPath.size() - 1 && mPath.size() > 0)
		{

			Vector3 dir = mPath[mCurPathIdx + 1] - mPath[mCurPathIdx];
			Transform* tr = GetOwner()->GetComponent<Transform>();
			Vector3 diff = tr->GetPosition() - mPath[mCurPathIdx];
			if (fabsf(diff.x) > fabsf(dir.x))
			{
				mCurPathIdx++;
			}
				dir.y = 0;
				dir.Normalize();

				ActionScript* action = GetOwner()->GetScript<ActionScript>();
				action->Move(dir, mSpeed);
				mDir = dir;
		}
	}
	void NavMesh::FixedUpdate()
	{
	}
	void NavMesh::Render()
	{
	}
	void NavMesh::RenewPath(Vector3 dest)
	{

		if (mRenewTimer > 0.2f)
		{
			mDestPos = dest;
			mbNavOn = true;
			mbTraceOn = true;
			mRenewTimer = 0;
		}
		 else
			 mRenewTimer += Time::DeltaTime();
	}
}