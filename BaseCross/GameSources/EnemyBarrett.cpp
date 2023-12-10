#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void EnemyBullet::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");//弾の形

		//向きをベクトルで所得
		auto ownerTrans = m_owner->GetComponent<Transform>();
		m_transform = GetComponent<Transform>();//コンポーネントの取得
		m_transform->SetPosition(ownerTrans->GetPosition());//座標設定
		m_transform->SetScale(Vec3(0.5f, 0.5f, 0.5f));//サイズ調整
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンをつける
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetSleepActive(true);
		AddTag(L"EnemyBullet");//タグの設定

		MoveEnemyBullet();
	}

	void EnemyBullet::OnUpdate() {
		float delta = App::GetApp()->GetElapsedTime();//デルタタイムを取得

		//移動処理
		auto pos = m_transform->GetPosition();
		pos += m_forward * m_speed * delta;
		m_transform->SetPosition(pos.x,1.0f,pos.z);
	}
	void EnemyBullet::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"FixedCylinder"))//柱
		{
			GetStage()->RemoveGameObject<EnemyBullet>(GetThis<EnemyBullet>());
			return;
		}
		if (Other->FindTag(L"Wall")|| Other->FindTag(L"Floor"))//壁・床
		{
			GetStage()->RemoveGameObject<EnemyBullet>(GetThis<EnemyBullet>());
			return;
		}
		if (Other->FindTag(L"Enemy"))//敵
		{
			GetStage()->RemoveGameObject<EnemyBullet>(GetThis<EnemyBullet>());
			return;
		}
		if (Other->FindTag(L"FallingRocks"))//落石
		{
			GetStage()->RemoveGameObject<EnemyBullet>(GetThis<EnemyBullet>());
			return;
		}
		if (Other->FindTag(L"Player"))//プレイヤー
		{
			GetStage()->RemoveGameObject<EnemyBullet>(GetThis<EnemyBullet>());
			GetStage()->GetSharedGameObject<Player>(L"Player")->AddPlayerDamage(m_damege);
			return;
		}
	}

	void EnemyBullet::MoveEnemyBullet()
	{
		// デルタタイムの取得
		auto& app = App::GetApp();
		float delta = app->GetElapsedTime();

		// ベクトルの取得
		auto playerTrans = GetStage()->GetSharedGameObject<Player>(L"Player")->GetComponent<Transform>();
		auto playerPos = playerTrans->GetPosition(); // プレイヤーの位置を取得
		auto pos = m_transform->GetPosition();
		
		m_forward = playerPos - pos; // プレイヤーとの距離を計算

		// ベクトルの正規化処理
		float normalizeMagnification = 1 / sqrt(
			m_forward.x * m_forward.x +
			m_forward.y * m_forward.y +
			m_forward.z * m_forward.z);
		m_forward *= normalizeMagnification;

		float rotationY = atan2f(-m_forward.z, m_forward.x); // 回転の計算
		m_transform->SetRotation(Vec3(0, rotationY, 0)); // 回転処理
	}
}