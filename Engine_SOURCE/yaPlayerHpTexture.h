#pragma once
#include "yaGameObject.h"

namespace ya
{
    class Player;
    class PlayerHpTexture : public GameObject
    {
    public:
        PlayerHpTexture();
        virtual ~PlayerHpTexture();

    public:
        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;

        void SetPlayer(Player* player);

    private:
        Player* mPlayer;

        GameObject* mHpLayout;
        GameObject* mHpBar;

        std::vector<GameObject*> mResurrections;
        GameObject* mResurrectionFalse;

        GameObject* mPostureLayout;
        //GameObject* mPlayerpostureBar;

    };

}