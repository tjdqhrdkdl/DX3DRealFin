#pragma once
#include "yaGameObject.h"

namespace ya
{
    class Spearman_Sting :
        public GameObject
    {

    public:

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() override;
        //virtual void Render() override;


    private:


        float       mDeathTime;


    };

}