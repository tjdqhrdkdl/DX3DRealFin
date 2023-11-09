#include "yaCamera.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaApplication.h"
#include "yaRenderer.h"
#include "yaScene.h"
#include "yaResources.h"
#include "yaSceneManager.h"
#include "yaMaterial.h"
#include "yaBaseRenderer.h"
#include "yaSceneManager.h"
#include "yaFrustum.h"
#include "yaBoundarySphere.h"
#include "yaMeshObject.h"

extern ya::Application application;

namespace ya
{
	Matrix Camera::View = Matrix::Identity;
	Matrix Camera::InverseView = Matrix::Identity;
	Matrix Camera::Projection = Matrix::Identity;

	Camera::Camera()
		: Component(eComponentType::Camera)
		, mType(eProjectionType::Orthographic)
		, mAspectRatio(1.0f)
		, mNear(1.0f)
		, mFar(1000.0f)
		, mScale(1.0f)
	{
		EnableLayerMasks();
	}

	Camera::~Camera()
	{
	}

	void Camera::Initialize()
	{
		RegisterCameraInRenderer();
	}

	void Camera::Update()
	{

	}

	void Camera::FixedUpdate()
	{
		CreateViewMatrix();
		CreateProjectionMatrix();
		//절두체 구성
		mFrustum.ConstructFrustum(55.0f,mProjection,mView);

		RegisterCameraInRenderer();
	}

	void Camera::Render()
	{
		sortGameObjects();

		// View proj  행렬 direction light 껄로 바뀌어야한다.
		if (false == renderer::lights.empty())
		{
			Transform* tr = renderer::lights[0]->GetOwner()->GetComponent<Transform>();
			View = CreateViewMatrix(tr);
			Projection = CreateProjectionMatrix(eProjectionType::Perspective, 1600, 900, 1.0f, 1000.0f);

			ConstantBuffer* lightCB = renderer::constantBuffers[(UINT)eCBType::LightMatrix];

			LightMatrixCB data = {};
			data.lightView = View;
			data.lightProjection = Projection;
			lightCB->SetData(&data);
			lightCB->Bind(eShaderStage::VS);
			lightCB->Bind(eShaderStage::PS);
		}



		// shadow
		renderTargets[(UINT)eRTType::Shadow]->OmSetRenderTarget();
		renderShadow();

		View = mView;
		InverseView = View.Invert();
		Projection = mProjection;


	
		// deffered opaque render
		renderTargets[(UINT)eRTType::Deferred]->OmSetRenderTarget();
		renderDeferred();


		//// deffered light 
		renderTargets[(UINT)eRTType::Light]->OmSetRenderTarget();
		// 여러개의 모든 빛을 미리 한장의 텍스처에다가 계산을 해두고
		// 붙여버리자

		for (Light* light : renderer::lights)
		{
			light->Render();
		}


		// swapchain 
		renderTargets[(UINT)eRTType::Swapchain]->OmSetRenderTarget();

		////// defferd + swapchain merge
		std::shared_ptr<Material> mergeMaterial = Resources::Find<Material>(L"MergeMaterial");
		std::shared_ptr<Mesh> rectMesh = Resources::Find<Mesh>(L"RectMesh");

		rectMesh->BindBuffer(0);
		mergeMaterial->Bind();
		rectMesh->Render(0);

		// Foward render
		renderOpaque();
		renderCutout();
		renderTransparent();
		renderPostProcess();
	}

	void Camera::CreateViewMatrix()
	{
		//Transform* tr = GetOwner()->GetComponent<Transform>();
		//Vector3 pos = tr->GetPosition();

		//// Crate Translate view matrix
		//mView = Matrix::Identity;
		//mView *= Matrix::CreateTranslation(-pos);
		////회전 정보

		//Vector3 up = tr->Up();
		//Vector3 right = tr->Right();
		//Vector3 foward = tr->Forward();

		//Matrix viewRotate;
		//viewRotate._11 = right.x; viewRotate._12 = up.x; viewRotate._13 = foward.x;
		//viewRotate._21 = right.y; viewRotate._22 = up.y; viewRotate._23 = foward.y;
		//viewRotate._31 = right.z; viewRotate._32 = up.z; viewRotate._33 = foward.z;

		//mView *= viewRotate;

		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

		mView = Matrix::Identity;
		mView = Matrix::CreateLookAtLH(pos, pos + tr->Forward(), tr->Up());

	}

	Matrix Camera::CreateViewMatrix(Transform* tr)
	{
		Matrix view = Matrix::Identity;

		Vector3 pos = tr->GetPosition();

		view = Matrix::CreateLookAtLH(pos, pos + tr->Forward(), tr->Up());

		return view;
	}

	void Camera::CreateProjectionMatrix()
	{
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);

		float width = (winRect.right - winRect.left) * mScale;
		float height = (winRect.bottom - winRect.top) * mScale;
		mAspectRatio = width / height;

		if (mType == eProjectionType::Perspective)
		{
			mProjection = Matrix::CreatePerspectiveFieldOfViewLH
			(
				XM_2PI / 6.0f
				, mAspectRatio
				, mNear
				, mFar
			);
		}
		else
		{
			mProjection = Matrix::CreateOrthographicLH(width /*/ 100.0f*/, height /*/ 100.0f*/, mNear, mFar);
		}
	}

	Matrix Camera::CreateProjectionMatrix(eProjectionType type, float width, float height, float Near, float Far)
	{
		Matrix proj = Matrix::Identity;

		float AspectRatio = width / height;
		if (mType == eProjectionType::Perspective)
		{
			proj = Matrix::CreatePerspectiveFieldOfViewLH
			(
				XM_2PI / 6.0f
				, AspectRatio
				, Near
				, Far
			);
		}
		else
		{
			proj = Matrix::CreateOrthographicLH(width /*/ 100.0f*/, height /*/ 100.0f*/, Near, Far);
		}

		return proj;
	}

	void Camera::RegisterCameraInRenderer()
	{
		//eSceneType type = SceneManager::GetActiveScene()->GetSceneType();
		eSceneType type = GetOwner()->GetScene()->GetSceneType();
		renderer::cameras[(UINT)type].push_back(this);
	}

	void Camera::TurnLayerMask(eLayerType layer, bool enable)
	{
		mLayerMasks.set((UINT)layer, enable);
	}

	void Camera::sortGameObjects()
	{
		mDeferredOpaqueGameObjects.clear();
		mOpaqueGameObjects.clear();
		mCutoutGameObjects.clear();
		mTransparentGameObjects.clear();
		mPostProcessGameObjects.clear();

		//Scene* scene = SceneManager::GetActiveScene();
		Scene* scene = GetOwner()->GetScene();
		for (size_t i = 0; i < (UINT)eLayerType::End; i++)
		{
			if (mLayerMasks[i] == true)
			{
				Layer& layer = scene->GetLayer((eLayerType)i);
				GameObjects gameObjects = layer.GetGameObjects();
				if (gameObjects.size() == 0)
					continue;

				for (GameObject* obj : gameObjects)
				{
					BoundarySphere* sphere = obj->GetComponent<BoundarySphere>();
					if (sphere)
					{
						MeshObject* meshObject = dynamic_cast<MeshObject*>(obj);
						if (nullptr != meshObject)
							meshObject->SetChildRender(true);
						Vector3 center = sphere->GetPosition();
						float ra = sphere->GetRadius();
						if (mFrustum.CheckSphere(center.x, center.y, center.z, sphere->GetRadius()))
						{
							pushGameObjectToRenderingModes(obj);
						}
						else
						{
							if (nullptr != meshObject)
								meshObject->SetChildRender(false);
							continue;
						}
					}

					pushGameObjectToRenderingModes(obj);
				}
			}
		}
	}

	void Camera::renderShadow()
	{
		//for (GameObject* obj : mDeferredOpaqueGameObjects)
		//{
		//	if (obj == nullptr)
		//		continue;

		//	obj->PrevRender();
		//}

		//for (GameObject* obj : mOpaqueGameObjects)
		//{
		//	if (obj == nullptr)
		//		continue;

		//	obj->PrevRender();
		//}

		//for (GameObject* obj : mTransparentGameObjects)
		//{
		//	if (obj == nullptr)
		//		continue;

		//	obj->PrevRender();
		//}

		//for (GameObject* obj : mCutoutGameObjects)
		//{
		//	if (obj == nullptr)
		//		continue;

		//	obj->PrevRender();
		//}



	}

	void Camera::renderDeferred()
	{
		for (GameObject* obj : mDeferredOpaqueGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}

	void Camera::renderOpaque()
	{
		for (GameObject* obj : mOpaqueGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}

	void Camera::renderCutout()
	{
		for (GameObject* obj : mCutoutGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}

	void Camera::renderTransparent()
	{
		for (GameObject* obj : mTransparentGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}

	void Camera::renderPostProcess()
	{

		for (GameObject* obj : mPostProcessGameObjects)
		{
			if (obj == nullptr)
				continue;
			BoundarySphere* sphere = obj->GetComponent<BoundarySphere>();
			if (sphere)
			{
				Vector3 center = sphere->GetCenter();
				if (mFrustum.CheckSphere(center.x, center.y, center.z, sphere->GetRadius()))
					continue;
			}
			renderer::CopyRenderTarget();
			obj->Render();
		}
	}

	void Camera::pushGameObjectToRenderingModes(GameObject* gameObj)
	{
		BaseRenderer* renderer
			= gameObj->GetRenderer<BaseRenderer>();
		if (renderer == nullptr)
			return;

		std::shared_ptr<Material> material = renderer->GetMaterial(0);


		eRenderingMode mode = material->GetRenderingMode();

		switch (mode)
		{
		case ya::graphics::eRenderingMode::DeferredOpaque:
		case ya::graphics::eRenderingMode::DeferredMask:
			mDeferredOpaqueGameObjects.push_back(gameObj);
			break;
		case ya::graphics::eRenderingMode::Opaque:
			mOpaqueGameObjects.push_back(gameObj);
			break;
		case ya::graphics::eRenderingMode::CutOut:
			mCutoutGameObjects.push_back(gameObj);
			break;
		case ya::graphics::eRenderingMode::Transparent:
			mTransparentGameObjects.push_back(gameObj);
			break;
		case ya::graphics::eRenderingMode::PostProcess:
			mPostProcessGameObjects.push_back(gameObj);
			break;
		default:
			break;
		}
	}
}
