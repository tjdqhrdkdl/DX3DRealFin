#pragma once
#include "yaGameObject.h"

namespace ya
{
    class MonsterBase :
        public GameObject
    {

    public:
        MonsterBase();
        virtual ~MonsterBase();


    public:

        virtual void Initalize() override;
        virtual void Update() override;
        virtual void FixedUpdate() = 0;
        virtual void Render() = 0;



    private:




    };
}
