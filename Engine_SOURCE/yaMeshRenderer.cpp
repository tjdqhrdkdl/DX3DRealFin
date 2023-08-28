#include "yaMeshRenderer.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaAnimator.h"
#include "yaBoneAnimator.h"


namespace ya
{
	MeshRenderer::MeshRenderer()
		: BaseRenderer(eComponentType::MeshRenderer)
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
		for (size_t i = 0; i < subSetCount; i++)
		{
			if (animator)
			{
				animator->Binds();
				GetMaterial(i)->SetAnimation(true);

			}

			GetMesh()->BindBuffer(i);
			GetMaterial(i)->Bind();
			GetMesh()->Render(i);

			GetMaterial(i)->Clear();
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
