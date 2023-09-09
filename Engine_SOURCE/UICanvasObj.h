#pragma once
#include "yaGameObject.h"

namespace ya
{
    constexpr UINT Register_UIBuffer = 20u;

    namespace graphics
    {
        class StructedBuffer;
    }
    struct tUIInfo
    {
        int TextureSlot{};
        Vector3 Padding{};
        
        Vector2 UVBeginInTexture{};
        Vector2 UVSizeInTexture{};
    };

    struct tUIRenderInfo
    {
        tUIInfo UIInfo;

        Vector2 UVBeginInCanvas{};
        Vector2 UVSizeInCanvas{};
    };


	class UICanvasObj :
		public GameObject
	{
	public:
		UICanvasObj();
		virtual ~UICanvasObj();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

    public:
        void RenderUI(const tUIRenderInfo& _info) { mUIRenderQueue.push_back(_info); }
        
        inline void AddUIInfo(const std::wstring& _UIName, const tUIInfo& _uiInfo);

        const tUIInfo* FindUIInfo(const std::wstring& _UIName);

	private:       
		//텍스처별 UV값 정리
		std::unordered_map<std::wstring, tUIInfo> mMapUI;

        std::vector<tUIRenderInfo> mUIRenderQueue;

		std::unique_ptr<graphics::StructedBuffer> mUIDrawSBuffer;
	};

    inline void UICanvasObj::AddUIInfo(const std::wstring& _UIName, const tUIInfo& _uiInfo)
    {
        mMapUI.insert(std::make_pair(_UIName, _uiInfo));
    }
}


