#pragma once
#include "yaResource.h"
#include "yaShader.h"
#include "yaTexture.h"
#include "yaRenderer.h"
//#include "yaConstantBuffer.h"

using namespace ya::renderer;
namespace ya::graphics
{
	class Material : public Resource
	{
	public:
		Material();

		//복사 생성자(UI에서 사용)
		Material(const Material& _other) = default;
		std::shared_ptr<Material> Clone() { return std::make_shared<Material>(*this); }

		virtual ~Material();

		virtual HRESULT Save(const std::wstring& path, FILE* file = nullptr) override;
		virtual HRESULT Load(const std::wstring& filename, FILE* file = nullptr) override;

		void SetData(eGPUParam param, void* data);
		void Bind();
		void Clear();

		void SetShader(std::shared_ptr<Shader> shader) { mShader = shader; }
		std::shared_ptr<Shader> GetShader() { return mShader; }
		void SetTexture(eTextureSlot slot, std::shared_ptr<Texture> texture) { mTexture[(UINT)slot] = texture; }
		std::shared_ptr<Texture> GetTexture(eTextureSlot slot) { return mTexture[(UINT)slot]; }

		eRenderingMode GetRenderingMode() { return mMode; }
		void SetRenderingMode(eRenderingMode mode) { mMode = mode; }

		void SetMaterialCoefficient(Vector4 _vDiff, Vector4 _vSpec, Vector4 _vAmb, Vector4 _vEmis)
		{
			mDiffuseColor = _vDiff;
			mSpecularColor = _vSpec;
			mAmbientColor = _vAmb;
			mEmissiveColor = _vEmis;
		}
		void SetAnimation(bool enable) { mbAnimaion = enable; }

	private:
		std::shared_ptr<Shader> mShader;
		std::shared_ptr<Texture> mTexture[(UINT)eTextureSlot::End];
		MaterialCB mCB;
		eRenderingMode mMode;

		Vector4 mDiffuseColor;
		Vector4 mSpecularColor;
		Vector4 mAmbientColor;
		Vector4 mEmissiveColor;
		bool mbAnimaion;

	};
}

