#include "yaCollisionManager.h"

#include "PhysxWrapper.h"

#include "yaApplication.h"
#include "yaCamera.h"
#include "yaScene.h"

#include "yaResources.h"
#include "yaConstantBuffer.h"
#include "yaShader.h"
#include "yaRenderer.h"

namespace ya
{
	bool CollisionManager::Initialize(void)
	{
		PhysxWrapper::getInstance().Initialize();

		return true;
	}

	void CollisionManager::Update(float deltaTime)
	{
		if (_enable == false)
			return;

		PhysxWrapper::getInstance().Update(deltaTime);
	}

	void CollisionManager::EnableRaycast(UINT32 leftLayerIndex, UINT32 rightLayerIndex, bool enable)
	{
		PhysxWrapper::getInstance().EnableRaycast(leftLayerIndex, rightLayerIndex, enable);
	}

	void CollisionManager::EnableCollision(UINT32 leftLayerIndex, UINT32 rightLayerIndex, bool enable)
	{
		PhysxWrapper::getInstance().EnableCollision(leftLayerIndex, rightLayerIndex, enable);
	}

	void CollisionManager::EnableGravity(bool enable, Scene* pScene, Vector3 gravity)
	{
		gravity = (enable) ? gravity : Vector3{ 0.f, 0.f, 0.f };
		PhysxWrapper::getInstance().EnableGravity(enable, pScene, gravity);
	}


	bool CollisionManager::Raycast(UINT32 srcLayerIndex, const Vector3& origin, const Vector3& direction, float maxDistance, RaycastHit* outHit)
	{
		Vector3 normalized{};
		direction.Normalize(normalized);
		return PhysxWrapper::getInstance().Raycast(srcLayerIndex, origin, normalized, maxDistance, outHit);
	}




	void CollisionManager::drawRaycast(const Vector3& origin, const Vector3& direction, float maxDistance, const Vector3& color)
	{
		DebugMesh debugInfo{};
		debugInfo.position = origin;
		debugInfo.rotation = direction;
		debugInfo.scale = Vector3(maxDistance);
		debugInfo.collisionCount = 1u;

		renderer::debugMeshes.push_back(debugInfo);

		//using namespace renderer;

		//Vector3 forward{};
		//direction.Normalize(forward);
		//const Matrix world = Matrix::CreateWorld(origin, forward, Vector3{ 0.f, 1.f, 0.f });
		//TransformCB	 buffer{};
		//buffer.world = Matrix::CreateScale(maxDistance) * world;
		////const Camera& camera = Application::getInstance().getCamera();
		//buffer.view = Camera::GetGpuViewMatrix();
		//buffer.projection = Camera::GetGpuProjectionMatrix();

		//ConstantBuffer* positionBuffer = renderer::constantBuffers[(int)eCBType::Transform];
		//positionBuffer->SetData(&buffer);
		//positionBuffer->Bind(eShaderStage::ALL);
		//
		//static std::shared_ptr<Mesh>		  line = Resources::Find<Mesh>(L"LineMesh");
		//static std::shared_ptr<Shader> shader = Resources::Find<Shader>(L"DebugShader");

		//shader->Binds();
		//line->Render(0u);
	}

	void CollisionManager::createPhysicScene(Scene* scene)
	{
		PhysxWrapper::getInstance().createScene(scene);
	}

	void CollisionManager::changePhysicScene(Scene* scene)
	{
		PhysxWrapper::getInstance().changeScene(scene);
	}
} //namespace ya
