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

		virtual void OnCollisionEnter(Collider2D* collider) override;
		virtual void OnCollisionStay(Collider2D* collider) override;
		virtual void OnCollisionExit(Collider2D* collider) override;

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
