#pragma once
#include "yaGameObject.h"

namespace ya
{
    class PlayerDangerUI :
        public GameObject
    {

    public:
        PlayerDangerUI();
        virtual ~PlayerDangerUI();


    public:

        virtual void Initialize() override;
        virtual void Update() override;
        void SetPlayer(class Player* pl);

        void UIOn() { mbRender = true, mRenderTimeChecker = 0; }
        void UIOff() { mbRender = false, mRenderTimeChecker = 0; }

        void CBBind();

        //이미지 하나는 투명하게 나오고
        //이미지 하나는 명확하게 나옴.

        //투명한 이미지는 커지면서 투명도가 높아지며 사라지고
        //명확한 이미지는 일정 시간이후 투명도가 높아지며 사라짐.

    private:

        class Player* mPlayer;

        GameObject* mMainUI;
        GameObject* mEffectUI;

        bool mbRender;
        float mRenderTime;
        float mRenderTimeChecker;
    };

}