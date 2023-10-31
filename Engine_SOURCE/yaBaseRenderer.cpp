#include "yaBaseRenderer.h"


namespace ya
{
	BaseRenderer::BaseRenderer(eComponentType type)
		: Component(type)
	{
		mMaterial.resize(20);
	}

	void BaseRenderer::SetMesh(std::shared_ptr<Mesh> mesh)
	{
		mMesh = mesh;
		if (!mMaterial.empty())
			mMaterial.clear();

		UINT count = mMesh->GetSubSetCount();
		mMaterial.resize(count);
	}
}
