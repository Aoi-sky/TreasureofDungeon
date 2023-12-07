/*!
@file ShotEnemy.cpp
@brief 球を飛ばしてくる敵
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	ShotEnemy::ShotEnemy(const shared_ptr<Stage>& stage):
		GameObject(stage)

	{
	}
	ShotEnemy::~ShotEnemy() {}

	void ShotEnemy::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");

		m_transform = GetComponent<Transform>();//コンポーネントの取得
		m_transform->SetPosition(Vec3(1.0f, 5.0f, 1.0f));//座標設定
		m_transform->SetScale(Vec3(1.0f, 2.0f, 1.0f));//サイズ調整


		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetSleepActive(true);


		AddTag(L"ShotEnemy");//タグの設定

	}

	void ShotEnemy::OnUpdate() {

	}
}