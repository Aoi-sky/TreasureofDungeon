#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void Wave::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");//弾の形
		drawComp->SetDrawActive(true);

		//向きをベクトルで所得
		auto ownerTrans = m_owner->GetComponent<Transform>();
		m_forward = ownerTrans->GetForward();
		m_transform = GetComponent<Transform>();//コンポーネントの取得
		m_transform->SetPosition(ownerTrans->GetPosition() + m_forward * m_speed);//座標設定
		m_transform->SetScale(Vec3(1.5f,5.0f,1.5f));//サイズ調整
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンをつける
		//衝突判定
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetSleepActive(true);
		AddTag(L"Wave");//タグの設定
	}

	void Wave::OnUpdate() {
		auto& app = App::GetApp();
		float delta = app->GetElapsedTime();//デルタタイムを取得

		//移動処理
		auto pos = m_transform->GetPosition();
		pos += m_forward * m_speed * delta;
		m_transform->SetPosition(pos.x, 1,pos.z);

		GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());

		//m_time += delta * 0.1f;
		//if (m_time >= 1.0f)
		//{
		//	GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
		//	m_time = 0;
		//}

	}
	void Wave::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"FixedCylinder"))//柱
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		if (Other->FindTag(L"Wall"))//壁
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		if (Other->FindTag(L"Enemy"))//敵
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		if (Other->FindTag(L"Golem"))//敵
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			GetStage()->GetSharedGameObject<Golem>(L"Golem")->AddDamage(70);
			return;
		}
		if (Other->FindTag(L"ShotEnemy"))//敵
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		if (Other->FindTag(L"FallingRocks"))//落石
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		
	}

}