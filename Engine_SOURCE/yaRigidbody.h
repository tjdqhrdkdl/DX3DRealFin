#pragma once
#include "yaComponent.h"

using namespace ya::math;
namespace ya
{	/// 물리
	class Rigidbody : public Component
	{
	public:
		Rigidbody();
		virtual ~Rigidbody();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	public:
		void AddForce(Vector3 force);
		void ClearForce();
		void SetMass(float mass) { mMass = mass; }
		void SetGround(bool isGround) { mbGround = isGround; }
		bool IsGround() { return mbGround; }

		void SetVelocity(Vector3 velocity) { mVelocity = velocity; }
		Vector3 GetVelocity() { return mVelocity; }

		Vector3 GetLimitVelocity() { return mLimitVelocity; }
		void SetLimitVelocity(Vector3 limit) { mLimitVelocity = limit; }

	private:
		// 힘과 마찰력을 이용한 이동
		float mMass;		/// 무게
		Vector3 mForce;		/// 물체는 미는 힘
		Vector3 mVelocity;	/// 방향을 갖는 속도
		Vector3 mAccelation;/// 가속도
		float mFriction;	/// 마찰력

		// 중력을 이용한 점프
		Vector3 mGravity;	/// 중력 크기
		bool mbGround;		/// 지면에 붙어있는지
		Vector3 mLimitVelocity;	/// 떨어지는 속도 한계값
	};
}