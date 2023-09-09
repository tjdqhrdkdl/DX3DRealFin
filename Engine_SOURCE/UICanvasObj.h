#pragma once
#include "yaGameObject.h"
#include "UINames.h"

namespace ya
{
    constexpr UINT Register_UIBuffer = 20u;
    constexpr math::Vector2 GameResolution = math::Vector2(1600.f, 900.f);

    namespace graphics
    {
        class StructedBuffer;
        class Texture;
    }
    struct tUIInfo
    {
        int TextureSlot{};
        Vector3 Padding_UIInfo{};
        
        Vector2 UVBeginInTexture{};
        Vector2 UVSizeInTexture{};
    };

    struct tUIRenderInfo
    {
        tUIInfo UIInfo;

        Vector2 UVBeginInCanvas{};
        Vector2 UVSizeInCanvas{};

        float ZValue{};
        Vector3 Padding_UIRenderInfo;
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
        inline bool RenderUI(const std::wstring& _name, const Vector2& _start, const Vector2& _size, float _z, bool _isUV = false);
        void RenderUI(const tUIRenderInfo& _info) { mUIRenderQueue.push_back(_info); }
        
        bool AddUIInfo(const std::wstring& _name, int _texSlot, const Vector2& _start, const Vector2& _end);
        inline void AddUIInfo(const std::wstring& _UIName, const tUIInfo& _uiInfo);
        

        const tUIInfo* FindUIInfo(const std::wstring& _UIName);

	private:
		//텍스처별 UV값 정리
		std::unordered_map<std::wstring, tUIInfo> mMapUIInfo;

        std::vector<tUIRenderInfo> mUIRenderQueue;

		std::unique_ptr<graphics::StructedBuffer> mUIDrawSBuffer;
	};

    inline void UICanvasObj::AddUIInfo(const std::wstring& _UIName, const tUIInfo& _uiInfo)
    {
        mMapUIInfo.insert(std::make_pair(_UIName, _uiInfo));
    }

    inline const tUIInfo* UICanvasObj::FindUIInfo(const std::wstring& _UIName)
    {
        const tUIInfo* uiInfo = nullptr;
        const auto& iter = mMapUIInfo.find(_UIName);
        if (iter != mMapUIInfo.end())
        {
            uiInfo = &iter->second;
        }

        return uiInfo;
    }

    inline bool UICanvasObj::RenderUI(const std::wstring& _name, const Vector2& _start, const Vector2& _size, float _z = 0.f, bool _isUV)
    {
        bool suc = false;
        const tUIInfo* uiInfo = FindUIInfo(_name);
        if (uiInfo)
        {
            tUIRenderInfo renderInfo{};
            renderInfo.UIInfo = *uiInfo;
            if (_isUV)
            {
                renderInfo.UVBeginInCanvas = _start;
                renderInfo.UVSizeInCanvas = _size;
            }
            else
            {
                renderInfo.UVBeginInCanvas = _start / GameResolution;
                renderInfo.UVSizeInCanvas = _size / GameResolution;
            }

            renderInfo.ZValue = _z;
            RenderUI(renderInfo);
            suc = true;
        }
        return suc;
    }
}


