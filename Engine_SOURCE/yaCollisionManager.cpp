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


	void CollisionManager::drawRaycast(const Vector3& origin, const Vector3& direction, float maxDistance, const Vector3& color)
	{
		DebugMesh debugInfo{};
		debugInfo.type = eColliderType::Line;
		debugInfo.position = origin;

		Vector3 normDir = direction;
		direction.Normalize(normDir);

		//각도를 구해준다
		debugInfo.rotation.x = -std::atan2f(normDir.y, normDir.z);
		debugInfo.rotation.y = std::asin(normDir.x);
		debugInfo.rotation.z = 0.f;
		debugInfo.rotation *= gRadToDegreeFactor;


		//Vector3 rotationAxis = Vector3::UnitX.Cross(normDir);
		//float rotationAngle = Vector3::UnitX.Dot(normDir);

		//// Assuming rotationAxis is normalized
		//debugInfo.rotation.x = std::atan2(rotationAxis.y, rotationAxis.z);
		//debugInfo.rotation.y = std::atan2(-rotationAxis.x, std::sqrt(rotationAxis.y * rotationAxis.y + rotationAxis.z * rotationAxis.z));
		//debugInfo.rotation.z = rotationAngle;



		//debugInfo.rotation.x = std::acosf(normDir.Dot(Vector3::UnitX));
		//debugInfo.rotation.y = std::acosf(normDir.Dot(Vector3::UnitY));
		//debugInfo.rotation.z = std::acosf(normDir.Dot(Vector3::UnitZ));

		//float rotX = std::atan2f(0.f, 0.f) - std::atan2f(direction.y, direction.z);
		//float rotY = std::atan2f(0.f, 1.f) - std::atan2f(direction.z, direction.x);
		//float rotZ = std::atan2f(1.f, 0.f) - std::atan2f(direction.x, direction.y);
		//debugInfo.rotation = Vector3(rotX, rotY, rotZ);

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
