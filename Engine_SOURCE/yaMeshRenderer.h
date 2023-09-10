#pragma once
#include "yaComponent.h"
#include "yaMesh.h"
#include "yaMaterial.h"
#include "yaBaseRenderer.h"

using namespace ya::graphics;
namespace ya
{
	class MeshRenderer : public BaseRenderer
	{
	public:
		MeshRenderer();
		virtual ~MeshRenderer();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void PrevRender() override;
		virtual void Render() override;
	};
}