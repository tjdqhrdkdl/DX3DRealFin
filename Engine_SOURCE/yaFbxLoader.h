#pragma once
#include "yaEngine.h"
#include "yaMath.h"
#include "yaGraphics.h"

#include <fbxsdk/scene/geometry/fbxnurbs.h >
#include <fbxsdk.h>

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\fbx\\lib\\Debug\\libfbxsdk-md.lib") 
#else 
#pragma comment(lib, "..\\External\\fbx\\lib\\Release\\libfbxsdk-md.lib") 
#endif



namespace ya
{
    using namespace math;
    using namespace graphics;

    class FbxLoader
    {
    public:
        FbxLoader();


    private:
        static fbxsdk::FbxManager* mManager;
        static fbxsdk::FbxScene* mScene;
        static fbxsdk::FbxImporter* mImporter;
    };
}