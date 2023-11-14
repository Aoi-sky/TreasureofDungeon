#include "stdafx.h"
#include "Project.h"



namespace basecross {
	void Wave::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");//弾の形

		//向きをベクトルで所得
		auto ownerTrans = m_owner->GetComponent<Transform>();
		m_forward = ownerTrans->GetForward();
		m_transform = GetComponent<Transform>();//コンポーネントの取得
		m_transform->SetPosition(ownerTrans->GetPosition());//座標設定
		m_transform->SetScale(Vec3(1.0f,0.5f,1.0f));//サイズ調整
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

	}
	void Wave::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"FixedCylinder"))//柱
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			//GetStage()->AddGameObject<FallingRocks>();
			return;
		}
		if (Other->FindTag(L"FixedBox"))//壁
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		if (Other->FindTag(L"Enemy"))//敵
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