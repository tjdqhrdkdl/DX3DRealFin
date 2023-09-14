#pragma once
#include "yaComponent.h"
#include "yaFrustum.h"
namespace ya
{
	using namespace math;
	class Camera : public Component
	{
	public:
		enum eProjectionType
		{
			Perspective,
			Orthographic,
		};

		__forceinline static Matrix& GetGpuViewMatrix() { return View; }
		__forceinline static Matrix& GetGpuProjectionMatrix() { return Projection; }
		__forceinline static void SetGpuViewMatrix(Matrix view) { View = view; }
		__forceinline static void SetGpuProjectionMatrix(Matrix projection) { Projection = projection; }

		Camera();
		virtual ~Camera();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;


		void CreateViewMatrix();
		Matrix CreateViewMatrix(class Transform* tr);
		void CreateProjectionMatrix();
		Matrix CreateProjectionMatrix(eProjectionType type, float width, float height, float Near, float Far);
		void RegisterCameraInRenderer();

		void TurnLayerMask(eLayerType layer, bool enable = true);
		void EnableLayerMasks() { mLayerMasks.set(); }
		void DisableLayerMasks() { mLayerMasks.reset(); }

		void SetProjectionType(eProjectionType type) { mType = type; }
		float GetScale() { return mScale; }
		Matrix& GetViewMatrix() { return mView; }
		Matrix& GetProjectionMatrix() { return mProjection; }

	private:
		void sortGameObjects();
		void renderShadow();
		void renderDeferred();
		void renderOpaque();
		void renderCutout();
		void renderTransparent();
		void renderPostProcess();
		void pushGameObjectToRenderingModes(GameObject* gameObj);

	private:
		static Matrix View;
		static Matrix Projection;
		static Matrix InverseView;

		Matrix mView;
		Matrix mProjection;

		eProjectionType mType;
		float mAspectRatio;

		float mNear;
		float mFar;
		float mScale;

		std::bitset<(UINT)eLayerType::End> mLayerMasks;
		std::vector<GameObject*> mDeferredOpaqueGameObjects;
		std::vector<GameObject*> mOpaqueGameObjects;
		std::vector<GameObject*> mCutoutGameObjects;
		std::vector<GameObject*> mTransparentGameObjects;
		std::vector<GameObject*> mPostProcessGameObjects;

		Frustum mFrustum;
	};
}
