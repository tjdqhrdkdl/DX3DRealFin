#include "yaCollisionManager.h"
#include "yaScene.h"
#include "yaSceneManager.h"

namespace ya
{
	std::bitset<(UINT)eLayerType::End> CollisionManager::mLayerCollisionMatrix[(UINT)eLayerType::End] = {};
	std::map<UINT64, bool> CollisionManager::mCollisionMap;

	void CollisionManager::Initialize()
	{
	}
	void CollisionManager::Update()
	{
		Scene* scene = SceneManager::GetActiveScene();
		for (UINT row = 0; row < (UINT)eLayerType::End; row++)
		{
			for (UINT column = 0; column < (UINT)eLayerType::End; column++)
			{
				if (mLayerCollisionMatrix[row][column])
				{
					LayerCollision(scene, (eLayerType)row, (eLayerType)column);
				}
			}
		}
	}
	void CollisionManager::FixedUpdate()
	{
	}
	void CollisionManager::Render()
	{
	}
	void CollisionManager::CollisionLayerCheck(eLayerType left, eLayerType right, bool enable)
	{
		int row = 0;
		int column = 0;

		if ((UINT)left <= (UINT)right)
		{
			row = (UINT)left;
			column = (UINT)right;
		}
		else
		{
			row = (UINT)right;
			column = (UINT)left;
		}

		mLayerCollisionMatrix[row][column] = enable;
	}
	void CollisionManager::LayerCollision(Scene* scene, eLayerType left, eLayerType right)
	{
		const std::vector<GameObject*>& lefts = scene->GetGameObjects(left);
		const std::vector<GameObject*>& rights = scene->GetGameObjects(right);

		if (left == right)
		{
			for (size_t i = 0; i < lefts.size(); i++)
			{
				GameObject* left = lefts[i];

				{
					if (left->GetState() != GameObject::Active)
						continue;
					if (left->GetComponent<Collider2D>() == nullptr)
						continue;

					for (size_t k = i; k < rights.size(); k++)
					{
						GameObject* right = rights[k];

						if (right->GetState() != GameObject::Active)
							continue;
						if (right->GetComponent<Collider2D>() == nullptr)
							continue;
						if (left == right)
							continue;

						ColliderCollision(left->GetComponent<Collider2D>(), right->GetComponent<Collider2D>());
					}

					if ((UINT)left == (UINT)right)
						break;
				}
			}
		}

		else
		{
			for (GameObject* left : lefts)
			{
				if (left->GetState() != GameObject::Active)
					continue;
				if (left->GetComponent<Collider2D>() == nullptr)
					continue;

				for (GameObject* right : rights)
				{
					if (right->GetState() != GameObject::Active)
						continue;
					if (right->GetComponent<Collider2D>() == nullptr)
						continue;
					if (left == right)
						continue;

					ColliderCollision(left->GetComponent<Collider2D>(), right->GetComponent<Collider2D>());
				}

				if ((UINT)left == (UINT)right)
					break;
			}
		}
	}

	void CollisionManager::ColliderCollision(Collider2D* left, Collider2D* right)
	{
		// �� �浹ü ���̾�� ������ ID Ȯ��
		ColliderID colliderID;
		colliderID.left = (UINT)left->GetID();
		colliderID.right = (UINT)right->GetID();

		// ���� �浹 ������ �˻��Ѵ�.
		// ���࿡ �浹������ ���� ���¶��
		// �浹������ �������ش�.
		std::map<UINT64, bool>::iterator iter = mCollisionMap.find(colliderID.id);
		if (iter == mCollisionMap.end())
		{
			mCollisionMap.insert(std::make_pair(colliderID.id, false));
			iter = mCollisionMap.find(colliderID.id);
		}
		
		// �浹üũ�� ���ش�.
		if (Intersect(left, right)) // �浹�� �� ����
		{
			// ���� �浹�� Enter
			if (iter->second == false)
			{
				if (left->IsTriiger())
					left->OnTriggerEnter(right);
				else
					left->OnCollisionEnter(right);

				if (right->IsTriiger())
					right->OnTriggerEnter(left);
				else
					right->OnCollisionEnter(left);

				iter->second = true;
			}
			else // �浹 ������ ���� ���� Enter
			{
				if (left->IsTriiger())
					left->OnTriggerStay(right);
				else
					left->OnCollisionStay(right);

				if (right->IsTriiger())
					right->OnTriggerStay(left);
				else
					right->OnCollisionStay(left);
			}
		}
		else // �浹���� ��������
		{
			// �浹 ���λ��� Exit
			if (iter->second == true)
			{
				if (left->IsTriiger())
					left->OnTriggerExit(right);
				else
					left->OnCollisionExit(right);

				if (right->IsTriiger())
					right->OnTriggerExit(left);
				else
					right->OnCollisionExit(left);

				iter->second = false;
			}
		}
	}

	bool CollisionManager::Intersect(Collider2D* left, Collider2D* right)
	{
		// Rect vs Rect 
		// 0 --- 1
		// |     |
		// 3 --- 2
		Vector3 arrLocalPos[4] =
		{
			Vector3{-0.5f, 0.5f, 0.0f}
			,Vector3{0.5f, 0.5f, 0.0f}
			,Vector3{0.5f, -0.5f, 0.0f}
			,Vector3{-0.5f, -0.5f, 0.0f}
		};

		Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
		Transform* rightTr = right->GetOwner()->GetComponent<Transform>();

		Matrix leftMat = leftTr->GetWorldMatrix();
		Matrix rightMat = rightTr->GetWorldMatrix();


		if (left->GetColliderType() == eColliderType::Rect
			&& right->GetColliderType() == eColliderType::Rect
			)
		{	// �и��� ���� 4�� ���ϱ�
			Vector3 Axis[4] = {};

			Vector3 leftScale = Vector3(left->GetSize().x, left->GetSize().y, 1.0f);
			Matrix finalLeft = Matrix::CreateScale(leftScale);
			finalLeft *= leftMat;

			Vector3 rightScale = Vector3(right->GetSize().x, right->GetSize().y, 1.0f);
			Matrix finalRight = Matrix::CreateScale(rightScale);
			finalRight *= rightMat;

			Axis[0] = Vector3::Transform(arrLocalPos[1], finalLeft);
			Axis[1] = Vector3::Transform(arrLocalPos[3], finalLeft);
			Axis[2] = Vector3::Transform(arrLocalPos[1], finalRight);
			Axis[3] = Vector3::Transform(arrLocalPos[3], finalRight);

			Axis[0] -= Vector3::Transform(arrLocalPos[0], finalLeft);
			Axis[1] -= Vector3::Transform(arrLocalPos[0], finalLeft);
			Axis[2] -= Vector3::Transform(arrLocalPos[0], finalRight);
			Axis[3] -= Vector3::Transform(arrLocalPos[0], finalRight);

			for (size_t i = 0; i < 4; i++)
				Axis[i].z = 0.0f;

			Vector3 vc = leftTr->GetPosition() - rightTr->GetPosition();
			vc.z = 0.0f;

			Vector3 centerDir = vc;
			for (size_t i = 0; i < 4; i++)
			{
				Vector3 vA = Axis[i];
				//vA.Normalize();

				float projDist = 0.0f;
				for (size_t j = 0; j < 4; j++)
				{
					projDist += fabsf(Axis[j].Dot(vA) / 2.0f);
				}

				if (projDist < fabsf(centerDir.Dot(vA)))
				{
					return false;
				}
			}
			return true;
		}

		else if (left->GetColliderType() == eColliderType::Box
			&& right->GetColliderType() == eColliderType::Box
			)
		{
			Vector3 arrCubeLocalPos[24] =
			{
				//����
				arrCubeLocalPos[0] = Vector3(-0.5f, 0.5f, 0.5f) ,
				arrCubeLocalPos[1] = Vector3(0.5f, 0.5f, 0.5f),
				arrCubeLocalPos[2] = Vector3(0.5f, 0.5f, -0.5f),
				arrCubeLocalPos[3] = Vector3(-0.5f, 0.5f, -0.5f),
				//�Ʒ�					 3
				arrCubeLocalPos[4] = Vector3(-0.5f, -0.5f, -0.5f),
				arrCubeLocalPos[5] = Vector3(0.5f, -0.5f, -0.5f),
				arrCubeLocalPos[6] = Vector3(0.5f, -0.5f, 0.5f),
				arrCubeLocalPos[7] = Vector3(-0.5f, -0.5f, 0.5f),
				//��						  3
				arrCubeLocalPos[8] = Vector3(-0.5f, 0.5f, 0.5f),
				arrCubeLocalPos[9] = Vector3(-0.5f, 0.5f, -0.5f),
				arrCubeLocalPos[10] = Vector3(-0.5f, -0.5f, -0.5f),
				arrCubeLocalPos[11] = Vector3(-0.5f, -0.5f, 0.5f),
				//��						   
				arrCubeLocalPos[12] = Vector3(0.5f, 0.5f, -0.5f),
				arrCubeLocalPos[13] = Vector3(0.5f, 0.5f, 0.5f),
				arrCubeLocalPos[14] = Vector3(0.5f, -0.5f, 0.5f),
				arrCubeLocalPos[15] = Vector3(0.5f, -0.5f, -0.5f),
				//��						   
				arrCubeLocalPos[16] = Vector3(0.5f, 0.5f, 0.5f),
				arrCubeLocalPos[17] = Vector3(-0.5f, 0.5f, 0.5f),
				arrCubeLocalPos[18] = Vector3(-0.5f, -0.5f, 0.5f),
				arrCubeLocalPos[19] = Vector3(0.5f, -0.5f, 0.5f),
				//��						   
				arrCubeLocalPos[20] = Vector3(-0.5f, 0.5f, -0.5f),
				arrCubeLocalPos[21] = Vector3(0.5f, 0.5f, -0.5f),
				arrCubeLocalPos[22] = Vector3(0.5f, -0.5f, -0.5f),
				arrCubeLocalPos[23] = Vector3(-0.5f, -0.5f, -0.5f)
			};


			Matrix leftMat = leftTr->GetWorldMatrix();
			Matrix rightMat = rightTr->GetWorldMatrix();

			Vector3 leftScale = Vector3(left->GetSize().x, left->GetSize().y, left->GetSize().z);
			Matrix finalLeft = Matrix::CreateScale(leftScale);
			finalLeft *= leftMat;

			Vector3 rightScale = Vector3(right->GetSize().x, right->GetSize().y, right->GetSize().z);
			Matrix finalRight = Matrix::CreateScale(rightScale);
			finalRight *= rightMat;

			// �и��� ���� �� �ڽ��� ���̿� �ʺ�, ���� �� 6����
			Vector3 Axis[6] = {};
			Axis[0] = Vector3::Transform(arrCubeLocalPos[1], finalLeft);
			Axis[1] = Vector3::Transform(arrCubeLocalPos[3], finalLeft);
			Axis[2] = Vector3::Transform(arrCubeLocalPos[7], finalLeft);
			Axis[3] = Vector3::Transform(arrCubeLocalPos[1], finalRight);
			Axis[4] = Vector3::Transform(arrCubeLocalPos[3], finalRight);
			Axis[5] = Vector3::Transform(arrCubeLocalPos[7], finalRight);

			Axis[0] -= Vector3::Transform(arrCubeLocalPos[0], finalLeft);
			Axis[1] -= Vector3::Transform(arrCubeLocalPos[0], finalLeft);
			Axis[2] -= Vector3::Transform(arrCubeLocalPos[0], finalLeft);
			Axis[3] -= Vector3::Transform(arrCubeLocalPos[0], finalRight);
			Axis[4] -= Vector3::Transform(arrCubeLocalPos[0], finalRight);
			Axis[5] -= Vector3::Transform(arrCubeLocalPos[0], finalRight);


			Vector3 vc = left->GetPosition() - right->GetPosition();

			Vector3 centerDir = vc;
			for (size_t i = 0; i < 6; i++)
			{
				//�ϳ��� �и����� �������� ��´�.
				//�и����� ǥ��ȭ�ϰ�, �� �������Ϳ� ��� �и����� ������Ų ������ ��Į����� ���Ѵ�.
				//�� ��Į����� �հ� �߽��� �Ÿ��� ������ ������(������Ų ������ ��Į��)�� ���Ͽ� �ݸ��� ���� Ȯ��
				Vector3 vA = Axis[i];
				vA.Normalize();

				float projDist = 0.0f;
				for (size_t j = 0; j < 6; j++)
				{
					projDist += fabsf(Axis[j].Dot(vA) / 2.0f);
				}

				if (projDist < fabsf(centerDir.Dot(vA)))
				{
					return false;
				}
			}

			return true;
		}
		// ���� Circle vs Cirlce
		else
			return false;
	}

	RayHit CollisionManager::RayCast(GameObject* owner, Vector3 direction, std::vector<eLayerType> layers)
	{
		Scene* scene = SceneManager::GetActiveScene();
		Vector3 position = owner->GetComponent<Transform>()->GetPosition();
		ya::Ray ray = ya::Ray(position, direction);

		RayHit hit = RayHit(false, nullptr, Vector3::Zero);

		float distMin = 10000000;
		GameObject* colObj = nullptr;

		eLayerType colType = eLayerType::Player;
		for (eLayerType layer:layers )
		{
			DistAndObj ret = LayerRayCollision(scene, layer, ray, owner);
			if (ret.obj != nullptr && ret.dist < distMin)
			{
				distMin = ret.dist;
				colObj = ret.obj;
			}
		}
			
			

			
		
		
		if (colObj != nullptr)
		{
			hit.isHit = true;
			hit.hitObj = colObj;
			hit.contact = distMin * ray.direction + ray.position;
		}
		return hit;
	}
	//bool TestRayOBBIntersection(
	//	Vector3 ray_origin,        // Ray origin, in world space
	//	Vector3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
	//	Vector3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
	//	Vector3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
	//	glm::mat4 ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
	//	float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
	//) {
	DistAndObj CollisionManager::LayerRayCollision(Scene* scene, eLayerType objType, ya::Ray ray, GameObject* owner)
	{
		std::vector<GameObject*> objects = scene->GetGameObjects(objType);

		GameObject* colObj = nullptr;
		float distMin = 100000000;

		for (size_t i = 0; i < objects.size(); i++)
		{
			GameObject* obj = objects[i];
			Transform* tr = obj->GetComponent<Transform>();
			if (obj->GetState() != GameObject::Active)
				continue;
			if (obj->GetComponent<Collider2D>() == nullptr)
				continue;
			if (obj == owner)
				continue;
			
			Matrix worldMat = tr->GetWorldMatrix();

			Collider2D* collider = obj->GetComponent<Collider2D>();
			Vector3 colScale = Vector3(collider->GetSize().x, collider->GetSize().y, collider->GetSize().z);
			Matrix colMatrix = Matrix::CreateScale(colScale);
			worldMat *= colMatrix;

			float dist = RayIntersect(ray, obj);
			if (dist < 0)
				continue;
			
			if (dist < distMin)
				distMin = dist; colObj = obj;
		}
		return DistAndObj(distMin, colObj);

	}
	float CollisionManager::RayIntersect(ya::Ray ray, GameObject* colObj)
	{
		Vector3 aabb_min = Vector3(-0.5, -0.5, -0.5);
		Vector3 aabb_max = Vector3(0.5, 0.5, 0.5);

		Transform* tr = colObj->GetComponent<Transform>();
		Collider2D* col = colObj->GetComponent<Collider2D>();
		Matrix worldMatrix = tr->GetWorldMatrix();
		Vector3 scale = tr->GetScale();
		Vector3 colScale = col->GetSize();
		scale.x *= colScale.x;
		scale.y *= colScale.y;
		scale.z *= colScale.z;
		
		float tMin = 0.0f;
		float tMax = 100000.0f;
		float threshHold = 0.00000001;



		Vector3 boxWorldPosition(worldMatrix._41, worldMatrix._42, worldMatrix._43);


		//x
		{
			Vector3 xaxis = tr->Right();
			xaxis.Normalize();
				
			Vector3 xdeltaMax = boxWorldPosition + xaxis * scale.x / 2 - ray.position;
			Vector3 xdeltaMin = boxWorldPosition - xaxis * scale.x / 2 - ray.position;

			float e1 = xaxis.Dot(xdeltaMax);
			float e2 = xaxis.Dot(xdeltaMin);
			float f = ray.direction.Dot(xaxis);


			if (fabsf(f) > threshHold)
			{
				float t1 = e1 / f; // Intersection with the "left" plane
				float t2 = e2 / f; // Intersection with the "right" plane

				if (t1 > t2) { // if wrong order
					float w = t1;
					t1 = t2;
					t2 = w; // swap t1 and t2
				}

				if (t1< 0)
					t1 = 0;

				if (t1 < 0 && t2 < 0)
					return -1;
				// tMax �� ���� �������ִ� "��" ����
				if (t2 < tMax)
					tMax = t2;
				// tMin �� ���� �ָ��ִ� "�����" ����
				if (t1 > tMin)
					tMin = t1;

				if (tMax < tMin)
					return -1;
			}
			else
				return -1;
		}
		//y
		{
			Vector3 yaxis = tr->Up();
			yaxis.Normalize();

			Vector3 ydeltaMax = boxWorldPosition + yaxis * scale.y / 2 - ray.position;
			Vector3 ydeltaMin = boxWorldPosition - yaxis * scale.y / 2 - ray.position;

			float e1 = yaxis.Dot(ydeltaMax);
			float e2 = yaxis.Dot(ydeltaMin);

			float f = ray.direction.Dot(yaxis);


			if (fabsf(f) > threshHold)
			{
				float t1 = e1 / f; // Intersection with the "left" plane
				float t2 = e2 / f; // Intersection with the "right" plane

				if (t1 > t2) { // if wrong order
					float w = t1;
					t1 = t2;
					t2 = w; // swap t1 and t2
				}
				if (t1 < 0)
					t1 = 0;

				if (t1 < 0 && t2 < 0)
					return -1;
				// tMin �� ���� �������ִ� "��" ����
				if (t2 < tMax)
					tMax = t2;
				// tMin �� ���� �ָ��ִ� "�����" ����
				if (t1 > tMin)
					tMin = t1;

				if (tMax < tMin)
					return -1;
			}
			else
				return -1;
		}
		//z
		{
			Vector3 zaxis = tr->Forward();

			zaxis.Normalize();
				
			Vector3 zdeltaMax = boxWorldPosition + zaxis * scale.z / 2 - ray.position;
			Vector3 zdeltaMin = boxWorldPosition - zaxis * scale.z / 2 - ray.position;

			float e1 = zaxis.Dot(zdeltaMax);
			float e2 = zaxis.Dot(zdeltaMin);

			float f = ray.direction.Dot(zaxis);


			if (fabsf(f) > threshHold)
			{
				float t1 = e1 / f; // Intersection with the "left" plane
				float t2 = e2 / f; // Intersection with the "right" plane

				if (t1 > t2) { // if wrong order
					float w = t1;
					t1 = t2;
					t2 = w; // swap t1 and t2
				}
				if (t1 < 0)
					t1 = 0;

				if (t1 < 0 && t2 < 0)
					return -1;
				// tMin �� ���� �������ִ� "��" ����
				if (t2 < tMax)
					tMax = t2;
				// tMin �� ���� �ָ��ִ� "�����" ����
				if (t1 > tMin)
					tMin = t1;

				if (tMax < tMin)
					return-1;
			}

			else
				return -1;
		}
		return tMin;
	}
}
