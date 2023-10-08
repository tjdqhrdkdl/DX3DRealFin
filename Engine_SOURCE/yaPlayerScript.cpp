#include "yaPlayerScript.h"
#include "yaPlayer.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaInput.h"
#include "yaTime.h"
#include "yaAnimator.h"
#include "yaObject.h";
#include "yaMeshRenderer.h"
#include "yaResources.h"
#include "yaRigidbody.h"
#include "yaActionScript.h"
#include "yaGrappleHookScript.h"
#include "yaMusketeerman.h"
#include "yaSpearman.h"
#include "yaSwordsman.h"


#include "yaSceneManager.h"
#include "yaScene.h"

#include "yaPlayerMeshScript.h"
#include "yaState.h"


namespace ya
{
	PlayerScript::PlayerScript()
		: Script()
		, mMoveTimer(0.0f)
		, mMoveTimerMax(0.15f)
		, mHitDirection(Vector3::Zero)
	{
	}

	PlayerScript::~PlayerScript()
	{
	}

	void PlayerScript::Initialize()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

		mPlayer = dynamic_cast<Player*>(GetOwner());
		mPlayerAnim = mPlayer->GetScript<PlayerMeshScript>();
	}

	void PlayerScript::Update()
	{
		if (Input::GetKey(eKeyCode::F))
		{
			GrappleHookScript* grap = GetOwner()->GetScript<GrappleHookScript>();
			if(grap != nullptr)
			{
				grap->GrappleHook();
			}
		}
		ActionScript* action = GetOwner()->GetScript<ActionScript>();
		Transform* tr = GetOwner()->GetComponent < Transform>();

		if (mMoveTimer > 0.0f)
		{
			mMoveTimer -= Time::DeltaTime();
			action->Move(mHitDirection, 200.0f);
		}
	}

	void PlayerScript::Render()
	{
	}

	void PlayerScript::OnCollisionEnter(Collider2D* collider)
	{
		GameObject* obj = collider->GetOwner();

		BoneCollider* boneCollider = dynamic_cast<BoneCollider*>(obj);
		if(boneCollider != nullptr)
		{
			if (mPlayer->IsStateFlag(ePlayerState::Block))
			{
				mPlayer->SetStateFlag(ePlayerState::Block, false);

				mPlayer->GetState()->AddPosture(-10);

				Transform* playerTr = mPlayer->GetComponent<Transform>();
				Vector3 playerPos = playerTr->GetPosition();
				Vector3 playerDir = playerTr->Forward();

				Vector3 colliderPos = collider->GetPosition();

				// 플레이어의 방향과 collider간의 각도를 구한다.
				Quaternion quater = Quaternion::FromToRotation(playerDir, Vector3(colliderPos.x - playerPos.x, playerPos.y, colliderPos.z - playerPos.z));
				Vector3 quaterToEuler = quater.ToEuler();
				Vector3 theta = quaterToEuler * 180.0f / XM_PI;

				// 충돌 각도에 따라 피격 방향(애니메이션) 달라짐
				if (theta.y > -45.0f && theta.y <= 45.0f)
					mPlayerAnim->Play(L"a050_120101");
				else if (theta.y > 45.0f && theta.y <= 135.0f)
					mPlayerAnim->Play(L"a050_120100");
				else if (theta.y > 135.0f && theta.y <= 180.0f)
					mPlayerAnim->Play(L"a050_120103");
				else if (theta.y > -180.0f && theta.y <= -135.0f)
					mPlayerAnim->Play(L"a050_120103");
				else if (theta.y > -135.0f && theta.y <= -45.0f)
					mPlayerAnim->Play(L"a050_120102");
			}
			else
			{
				mPlayer->GetState()->AddHp(-10);
				mPlayer->SetStateFlag(ePlayerState::Hit, true); // 경직상태

				Transform* playerTr = mPlayer->GetComponent<Transform>();
				Vector3 playerPos = playerTr->GetPosition();
				Vector3 playerDir = playerTr->Forward();

				Vector3 colliderPos = collider->GetPosition();

				// 플레이어의 방향과 collider간의 각도를 구한다.
				Quaternion quater = Quaternion::FromToRotation(playerDir, Vector3(colliderPos.x - playerPos.x, playerPos.y, colliderPos.z - playerPos.z));
				Vector3 quaterToEuler = quater.ToEuler();
				Vector3 theta = quaterToEuler * 180.0f / XM_PI;

				// 충돌 각도에 따라 피격 방향(애니메이션) 달라짐
				if (theta.y > -45.0f && theta.y <= 45.0f)
					mPlayerAnim->Play(L"a000_100102");
				else if (theta.y > 45.0f && theta.y <= 135.0f)
					mPlayerAnim->Play(L"a000_100100");
				else if (theta.y > 135.0f && theta.y <= 180.0f)
					mPlayerAnim->Play(L"a000_100103");
				else if (theta.y > -180.0f && theta.y <= -135.0f)
					mPlayerAnim->Play(L"a000_100103");
				else if (theta.y > -135.0f && theta.y <= -45.0f)
					mPlayerAnim->Play(L"a000_100101");

				if (mMoveTimer <= 0.0f)
				{
					mMoveTimer = mMoveTimerMax;
					mHitDirection = -playerTr->Forward();
				}
			}
		}

	}

	void PlayerScript::OnCollisionStay(Collider2D* collider)
	{
	}

	void PlayerScript::OnCollisionExit(Collider2D* collider)
	{
	}

}
