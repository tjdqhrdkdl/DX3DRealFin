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
	void TenzenCollisionScript::OnCollisionEnter(Collider2D* collider)
	{
		mTenzen->OnCollisionEnter(collider);
	}
	void TenzenCollisionScript::OnCollisionStay(Collider2D* collider)
	{
		mTenzen->OnCollisionStay(collider);

	}
	void TenzenCollisionScript::OnCollisionExit(Collider2D* collider)
	{
		mTenzen->OnCollisionExit(collider);

	}
}