#pragma once
#include "yaGameObject.h"


namespace ya
{
    class Musketeerman_Almost :
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
