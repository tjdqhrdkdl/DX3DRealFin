#pragma once
#include "UICanvasObj.h"
#include <unordered_map>

namespace ya
{
    namespace graphics
    {
        class StructedBuffer;
    }

    enum class eInGameUITextureSlot
    {
        MainHudTexture,
        END
    };

    struct tUI_UVPos
    {
        eInGameUITextureSlot TextureSlot{};
        Vector2 UVBegin{};
        Vector2 UVEnd{};
    };

    class UICanvas_InGame :
        public UICanvasObj
    {
    public:
        UICanvas_InGame();
        virtual ~UICanvas_InGame();

        virtual void Initialize() override;



    private:
        //텍스처별 UV값 정리
        std::unordered_map<std::wstring, tUI_UVPos> mMapUI;

        std::unique_ptr<graphics::StructedBuffer> mUIDrawSBuffer;
    };
}


