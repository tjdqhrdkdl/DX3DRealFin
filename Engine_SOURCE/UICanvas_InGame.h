#pragma once
#include "UICanvasObj.h"
#include <unordered_map>

namespace ya
{
    namespace InGameUITextures
    {
        enum ENUM
        {
            HUD_Player,
            HUD_Enemy,

        };
    }


    class UICanvas_InGame :
        public UICanvasObj
    {
    public:
        UICanvas_InGame();
        virtual ~UICanvas_InGame();

        virtual void Initialize() override;

        virtual void Render() override;

    private:
        void CreateHP_HUD();
        void CreateGeneralHUD();
    };
}


