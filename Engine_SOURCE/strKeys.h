#pragma once
#define STRKEY constexpr const wchar_t*
#define MAKE_STRKEY(_wstr) constexpr const wchar_t* _wstr = L#_wstr

namespace strKeys
{
	namespace mesh
	{
		MAKE_STRKEY(PointMesh);
		MAKE_STRKEY(RectMesh);
		MAKE_STRKEY(DebugRectMesh);
		MAKE_STRKEY(CircleMesh);
		MAKE_STRKEY(CubeMesh);
		MAKE_STRKEY(SphereMesh);
		MAKE_STRKEY(LineMesh);
		MAKE_STRKEY(CylinderMesh);
	}
	namespace shader
	{
		MAKE_STRKEY(RectShader);
		MAKE_STRKEY(SpriteShader);
		MAKE_STRKEY(PostureShader);
		MAKE_STRKEY(UIShader);
		MAKE_STRKEY(MeterShader);
		MAKE_STRKEY(GridShader);
		MAKE_STRKEY(DebugShader);
		MAKE_STRKEY(PaintShader);
		MAKE_STRKEY(ParticleShader);
		MAKE_STRKEY(PostProcessShader);
		MAKE_STRKEY(BasicShader);
		MAKE_STRKEY(DeferredShader);
		MAKE_STRKEY(LightDirShader);
		MAKE_STRKEY(LightPointShader);
		MAKE_STRKEY(MergeShader);
		MAKE_STRKEY(ShadowShader);
		MAKE_STRKEY(SkyBoxShader);
		MAKE_STRKEY(PhysXDebugShader);
		MAKE_STRKEY(BoneComputeShader);
	}
}