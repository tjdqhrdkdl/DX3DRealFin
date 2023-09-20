#pragma once
#include "yaMath.h"


namespace ya
{
    class Frustum
    {
    public:
        Frustum();
        Frustum(const Frustum&);
        ~Frustum();
        
        void ConstructFrustum(float screenDepth, XMMATRIX projectionMatrix, XMMATRIX viewMatrix);

        bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);

    private:
        XMVECTOR m_planes[6];
    };
}