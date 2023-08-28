#include "yaMaterial.h"
#include "yaResources.h"

namespace ya::graphics
{
    Material::Material()
        : Resource(eResourceType::Material)
        , mMode(eRenderingMode::Opaque)
        , mbAnimaion(false)
    {

    }

    Material::~Material()
    {

    }

    HRESULT Material::Save(const std::wstring& path, FILE* file)
    {      

        std::wstring fullPath = path;
        std::wstring key = GetKey();

        UINT nameSize = key.size();
        fwrite(&nameSize, sizeof(UINT), 1, file);
        fwrite(key.c_str(), key.size() * sizeof(wchar_t), 1, file);

        UINT pathSize = fullPath.size();
        fwrite(&pathSize, sizeof(UINT), 1, file);
        fwrite(fullPath.c_str(), fullPath.size() * sizeof(wchar_t), 1, file);

        // 해당 정보들 저장 해주어야 한다.
        //std::shared_ptr<Shader> mShader;
        key = mShader->GetKey();
        fullPath = mShader->GetPath();
        //fullPath = L"..\\SHADER_SOURCE\\";


        nameSize = key.size();
        fwrite(&nameSize, sizeof(UINT), 1, file);
        fwrite(key.c_str(), key.size() * sizeof(wchar_t), 1, file);

        pathSize = fullPath.size();
        fwrite(&pathSize, sizeof(UINT), 1, file);
        fwrite(fullPath.c_str(), fullPath.size() * sizeof(wchar_t), 1, file);


        //std::shared_ptr<Texture> mTexture[(UINT)eTextureSlot::End];
        for (size_t i = 0; i < (UINT)eTextureSlot::End; i++)
        {
            if (mTexture[i] == nullptr)
            {
                int j = 0;
                fwrite(&j, sizeof(int), 1, file);

                continue;
            }
            else
            {
                int j = 1;
                fwrite(&j, sizeof(int), 1, file);
            }

            key = mTexture[i]->GetKey();
            fullPath = mTexture[i]->GetPath();

            nameSize = key.size();
            fwrite(&nameSize, sizeof(UINT), 1, file);
            fwrite(key.c_str(), key.size() * sizeof(wchar_t), 1, file);

            pathSize = fullPath.size();
            fwrite(&pathSize, sizeof(UINT), 1, file);
            fwrite(fullPath.c_str(), fullPath.size() * sizeof(wchar_t), 1, file);
        }

        ////BoneAnimationCB mBoneCB;
        //fwrite(&mBoneCB.boneCount, sizeof(UINT), 1, file);

        //if (mBoneCB.boneCount <= 0)
        //    return S_OK;

        //fwrite(&mBoneCB.frameIdx, sizeof(UINT), 1, file);
        //fwrite(&mBoneCB.nextFrameIdx, sizeof(UINT), 1, file);
        //fwrite(&mBoneCB.frameRatio, sizeof(float), 1, file);

        //eRenderingMode mMode;
        fwrite(&mMode, sizeof(UINT), 1, file);
        fwrite(&mDiffuseColor, sizeof(Vector4), 1, file);
        fwrite(&mSpecularColor, sizeof(Vector4), 1, file);
        fwrite(&mAmbientColor, sizeof(Vector4), 1, file);
        fwrite(&mEmissiveColor, sizeof(Vector4), 1, file);
        fwrite(&mbAnimaion, sizeof(bool), 1, file);

        return S_OK;
    }

    HRESULT Material::Load(const std::wstring& filename, FILE* file)
    {
        
        wchar_t buff[256] = {};
        UINT nameSize = 0;
        fread(&nameSize, sizeof(UINT), 1, file);
        fread(buff, sizeof(wchar_t), nameSize, file);
        std::wstring key = buff;
        SetKey(key);

        UINT pathSize = 0;
        fread(&pathSize, sizeof(UINT), 1, file);
        fread(buff, sizeof(wchar_t), pathSize, file);
        std::wstring path = buff;
        SetPath(buff);


        // 해당 정보들 저장 해주어야 한다.
        //std::shared_ptr<Shader> mShader;
        key = L"";
        path = L"";
        ZeroMemory(buff, 256);        

        nameSize = 0;
        fread(&nameSize, sizeof(UINT), 1, file);
        fread(buff, sizeof(wchar_t), nameSize, file);
        key = buff;

        ZeroMemory(buff, 256);

        pathSize = 0;
        fread(&pathSize, sizeof(UINT), 1, file);
        fread(buff, sizeof(wchar_t), pathSize, file);
        path = buff;

        // 구조 바뀌어야 한다.
        //path = L"..\\SHADER_SOURCE\\BasicShader";

        mShader = Resources::Find<Shader>(key);

        //std::shared_ptr<Texture> mTexture[(UINT)eTextureSlot::End];
        for (size_t i = 0; i < (UINT)eTextureSlot::End; i++)
        {
            int j = -1;
            fread(&j, sizeof(int), 1, file);

            if (j == 0)
                continue;

            key = L"";
            path = L"";

            ZeroMemory(buff, 256);

            nameSize = 0;
            fread(&nameSize, sizeof(UINT), 1, file);
            fread(buff, sizeof(wchar_t), nameSize, file);
            key = buff;

            ZeroMemory(buff, 256);

            pathSize = 0;
            fread(&pathSize, sizeof(UINT), 1, file);
            fread(buff, sizeof(wchar_t), pathSize, file);
            path = buff;

            std::filesystem::path parentPath = std::filesystem::current_path().parent_path();
            std::wstring fullPath = parentPath.wstring() + L"\\Resources\\" + path;

            mTexture[i] = Resources::Load<Texture>(key, path);
            
        }

        ////BoneAnimationCB mBoneCB;
        //fread(&mBoneCB.boneCount, sizeof(UINT), 1, file);

        //if (mBoneCB.boneCount <= 0)
        //    return S_OK;

        //fread(&mBoneCB.frameIdx, sizeof(UINT), 1, file);
        //fread(&mBoneCB.nextFrameIdx, sizeof(UINT), 1, file);
        //fread(&mBoneCB.frameRatio, sizeof(float), 1, file);

        //eRenderingMode mMode;
        fread(&mMode, sizeof(UINT), 1, file);
        fread(&mDiffuseColor, sizeof(Vector4), 1, file);
        fread(&mSpecularColor, sizeof(Vector4), 1, file);
        fread(&mAmbientColor, sizeof(Vector4), 1, file);
        fread(&mEmissiveColor, sizeof(Vector4), 1, file);
        fread(&mbAnimaion, sizeof(bool), 1, file);


        return E_NOTIMPL;
    }



    void Material::SetData(eGPUParam param, void* data)
    {
        //switch (param)
        //{
        //case ya::graphics::eGPUParam::Int:
        //    mCB.iData = *static_cast<int*>(data);
        //    break;
        //case ya::graphics::eGPUParam::Float:
        //    mCB.fData = *static_cast<float*>(data);
        //    break;
        //case ya::graphics::eGPUParam::Vector2:
        //    mCB.xy = *static_cast<Vector2*>(data);
        //    break;
        //case ya::graphics::eGPUParam::Vector3:
        //    mCB.xyz = *static_cast<Vector3*>(data);
        //    break;
        //case ya::graphics::eGPUParam::Vector4:
        //    mCB.xyzw = *static_cast<Vector4*>(data);
        //    break;
        //case ya::graphics::eGPUParam::Matrix:
        //    mCB.matrix = *static_cast<Matrix*>(data);
        //    break;
        //default:
        //    break;
        //}

    }

    void Material::Bind()
    {
        for (size_t i = 0; i < (UINT)eTextureSlot::End; i++)
        {
            if (mTexture[i] == nullptr)
                continue;

            mTexture[i]->BindShaderResource(eShaderStage::VS, i);
            mTexture[i]->BindShaderResource(eShaderStage::HS, i);
            mTexture[i]->BindShaderResource(eShaderStage::DS, i);
            mTexture[i]->BindShaderResource(eShaderStage::GS, i);
            mTexture[i]->BindShaderResource(eShaderStage::PS, i);
            mTexture[i]->BindShaderResource(eShaderStage::CS, i);
        }

        if (mTexture[(UINT)eTextureSlot::Albedo])
            mCB.usedAlbedo = 1;

        if (mTexture[(UINT)eTextureSlot::Normal])
            mCB.usedNormal = 1;

        if (mTexture[(UINT)eTextureSlot::Specular])
            mCB.usedSpecular = 1;

        if (mbAnimaion)
            mCB.usedAnimation = 1;

        ConstantBuffer* pCB = renderer::constantBuffers[(UINT)eCBType::Material];
        pCB->SetData(&mCB);
        pCB->Bind(eShaderStage::VS);
        pCB->Bind(eShaderStage::GS);
        pCB->Bind(eShaderStage::PS);

        mShader->Binds();
    }

    void Material::Clear()
    {
        for (size_t i = 0; i < (UINT)eTextureSlot::End; i++)
        {
            if (mTexture[i] == nullptr)
                continue;

            mTexture[i]->Clear();
        }
    }
}