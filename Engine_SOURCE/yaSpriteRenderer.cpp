#include "yaSpriteRenderer.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaAnimator.h"

namespace ya
{
	SpriteRenderer::SpriteRenderer()
		: BaseRenderer(eComponentType::SpriteRenderer)
		, ComponentType(eComponentType::SpriteRenderer)
	{
	}

	SpriteRenderer::~SpriteRenderer()
	{
	}

	void SpriteRenderer::Initialize()
	{
	}

	void SpriteRenderer::Update()
	{
	}

	void SpriteRenderer::FixedUpdate()
	{
	}

	void SpriteRenderer::Render()
	{
		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

		GetMesh()->BindBuffer(0);
		GetMaterial(0)->Bind();

		Animator* animator = GetOwner()->GetComponent<Animator>();
		if (animator)
		{
			animator->Binds();
		}

		GetMesh()->Render(0);
		GetMaterial(0)->Clear();

		if (animator)
		{
			animator->Clear();
		}

	}

}
