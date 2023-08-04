#pragma once
#include "yaScript.h"

namespace ya
{
	class MoveScript : public Script
	{
		enum class eDirection
		{
			Forward,
			Back,
			Right,
			Left,
		};

	public:
		MoveScript();
		~MoveScript();

		virtual void Initalize() override;
		virtual void Update() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(Collider2D* collider) override;
		virtual void OnCollisionStay(Collider2D* collider) override;
		virtual void OnCollisionExit(Collider2D* collider) override;

	private:
		bool mbRotate;			// �����Ϸ��� ����� �÷��̾��� forward�� �����ֱ� ���� ȸ�� ������ ����
		eDirection mLastDir;	// �������� �Է¹��� ����
	};
}
