#include "stdafx.h"
#include "Project.h"

namespace basecross {
Enemy::Enemy(const shared_ptr<Stage>& StagePtr,
	const shared_ptr<StageCellMap>& CellMap,
	const Vec3& Position) :
	GameObject(StagePtr),
	m_CelMap(CellMap),
	m_StartPosition(Position),
	m_Force(0),
	m_Velocity(0)
{}
Enemy::~Enemy() {}
//èâä˙âª
void Enemy::OnCreate() {
	auto ptr = GetComponent<Transform>();
	ptr->SetScale(0.5f, 0.5f, 0.5f);
	ptr->SetRotation(0.0f, 0.0f, 0.0f);
	ptr->SetPosition(m_StartPosition);
	AddComponent<CollisionSphere>();
	auto MapPtr = m_CelMap.lock();
	if (MapPtr) {
		AddComponent<PathSearch>(MapPtr);

	}

}


}


