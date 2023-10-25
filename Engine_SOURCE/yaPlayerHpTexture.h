#pragma once
#include "yaGameObject.h"

namespace ya
{
    class PlayerHpTexture : public GameObject
    {
    public:
        PlayerHpTexture();
        virtual ~PlayerHpTexture();

    public:

        virtual void Initialize() override;
        virtual void Update() override;

        void SetPlayer(class Player* player) { mPlayer = player; }

        void PlayerMeterCheak();
        
        void PlayerResurrection_True();
        void PlayerResurrection_False();

    private:

        class Player* mPlayer;
        GameObject* mPlayerHpLayout;
        GameObject* mPlayerHpBar;
        GameObject* mPlayerResurrection;
        GameObject* mPlayerpostureLayout;
        //GameObject* mPlayerpostureBar;

    };

}