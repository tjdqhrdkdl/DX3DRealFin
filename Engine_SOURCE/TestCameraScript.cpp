#include "TestCameraScript.h"
#include "yaGameObject.h"
#include "yaInput.h"
#include "yaTime.h"

namespace ya
{
	TestCameraScript::TestCameraScript()
	{
	}
	TestCameraScript::~TestCameraScript()
	{
	}
	void TestCameraScript::Update()
	{
		Transform* tf = GetOwner()->GetComponent<Transform>();

		Vector3 vPos = tf->GetLocalPosition();
		Vector3 vRot = tf->GetLocalRotation();

		Vector3 vFront = tf->Forward();
		Vector3 vUp = tf->Up();
		Vector3 vRight = tf->Right();

		float fSpeed = 500.f;

		//if (Input::GetKey(eKeyCode::LSHIFT))
		//	fSpeed *= 5.f;

		if (Input::GetKey(eKeyCode::W))
		{
			vPos += Time::DeltaTime() * vFront * fSpeed;
		}

		if (Input::GetKey(eKeyCode::S))
		{
			vPos -= Time::DeltaTime() * vFront * fSpeed;
		}

		if (Input::GetKey(eKeyCode::A))
		{
			vPos -= Time::DeltaTime() * vRight * fSpeed;
		}

		if (Input::GetKey(eKeyCode::D))
		{
			vPos += Time::DeltaTime() * vRight * fSpeed;
		}



		if (Input::GetKey(eKeyCode::RBTN))
		{
			Vector2 vMouseDir = Input::GetMouseDir();
			vRot.y += Time::DeltaTime() * vMouseDir.x;
			vRot.x -= Time::DeltaTime() * vMouseDir.y;
		}

		tf->SetLocalPosition(vPos);
		tf->SetLocalRotation(vRot);

	}
}