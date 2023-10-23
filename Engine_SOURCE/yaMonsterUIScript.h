#pragma once
#include "yaScript.h"




namespace ya
{
	class MonsterUIScript :
		public Script
	{

	public:
		
		virtual void OnCollisionEnter(Collider2D* collider);
		void SetMonsterUI(class MonsterUI* obj) { mMonsterUI = obj; }

	private:
		class MonsterUI* mMonsterUI;
	};

}