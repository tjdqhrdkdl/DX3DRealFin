#pragma once
#include "yaGameObject.h"

namespace ya
{
    class Player;
    class PlayerScreenUI : public GameObject
    {
    public:
        PlayerScreenUI();
        virtual ~PlayerScreenUI();

    public:
        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;

    public:
        void SetPlayer(Player* player);

        void OnDeathUI(bool on = true);
        void OnGameOverUI(bool on = true);

    private:
        void DeathUI(bool on = true);
        void GameOverUI(bool on = true);

    private:
        Player* mPlayer;

        bool    mbDeathUI;
        float   mDeathUITimer;
        bool    mbGameOverUI;
        float   mGameOverUITimer;

        // HP
        GameObject* mHpLayout;
        GameObject* mHpBar;

        /// 회생
        std::vector<GameObject*> mResurrections;
        GameObject* mResurrectionFalse;
        //GameObject* mResurrectionText;

        // 체간
        GameObject* mPostureLayout;

        // 죽음
        GameObject* mDeathText;
        GameObject* mBloodScreen;
        GameObject* mBloodDeathScreen;
        GameObject* mFadeObject;

        // 마우스클릭 UI
        GameObject* mKeyRBTN;
        GameObject* mKeyLBTN;

    };

}