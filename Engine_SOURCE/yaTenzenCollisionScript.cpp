#include "yaTenzenCollisionScript.h"
#include "yaTenzen.h"
#include "yaGameObject.h"

namespace ya
{
	TenzenCollisionScript::TenzenCollisionScript()
	{
	}
	TenzenCollisionScript::~TenzenCollisionScript()
	{
	}
	void TenzenCollisionScript::Initialize()
	{
		mTenzen = dynamic_cast<Tenzen*>(GetOwner());
	}
	void TenzenCollisionScript::OnCollisionEnter(Collider3D* collider)
	{
		mTenzen->OnCollisionEnter(collider);
	}
	void TenzenCollisionScript::OnCollisionStay(Collider3D* collider)
	{
		mTenzen->OnCollisionStay(collider);

	}
	void TenzenCollisionScript::OnCollisionExit(Collider3D* collider)
	{
		mTenzen->OnCollisionExit(collider);

	}
}