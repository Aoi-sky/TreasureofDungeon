#include "stdafx.h"
#include "Project.h"



namespace basecross {
	void Wave::OnCreate()
	{
		//弾の形
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");

		//向きをベクトルで所得
		auto ownerTrans = m_owner->GetComponent<Transform>();
		m_forward = ownerTrans->GetForward();

		//自分のトランスコンポーネントを所得して座標や大きさを設定
		m_transform = GetComponent<Transform>();
		m_transform->SetPosition(ownerTrans->GetPosition());
		m_transform->SetScale(Vec3(0.5f));
		//コリジョンをつける
		auto ptrColl = AddComponent<CollisionSphere>();
		//衝突判定
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetSleepActive(true);

		AddTag(L"Wave");
	}

	void Wave::OnUpdate()
	{
		auto& app = App::GetApp();
		//デルタタイムを取得
		float delta = app->GetElapsedTime();

		//移動処理
		auto pos = m_transform->GetPosition();//現在の座標を所得
		pos += m_forward * m_speed * delta;
		m_transform->SetPosition(pos);

	}
	void Wave::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"FixedCylinder"))
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			GetStage()->AddGameObject<FallingRocks>();
			return;
		}
		if (Other->FindTag(L"FixedBox"))
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
	}

}