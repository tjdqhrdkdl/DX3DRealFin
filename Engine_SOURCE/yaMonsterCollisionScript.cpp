#include "yaMonsterCollisionScript.h"
#include "yaMonsterBase.h"
#include "yaGameObject.h"

namespace ya
{
	MonsterCollisionScript::MonsterCollisionScript()
	{
	}
	MonsterCollisionScript::~MonsterCollisionScript()
	{
	}
	void MonsterCollisionScript::Initialize()
	{
		mMonster = dynamic_cast<MonsterBase*>(GetOwner());
	}
	void MonsterCollisionScript::OnCollisionEnter(Collider2D* collider)
	{
		mMonster->OnCollisionEnter(collider);
	}
	void MonsterCollisionScript::OnCollisionStay(Collider2D* collider)
	{
		mMonster->OnCollisionStay(collider);

	}
	void MonsterCollisionScript::OnCollisionExit(Collider2D* collider)
	{
		mMonster->OnCollisionExit(collider);

	}
}