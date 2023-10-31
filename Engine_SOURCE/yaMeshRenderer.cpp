#include "yaMeshRenderer.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaAnimator.h"
#include "yaBoneAnimator.h"
#include "yaMaterial.h"
#include "yaResources.h"

namespace ya
{
	MeshRenderer::MeshRenderer()
		: BaseRenderer(eComponentType::MeshRenderer)
		, ComponentType(eComponentType::MeshRenderer)
	{
	}

	MeshRenderer::~MeshRenderer()
	{
	}

	void MeshRenderer::Initialize()
	{
	}

	void MeshRenderer::Update()
	{
	}

	void MeshRenderer::FixedUpdate()
	{
	}

	void MeshRenderer::PrevRender()
	{

		std::shared_ptr<Material> material
			= Resources::Find<Material>(L"ShadowMapMaterial");

		//material->Bind();

		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();
		BoneAnimator* animator
			= GetOwner()->GetComponent<BoneAnimator>();

		UINT subSetCount = GetMesh()->GetSubSetCount();
		for (UINT i = 0; i < subSetCount; i++)
		{
			if (animator)
			{
				animator->Binds();
				GetMaterial((int)i)->SetAnimation(true);

			}

			GetMesh()->BindBuffer(i);
			GetMaterial((int)i)->Bind();
			GetMesh()->Render(i);

			GetMaterial((int)i)->Clear();
		}

		if (animator)
		{
			animator->ClearData();
		}
	
	}

	void MeshRenderer::Render()
	{
		//GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

		//GetMaterial()->Bind();
		//GetMesh()->BindBuffer();

		//GetMesh()->Render();

		//GetMaterial()->Clear();
		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();
		BoneAnimator* animator
			= GetOwner()->GetComponent<BoneAnimator>();

		UINT subSetCount = GetMesh()->GetSubSetCount();
		for (UINT i = 0; i < subSetCount; i++)
		{
			if (animator)
			{
				animator->Binds();
				GetMaterial((int)i)->SetAnimation(true);

			}

			GetMesh()->BindBuffer(i);
			GetMaterial((int)i)->Bind();
			GetMesh()->Render(i);

			GetMaterial((UINT)i)->Clear();
		}

		if (animator)
		{
			animator->ClearData();
		}
		//GetMesh()->BindBuffer(0);
		//GetMaterial(0)->Bind();

		//Animator* animator = GetOwner()->GetComponent<Animator>();
		//if (animator)
		//{
		//	animator->Binds();
		//}

		//GetMesh()->Render(0);
		//GetMaterial(0)->Clear();

		//if (animator)
		//{
		//	animator->Clear();
		//}
	}
}
