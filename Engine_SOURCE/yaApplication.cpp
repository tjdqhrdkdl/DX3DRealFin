#include "yaApplication.h"
#include "yaRenderer.h"
#include "yaTime.h"
#include "yaInput.h"
#include "yaSceneManager.h"
#include "yaResources.h"
#include "yaCollisionManager.h"
#include "yaFmod.h"
#include "yaFontWrapper.h"
#include "ThreadPool.h"

namespace ya
{
	using namespace graphics;

	Application::Application()
		: initialized(false)
		, graphicDevice()
		, mHwnd{}
		, mHdc{}
		, mHeight{}
		, mWidth{}
	{
		Plane pla = Plane::Plane(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	}

	Application::~Application()
	{
		
	}

	void Application::Initialize()
	{
		ThreadPool::Initialize((size_t)std::thread::hardware_concurrency());
		Time::Initialize();
		Input::Initialize();
		Fmod::Initialize();
		FontWrapper::Initialize();

		CollisionManager::Initialize();
		renderer::Initialize();
		SceneManager::Initialize();
	}

	// 게임 로직 캐릭터 이동 등등 
	// CPU UPDATE
	void Application::Update()
	{
		Time::Update();
		Input::Update();
		SceneManager::Update();
		CollisionManager::Update(Time::DeltaTime());
	}

	// GPU Update
	void Application::FixedUpdate()
	{
		SceneManager::FixedUpdate();
	}

	void Application::Render()
	{
		Time::Render(mHdc);

		//graphicDevice->Clear();
		graphicDevice->AdjustViewPorts();
		renderer::ClearRenderTargets();

		
		renderer::Render();
	}

	void Application::Destroy()
	{
		SceneManager::Destroy();
	}

	// Running main engine loop
	void Application::Run()
	{
		Update();
		FixedUpdate();
		Render();
		Destroy();
	}

	void Application::Present()
	{
		graphicDevice->Present();
	}

	void Application::Release()
	{
		ThreadPool::Release();
		Resources::deleteTest();
		Fmod::Release();
		FontWrapper::Release();
	}

	void Application::SetWindow(HWND hwnd, UINT width, UINT height)
	{
		if (graphicDevice == nullptr)
		{
			mHwnd = hwnd;
			mHdc = GetDC(mHwnd);
			mWidth = width;
			mHeight = height;


			ValidationMode vaildationMode = ValidationMode::Disabled;
			graphicDevice = std::make_unique<GraphicDevice_DX11>();
			//graphics::GetDevice() = graphicDevice.get();
		}

		RECT rt = { 0, 0, (LONG)width , (LONG)height };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
		ShowWindow(mHwnd, true);
		UpdateWindow(mHwnd);
	}
}
