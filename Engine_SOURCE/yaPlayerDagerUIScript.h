#pragma once
#include "yaScript.h"
#include "yaRenderer.h"
namespace ya
{
	class PlayerDangerUIScript :public Script
	{
	public:
		PlayerDangerUIScript();
		~PlayerDangerUIScript();
		void Render() override;

		void SetCB(renderer::TimeCB cb) { mCB.time = cb.time; mCB.isMain = cb.isMain;}
	private:
		renderer::TimeCB mCB;

	};
}

