#pragma once
#include "yaGameObject.h"

namespace ya
{
    class PlayerHpTxture :
        public GameObject
    {

    public:
        PlayerHpTxture();
        virtual ~PlayerHpTxture();


    public:

        virtual void Initialize() override;
        virtual void Update() override;

        void SetPlayer(class Player* player) { mPlayer = player; }


        void PlayerHpCheak();

        
        void PlayerResurrection_True();
        void PlayerResurrection_False();


    private:

        class Player* mPlayer;
        GameObject* mPlayerHpLayout;
        GameObject* mPlayerHpBar;
        GameObject* mPlayerResurrection;


     

    };

}