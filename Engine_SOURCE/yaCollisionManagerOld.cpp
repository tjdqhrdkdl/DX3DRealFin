//#include "yaCollisionManager.h"
//#include "yaScene.h"
//#include "yaSceneManager.h"
//#include "PhysXManager.h"
//#include "yaTime.h"
//
//namespace ya
//{
//	std::bitset<(UINT)eLayerType::End> CollisionManager::mLayerCollisionMatrix[(UINT)eLayerType::End] = {};
//	std::map<UINT64, bool> CollisionManager::mCollisionMap;
//
//	void CollisionManager::Initialize()
//	{
//		if (false == CollisionManager::initialize())
//		{
//			assert(false);
//		}
//	}
//	void CollisionManager::Update()
//	{
//		CollisionManager::update(Time::DeltaTime());
//
//		Scene* scene = SceneManager::GetActiveScene();
//		for (UINT row = 0; row < (UINT)eLayerType::End; row++)
//		{
//			for (UINT column = 0; column < (UINT)eLayerType::End; column++)
//			{
//				if (mLayerCollisionMatrix[row][column])
//				{
//					LayerCollision(scene, (eLayerType)row, (eLayerType)column);
//				}
//			}
//		}
//	}
//	void CollisionManager::FixedUpdate()
//	{
//	}
//	void CollisionManager::Render()
//	{
//	}
//	void CollisionManager::CollisionLayerCheck(eLayerType left, eLayerType right, bool enable)
//	{
//		int row = 0;
//		int column = 0;
//
//		if ((UINT)left <= (UINT)right)
//		{
//			row = (UINT)left;
//			column = (UINT)right;
//		}
//		else
//		{
//			row = (UINT)right;
//			column = (UINT)left;
//		}
//
//		mLayerCollisionMatrix[row][column] = enable;
//	}
//	void CollisionManager::LayerCollision(Scene* scene, eLayerType left, eLayerType right)
//	{
//		const std::vector<GameObject*>& lefts = scene->GetGameObjects(left);
//		const std::vector<GameObject*>& rights = scene->GetGameObjects(right);
//
//		//동일 레이어 내부 충돌 검사일 경우
//		if (left == right)
//		{
//			for (size_t i = 0; i < lefts.size(); i++)
//			{
//				GameObject* leftObj = lefts[i];
//
//				{
//					if (leftObj->GetState() != GameObject::Active)
//						continue;
//					if (leftObj->GetComponent<Collider3D>() == nullptr)
//						continue;
//					if (!leftObj->GetComponent<Collider3D>()->IsActive())
//						continue;
//
//					for (size_t k = i; k < rights.size(); k++)
//					{
//						GameObject* rightObj = rights[k];
//
//						if (rightObj->GetState() != GameObject::Active)
//							continue;
//						if (rightObj->GetComponent<Collider3D>() == nullptr)
//							continue;
//						if (!rightObj->GetComponent<Collider3D>()->IsActive())
//							continue;
//						if (leftObj == rightObj)
//							continue;
//
//						ColliderCollision(leftObj->GetComponent<Collider3D>(), rightObj->GetComponent<Collider3D>());
//					}
//
//					//if (leftObj == rightObj)
//					//	break;
//				}
//			}
//		}
//
//		else
//		{
//			for (GameObject* leftObj : lefts)
//			{
//				if (leftObj->GetState() != GameObject::Active)
//					continue;
//				if (leftObj->GetComponent<Collider3D>() == nullptr)
//					continue;
//				if (!leftObj->GetComponent<Collider3D>()->IsActive())
//					continue;
//
//				for (GameObject* rightObj : rights)
//				{
//					if (rightObj->GetState() != GameObject::Active)
//						continue;
//					if (rightObj->GetComponent<Collider3D>() == nullptr)
//						continue;
//					if (!rightObj->GetComponent<Collider3D>()->IsActive())
//						continue;
//					if (leftObj == rightObj)
//						continue;
//					if (!rightObj->GetComponent<Collider3D>()->IsActive())
//						continue;
//
//					ColliderCollision(leftObj->GetComponent<Collider3D>(), rightObj->GetComponent<Collider3D>());
//				}
//
//				//if (reinterpret_cast<UINT>(left)== (UINT)right)
//				//	break;
//			}
//		}
//	}
//
//	void CollisionManager::ColliderCollision(Collider3D* left, Collider3D* right)
//	{
//		// 두 충돌체 레이어로 구성된 ID 확인
//		ColliderID colliderID;
//		colliderID.left = (UINT)left->GetID();
//		colliderID.right = (UINT)right->GetID();
//
//		// 이전 충돌 정보를 검색한다.
//		// 만약에 충돌정보가 없는 상태라면
//		// 충돌정보를 생성해준다.
//		std::map<UINT64, bool>::iterator iter = mCollisionMap.find(colliderID.id);
//		if (iter == mCollisionMap.end())
//		{
//			mCollisionMap.insert(std::make_pair(colliderID.id, false));
//			iter = mCollisionMap.find(colliderID.id);
//		}
//
//		// 충돌체크를 해준다.
//		if (Intersect(left, right)) // 충돌을 한 상태
//		{
//			// 최초 충돌중 Enter
//			if (iter->second == false)
//			{
//				if (left->IsTriiger())
//					left->OnTriggerEnter(right);
//				else
//					left->OnCollisionEnter(right);
//
//				if (right->IsTriiger())
//					right->OnTriggerEnter(left);
//				else
//					right->OnCollisionEnter(left);
//
//				iter->second = true;
//			}
//			else // 충돌 중이지 않은 상태 Enter
//			{
//				if (false == left->IsTriiger())
//				{
//					left->OnCollisionStay(right);
//				}
//
//				if (false == right->IsTriiger())
//				{
//					right->OnCollisionStay(left);
//				}
//			}
//		}
//		else // 충돌하지 않은상태
//		{
//			// 충돌 중인상태 Exit
//			if (iter->second == true)
//			{
//				if (left->IsTriiger())
//					left->OnTriggerExit(right);
//				else
//					left->OnCollisionExit(right);
//
//				if (right->IsTriiger())
//					right->OnTriggerExit(left);
//				else
//					right->OnCollisionExit(left);
//
//				iter->second = false;
//			}
//		}
//	}
//
//	bool CollisionManager::Intersect(Collider3D* left, Collider3D* right)
//	{
//		// Rect vs Rect 
//		// 0 --- 1
//		// |     |
//		// 3 --- 2
//		Vector3 arrLocalPos[4] =
//		{
//			Vector3{-0.5f, 0.5f, 0.0f}
//			,Vector3{0.5f, 0.5f, 0.0f}
//			,Vector3{0.5f, -0.5f, 0.0f}
//			,Vector3{-0.5f, -0.5f, 0.0f}
//		};
//
//		Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
//		Transform* rightTr = right->GetOwner()->GetComponent<Transform>();
//
//		Matrix leftMat = leftTr->GetWorldMatrix();
//		Matrix rightMat = rightTr->GetWorldMatrix();
//
//		Vector3 leftPos = Vector3(leftMat._41, leftMat._42, leftMat._43);
//		leftPos += left->GetCenter();
//		Vector3 rightPos = Vector3(rightMat._41, rightMat._42, rightMat._43);
//		rightPos += right->GetCenter();
//
//		if (left->GetCollider3DType() == eColliderType::Rect
//			&& right->GetCollider3DType() == eColliderType::Rect
//			)
//		{	// 분리축 벡터 4개 구하기
//			Vector3 Axis[4] = {};
//
//			Vector3 leftScale = Vector3(left->GetSize().x, left->GetSize().y, 1.0f);
//			Matrix finalLeft = Matrix::CreateScale(leftScale);
//			finalLeft *= leftMat;
//
//			Vector3 rightScale = Vector3(right->GetSize().x, right->GetSize().y, 1.0f);
//			Matrix finalRight = Matrix::CreateScale(rightScale);
//			finalRight *= rightMat;
//
//			Axis[0] = Vector3::Transform(arrLocalPos[1], finalLeft);
//			Axis[1] = Vector3::Transform(arrLocalPos[3], finalLeft);
//			Axis[2] = Vector3::Transform(arrLocalPos[1], finalRight);
//			Axis[3] = Vector3::Transform(arrLocalPos[3], finalRight);
//
//			Axis[0] -= Vector3::Transform(arrLocalPos[0], finalLeft);
//			Axis[1] -= Vector3::Transform(arrLocalPos[0], finalLeft);
//			Axis[2] -= Vector3::Transform(arrLocalPos[0], finalRight);
//			Axis[3] -= Vector3::Transform(arrLocalPos[0], finalRight);
//
//			for (size_t i = 0; i < 4; i++)
//				Axis[i].z = 0.0f;
//
//			Vector3 vc = leftPos - rightPos;
//			vc.z = 0.0f;
//
//			Vector3 centerDir = vc;
//			for (size_t i = 0; i < 4; i++)
//			{
//				Vector3 vA = Axis[i];
//				//vA.Normalize();
//
//				float projDist = 0.0f;
//				for (size_t j = 0; j < 4; j++)
//				{
//					projDist += fabsf(Axis[j].Dot(vA) / 2.0f);
//				}
//
//				if (projDist < fabsf(centerDir.Dot(vA)))
//				{
//					return false;
//				}
//			}
//			return true;
//		}
//
//		else if (left->GetCollider3DType() == eColliderType::Box
//			&& right->GetCollider3DType() == eColliderType::Box
//			)
//		{
//			Vector3 arrCubeLocalPos[24] =
//			{
//				//윗면
//				arrCubeLocalPos[0] = Vector3(-0.5f, 0.5f, 0.5f) ,
//				arrCubeLocalPos[1] = Vector3(0.5f, 0.5f, 0.5f),
//				arrCubeLocalPos[2] = Vector3(0.5f, 0.5f, -0.5f),
//				arrCubeLocalPos[3] = Vector3(-0.5f, 0.5f, -0.5f),
//				//아래					 3
//				arrCubeLocalPos[4] = Vector3(-0.5f, -0.5f, -0.5f),
//				arrCubeLocalPos[5] = Vector3(0.5f, -0.5f, -0.5f),
//				arrCubeLocalPos[6] = Vector3(0.5f, -0.5f, 0.5f),
//				arrCubeLocalPos[7] = Vector3(-0.5f, -0.5f, 0.5f),
//				//왼						  3
//				arrCubeLocalPos[8] = Vector3(-0.5f, 0.5f, 0.5f),
//				arrCubeLocalPos[9] = Vector3(-0.5f, 0.5f, -0.5f),
//				arrCubeLocalPos[10] = Vector3(-0.5f, -0.5f, -0.5f),
//				arrCubeLocalPos[11] = Vector3(-0.5f, -0.5f, 0.5f),
//				//오						   
//				arrCubeLocalPos[12] = Vector3(0.5f, 0.5f, -0.5f),
//				arrCubeLocalPos[13] = Vector3(0.5f, 0.5f, 0.5f),
//				arrCubeLocalPos[14] = Vector3(0.5f, -0.5f, 0.5f),
//				arrCubeLocalPos[15] = Vector3(0.5f, -0.5f, -0.5f),
//				//뒤						   
//				arrCubeLocalPos[16] = Vector3(0.5f, 0.5f, 0.5f),
//				arrCubeLocalPos[17] = Vector3(-0.5f, 0.5f, 0.5f),
//				arrCubeLocalPos[18] = Vector3(-0.5f, -0.5f, 0.5f),
//				arrCubeLocalPos[19] = Vector3(0.5f, -0.5f, 0.5f),
//				//앞						   
//				arrCubeLocalPos[20] = Vector3(-0.5f, 0.5f, -0.5f),
//				arrCubeLocalPos[21] = Vector3(0.5f, 0.5f, -0.5f),
//				arrCubeLocalPos[22] = Vector3(0.5f, -0.5f, -0.5f),
//				arrCubeLocalPos[23] = Vector3(-0.5f, -0.5f, -0.5f)
//			};
//
//			Vector3 leftScale = Vector3(left->GetSize().x, left->GetSize().y, left->GetSize().z);
//			Matrix finalLeft = Matrix::CreateScale(leftScale);
//			finalLeft *= leftMat;
//
//			Vector3 rightScale = Vector3(right->GetSize().x, right->GetSize().y, right->GetSize().z);
//			Matrix finalRight = Matrix::CreateScale(rightScale);
//			finalRight *= rightMat;
//
//			// 분리축 벡터 각 박스의 높이와 너비, 깊이 총 6가지
//			Vector3 Axis[6] = {};
//			Axis[0] = Vector3::Transform(arrCubeLocalPos[1], finalLeft);
//			Axis[1] = Vector3::Transform(arrCubeLocalPos[3], finalLeft);
//			Axis[2] = Vector3::Transform(arrCubeLocalPos[7], finalLeft);
//			Axis[3] = Vector3::Transform(arrCubeLocalPos[1], finalRight);
//			Axis[4] = Vector3::Transform(arrCubeLocalPos[3], finalRight);
//			Axis[5] = Vector3::Transform(arrCubeLocalPos[7], finalRight);
//
//			Axis[0] -= Vector3::Transform(arrCubeLocalPos[0], finalLeft);
//			Axis[1] -= Vector3::Transform(arrCubeLocalPos[0], finalLeft);
//			Axis[2] -= Vector3::Transform(arrCubeLocalPos[0], finalLeft);
//			Axis[3] -= Vector3::Transform(arrCubeLocalPos[0], finalRight);
//			Axis[4] -= Vector3::Transform(arrCubeLocalPos[0], finalRight);
//			Axis[5] -= Vector3::Transform(arrCubeLocalPos[0], finalRight);
//
//
//			Vector3 vc = leftPos - rightPos;
//
//			Vector3 centerDir = vc;
//			for (size_t i = 0; i < 6; i++)
//			{
//				//하나의 분리축을 기준으로 삼는다.
//				//분리축을 표준화하고, 이 단위벡터에 모든 분리축을 투영시킨 벡터의 스칼라들을 구한다.
//				//이 스칼라들의 합과 중심축 거리차 벡터의 내적값(투영시킨 벡터의 스칼라)을 비교하여 콜리전 여부 확인
//				Vector3 vA = Axis[i];
//				vA.Normalize();
//
//				float projDist = 0.0f;
//				for (size_t j = 0; j < 6; j++)
//				{
//					projDist += fabsf(Axis[j].Dot(vA) / 2.0f);
//				}
//
//				if (projDist < fabsf(centerDir.Dot(vA)))
//				{
//					return false;
//				}
//			}
//
//			return true;
//		}
//		// 숙제 Circle vs Cirlce
//		else
//			return false;
//	}
//
//	RayHit CollisionManager::RayCast(GameObject* owner, Vector3 direction, std::vector<eLayerType> layers)
//	{
//		//Scene* scene = SceneManager::GetActiveScene();
//		Scene* scene = owner->GetScene();
//		Matrix worldMat = owner->GetComponent<Transform>()->GetWorldMatrix();
//		Vector3 position = Vector3(worldMat._41, worldMat._42, worldMat._43);
//		ya::Ray ray = ya::Ray(position, direction);
//
//		RayHit hit = RayHit(false, nullptr, Vector3::Zero);
//
//		float distMin = 10000000;
//		GameObject* colObj = nullptr;
//
//		eLayerType colType = owner->GetLayerType();
//		for (eLayerType layer : layers)
//		{
//			DistAndObj ret = LayerRayCollision(scene, layer, ray, owner);
//			if (ret.obj != nullptr && ret.dist < distMin)
//			{
//				distMin = ret.dist;
//				colObj = ret.obj;
//			}
//		}
//
//
//		if (colObj != nullptr)
//		{
//			hit.isHit = true;
//			hit.hitObj = colObj;
//			hit.contact = distMin * ray.direction + ray.position;
//			hit.length = distMin;
//
//		}
//		return hit;
//	}
//
//	RayHit CollisionManager::RayCast(GameObject* owner, Vector3 position, Vector3 direction, float length, std::vector<eLayerType> layers)
//	{
//		//Scene* scene = SceneManager::GetActiveScene();
//		Scene* scene = owner->GetScene();
//		ya::Ray ray = ya::Ray(position, direction);
//
//		RayHit hit = RayHit(false, nullptr, Vector3::Zero);
//
//		float distMin = 10000000;
//		GameObject* colObj = nullptr;
//
//		eLayerType colType = owner->GetLayerType();
//		for (eLayerType layer : layers)
//		{
//			DistAndObj ret = LayerRayCollision(scene, layer, ray, owner);
//			if (ret.obj != nullptr && ret.dist < distMin)
//			{
//				distMin = ret.dist;
//				colObj = ret.obj;
//			}
//		}
//
//
//		if (colObj != nullptr && length > distMin)
//		{
//			hit.isHit = true;
//			hit.hitObj = colObj;
//			hit.contact = distMin * ray.direction + ray.position;
//			hit.length = distMin;
//		}
//		return hit;
//
//	}
//
//
//
//	RayHit CollisionManager::RayCast(GameObject* owner, Vector3 position, Vector3 direction, std::vector<eLayerType> layers)
//	{
//		//Scene* scene = SceneManager::GetActiveScene();
//		Scene* scene = owner->GetScene();
//		ya::Ray ray = ya::Ray(position, direction);
//
//		RayHit hit = RayHit(false, nullptr, Vector3::Zero);
//
//		float distMin = 10000000;
//		GameObject* colObj = nullptr;
//
//		eLayerType colType = owner->GetLayerType();
//		for (eLayerType layer:layers )
//		{
//			DistAndObj ret = LayerRayCollision(scene, layer, ray, owner);
//			if (ret.obj != nullptr && ret.dist < distMin)
//			{
//				distMin = ret.dist;
//				colObj = ret.obj;
//			}
//		}
//
//
//		if (colObj != nullptr)
//		{
//			hit.isHit = true;
//			hit.hitObj = colObj;
//			hit.contact = distMin * ray.direction + ray.position;
//			hit.length = distMin;
//		}
//		return hit;
//	}
//
//	DistAndObj CollisionManager::LayerRayCollision(Scene* scene, eLayerType objType, ya::Ray ray, GameObject* owner)
//	{
//		std::vector<GameObject*> objects = scene->GetGameObjects(objType);
//
//		GameObject* colObj = nullptr;
//		float distMin = 100000000;
//
//		for (size_t i = 0; i < objects.size(); i++)
//		{
//			GameObject* obj = objects[i];
//			Transform* tr = obj->GetComponent<Transform>();
//			if (obj->GetState() != GameObject::Active)
//				continue;
//			if (obj->GetComponent<Collider3D>() == nullptr)
//				continue;
//			if (obj == owner)
//				continue;
//
//
//			float dist = RayIntersect(ray, obj);
//			if (dist < 0)
//				continue;
//
//			if (dist < distMin)
//				distMin = dist; colObj = obj;
//		}
//		return DistAndObj(distMin, colObj);
//
//	}
//	float CollisionManager::RayIntersect(ya::Ray ray, GameObject* colObj)
//	{
//
//		Transform* tr = colObj->GetComponent<Transform>();
//		Collider3D* col = colObj->GetComponent<Collider3D>();
//		Matrix worldMatrix = tr->GetWorldMatrix();
//		Vector3 scale = tr->GetWorldScale();
//		Vector3 colScale = col->GetSize();
//		scale.x *= colScale.x;
//		scale.y *= colScale.y;
//		scale.z *= colScale.z;
//
//		float tMin = 0.0f;
//		float tMax = 100000.0f;
//		float threshHold = 0.00000001f;
//
//
//		Vector3 colCenter = col->GetCenter();
//		Vector3 boxWorldPosition(worldMatrix._41 , worldMatrix._42, worldMatrix._43);
//		boxWorldPosition += colCenter;
//
//
//		//x
//		{
//			Vector3 xaxis = tr->Right();
//			xaxis.Normalize();
//
//			Vector3 xdeltaMax = boxWorldPosition + xaxis * scale.x / 2 - ray.position;
//			Vector3 xdeltaMin = boxWorldPosition - xaxis * scale.x / 2 - ray.position;
//
//			float e1 = xaxis.Dot(xdeltaMax);
//			float e2 = xaxis.Dot(xdeltaMin);
//			float f = ray.direction.Dot(xaxis);
//
//
//			if (fabsf(f) > threshHold)
//			{
//				float t1 = e1 / f; // Intersection with the "left" plane
//				float t2 = e2 / f; // Intersection with the "right" plane
//
//				if (t1 > t2) { // if wrong order
//					float w = t1;
//					t1 = t2;
//					t2 = w; // swap t1 and t2
//				}
//
//				// tMax 은 가장 가까이있는 "먼" 교차
//				if (t2 < tMax)
//					tMax = t2;
//				// tMin 은 가장 멀리있는 "가까운" 교차
//				if (t1 > tMin)
//					tMin = t1;
//
//				if (tMax < tMin)
//					return -1;
//			}
//			else
//			{
//				if (e2 > e1) { // if wrong order
//					float w = e1;
//					e1 = e2;
//					e2 = w; // swap t1 and t2
//				}
//				if (e1 <0 ||e2 >0 )
//					return -1;
//
//			}
//		}
//		//y
//		{
//			Vector3 yaxis = tr->Up();
//			yaxis.Normalize();
//
//			Vector3 ydeltaMax = boxWorldPosition + yaxis * scale.y / 2 - ray.position;
//			Vector3 ydeltaMin = boxWorldPosition - yaxis * scale.y / 2 - ray.position;
//
//			float e1 = yaxis.Dot(ydeltaMax);
//			float e2 = yaxis.Dot(ydeltaMin);
//
//			float f = ray.direction.Dot(yaxis);
//
//
//			if (fabsf(f) > threshHold)
//			{
//				float t1 = e1 / f; // Intersection with the "left" plane
//				float t2 = e2 / f; // Intersection with the "right" plane
//
//				if (t1 > t2) { // if wrong order
//					float w = t1;
//					t1 = t2;
//					t2 = w; // swap t1 and t2
//				}
//
//				// tMin 은 가장 가까이있는 "먼" 교차
//				if (t2 < tMax)
//					tMax = t2;
//				// tMin 은 가장 멀리있는 "가까운" 교차
//				if (t1 > tMin)
//					tMin = t1;
//
//				if (tMax < tMin)
//					return -1;
//			}
//			else
//			{
//				if (e2 > e1) { // if wrong order
//					float w = e1;
//					e1 = e2;
//					e2 = w; // swap t1 and t2
//				}
//				if (e1 < 0 || e2 > 0)
//					return -1;
//			}
//		}
//		//z
//		{
//			Vector3 zaxis = tr->Forward();
//
//			zaxis.Normalize();
//
//			Vector3 zdeltaMax = boxWorldPosition + zaxis * scale.z / 2 - ray.position;
//			Vector3 zdeltaMin = boxWorldPosition - zaxis * scale.z / 2 - ray.position;
//
//			float e1 = zaxis.Dot(zdeltaMax);
//			float e2 = zaxis.Dot(zdeltaMin);
//
//			float f = ray.direction.Dot(zaxis);
//
//
//			if (fabsf(f) > threshHold)
//			{
//				float t1 = e1 / f; // Intersection with the "left" plane
//				float t2 = e2 / f; // Intersection with the "right" plane
//
//				if (t1 > t2) { // if wrong order
//					float w = t1;
//					t1 = t2;
//					t2 = w; // swap t1 and t2
//				}
//
//				// tMin 은 가장 가까이있는 "먼" 교차
//				if (t2 < tMax)
//					tMax = t2;
//				// tMin 은 가장 멀리있는 "가까운" 교차
//
//				if (t1 > tMin)
//					tMin = t1;
//
//				if (tMax < tMin)
//					return-1;
//			}
//
//			else
//			{
//				if (e2 > e1) { // if wrong order
//					float w = e1;
//					e1 = e2;
//					e2 = w; // swap t1 and t2
//				}
//				if (e1 < 0 || e2 > 0)
//					return -1;
//			}
//		}
//		return tMin;
//	}
//}
