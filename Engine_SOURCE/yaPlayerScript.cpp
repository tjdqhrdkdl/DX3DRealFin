#include "yaPlayerScript.h"
#include "yaPlayer.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaInput.h"
#include "yaTime.h"
#include "yaAnimator.h"


namespace ya
{
	PlayerScript::PlayerScript()
		: Script()
		, mJumpTimer(0.0f)
	{
	}

	PlayerScript::~PlayerScript()
	{
	}

	void PlayerScript::Initalize()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		{
			// 플레이어의 forward를 구분하기위한 object
			// 후에 mesh 씌우면 없앨 예정
			GameObject* face = object::Instantiate<GameObject>(eLayerType::Player, tr);
			face->SetName(L"face");
			Transform* faceTr = face->GetComponent<Transform>();
			faceTr->SetPosition(Vector3(0.0f, 0.5f, 0.5f));
			faceTr->SetScale(Vector3(0.4f, 0.4f, 0.4f));
			MeshRenderer* faceRenderer = face->AddComponent<MeshRenderer>();
			faceRenderer->SetMaterial(Resources::Find<Material>(L"BasicMaterial"));
			faceRenderer->SetMesh(Resources::Find<Mesh>(L"CubeMesh"));
		}

	}

	void PlayerScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();



		// camera script wasd 미사용시 키 변경
		if (Input::GetKey(eKeyCode::L))
		{

		}
		if (Input::GetKey(eKeyCode::J))
		{

		}
		if (Input::GetKey(eKeyCode::K))
		{

	}

	void PlayerScript::Render()
	{
	}

	void PlayerScript::OnCollisionEnter(Collider2D* collider)
	{

	}

	void PlayerScript::OnCollisionStay(Collider2D* collider)
	{
	}

	void PlayerScript::OnCollisionExit(Collider2D* collider)
	{
	}

	void PlayerScript::Start()
	{
	}

	void PlayerScript::Action()
	{
	}

	void PlayerScript::End()
	{
	}

}
