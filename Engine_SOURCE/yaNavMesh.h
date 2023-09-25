#pragma once
#include "yaComponent.h"

namespace ya
{
	using namespace math;
	class NavMesh : public Component
	{
	public:
		NavMesh();
		~NavMesh();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetDestination(Vector3 pos) { mDestPos = pos; }
		void SetNavigate(bool on) { mbNavOn = on; }
		void RenewPath(Vector3 dest) { mDestPos = dest; mbNavOn = true; }

		void SetSpeed(float speed) { mSpeed = speed; }
		void SetTraceOn(bool trace) { mbTraceOn = trace; }
	private:


		bool mbNavOn;
		Vector3 mDestPos;
		std::vector<Vector3> mPath;

		bool mbTraceOn;
		int mCurPathIdx;
		float mSpeed;
	};

}