#pragma once
#include "yaScript.h"
#include "yaMeshData.h"

namespace ya
{
	class PlayerMeshScript : public Script
	{
	public:
		PlayerMeshScript();
		virtual ~PlayerMeshScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(GameObject* _otherObj , const Vector3& _hitPoint) override;
		virtual void OnCollisionStay( GameObject* _otherObj , const Vector3& _hitPoint) override;
		virtual void OnCollisionExit( GameObject* _otherObj , const Vector3& _hitPoint) override;

	public:
		void LoadAnimation();
		void Play(std::wstring name);

		std::shared_ptr<MeshData> FindMeshData(std::wstring name);

	private:
		bool mbUse;
		bool mbLoad;

		std::map<std::wstring, std::shared_ptr<MeshData>> mMeshDataMap;
	};
}
