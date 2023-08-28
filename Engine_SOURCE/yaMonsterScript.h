#pragma once
#include "yaScript.h"
#include "yaPlayerScript.h"
#include "yaPlayer.h"
#include "yaSpearman.h"
#include "yaMonsterBase.h"




namespace ya
{
	class MonsterScript :
		public Script
	{

	public:

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		//virtual void Render() override;

		virtual void OnCollisionEnter(Collider2D* collider);
		virtual void OnCollisionStay(Collider2D* collider);
		virtual void OnCollisionExit(Collider2D* collider);


	public:


	private:


	};

}