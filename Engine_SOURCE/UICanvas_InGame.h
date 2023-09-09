#pragma once
#include "UICanvasObj.h"
#include <unordered_map>

namespace ya
{


    enum class eInGameUITextureSlot
    {
        MainHudTexture,
        END
    };



    class UICanvas_InGame :
        public UICanvasObj
    {
    public:
        UICanvas_InGame();
        virtual ~UICanvas_InGame();

        virtual void Initialize() override;


        virtual void Render() override;
        

    private:

    };
}


