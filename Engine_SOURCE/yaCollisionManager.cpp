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
	bool CollisionManager::initialize(void)
	{
		PhysxWrapper::getInstance().initialize();

		return true;
	}

	void CollisionManager::update(float deltaTime)
	{
		if (_enable == false)
			return;

		PhysxWrapper::getInstance().update(deltaTime);
	}

	void CollisionManager::enableRaycast(UINT32 leftLayerIndex, UINT32 rightLayerIndex, bool enable)
	{
		PhysxWrapper::getInstance().enableRaycast(leftLayerIndex, rightLayerIndex, enable);
	}

	void CollisionManager::enableCollision(UINT32 leftLayerIndex, UINT32 rightLayerIndex, bool enable)
	{
		PhysxWrapper::getInstance().enableCollision(leftLayerIndex, rightLayerIndex, enable);
	}

	void CollisionManager::enableGravity(bool enable, Scene* pScene, Vector3 gravity)
	{
		gravity = (enable) ? gravity : Vector3{ 0.f, 0.f, 0.f };
		PhysxWrapper::getInstance().enableGravity(enable, pScene, gravity);
	}


	bool CollisionManager::raycast(UINT32 layerIndex, const Vector3& origin, const Vector3& direction, float maxDistance, RaycastHit* outHit)
	{
		Vector3 normalized{};
		direction.Normalize(normalized);
		return PhysxWrapper::getInstance().raycast(layerIndex, origin, normalized, maxDistance, outHit);
	}




	void CollisionManager::drawRaycast(const Vector3& origin, const Vector3& direction, float maxDistance, const Vector3& color)
	{
		using namespace renderer;

		Vector3 forward{};
		direction.Normalize(forward);
		const Matrix world = Matrix::CreateWorld(origin, forward, Vector3{ 0.f, 1.f, 0.f });
		TransformCB	 buffer{};
		buffer.world = Matrix::CreateScale(maxDistance) * world;
		//const Camera& camera = Application::getInstance().getCamera();
		buffer.view = Camera::GetGpuViewMatrix();
		buffer.projection = Camera::GetGpuProjectionMatrix();

		ConstantBuffer* positionBuffer = renderer::constantBuffers[(int)eCBType::Transform];
		positionBuffer->SetData(&buffer);
		positionBuffer->Bind(eShaderStage::ALL);
		
		static std::shared_ptr<Mesh>		  line = Resources::Find<Mesh>(L"LineMesh");
		static std::shared_ptr<Shader> shader = Resources::Find<Shader>(L"DebugShader");

		shader->Binds();
		line->Render(0u);
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
