/*!
@file ShotEnemy.cpp
@brief 弾を飛ばしてくる敵
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	ShotEnemy::ShotEnemy(const shared_ptr<Stage>& stage, const Vec3& Position):
		GameObject(stage),
		m_positone(Position),
		m_RecastCount(120),
		m_recastFlame(0)
	{
	}
	ShotEnemy::~ShotEnemy() {}

	void ShotEnemy::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		//drawComp->SetMeshResource(L"DEFAULT_SPHERE");
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		m_transform = GetComponent<Transform>();//コンポーネントの取得
		m_transform->SetPosition(m_positone);//座標設定
		m_transform->SetRotation(Vec3(90,90,90));//角度設定
		m_transform->SetScale(Vec3(1.2f, 1.2f, 1.2f));//サイズ調整

		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetAfterCollision(AfterCollision::None);
		AddTag(L"ShotEnemy");//タグの設定
	}

	void ShotEnemy::OnUpdate() {

		m_recastFlame--;

		if (m_recastFlame <= 0)
		{
			GetStage()->AddGameObject<EnemyBullet>(GetThis<ShotEnemy>());

			m_recastFlame = m_RecastCount;
		}

	}

	void ShotEnemy::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Wave"))//柱
		{
			GetStage()->RemoveGameObject<ShotEnemy>(GetThis<ShotEnemy>());
			return;
		}
	}
}