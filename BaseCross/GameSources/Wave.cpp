#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void Wave::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");//弾の形
		drawComp->SetDrawActive(false);

		//向きをベクトルで所得
		auto ownerTrans = m_owner->GetComponent<Transform>();
		m_forward = ownerTrans->GetForward();
		m_transform = GetComponent<Transform>();//コンポーネントの取得
		m_transform->SetPosition(ownerTrans->GetPosition());//座標設定
		m_transform->SetScale(Vec3(1.0f,5.0f,1.0f));//サイズ調整
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンをつける
		//衝突判定
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetSleepActive(true);
		AddTag(L"Wave");//タグの設定

		//エフェクトの初期化
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		auto ShEfkInterface = App::GetApp()->GetScene<Scene>()->GetEfkInterface();
		m_sparkEffectStr = DataDir + L"Effects\\" + L"damege.efk";
		m_sparkEffect = ObjectFactory::Create<EfkEffect>(ShEfkInterface, m_sparkEffectStr);


	}

	void Wave::OnUpdate() {
		auto& app = App::GetApp();
		float delta = app->GetElapsedTime();//デルタタイムを取得

		//移動処理
		auto pos = m_transform->GetPosition();
		pos += m_forward * m_speed * delta;
		m_transform->SetPosition(pos.x, 1,pos.z);

		m_time += delta;
		if (m_time <= 10.0f)
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			m_time = 0;
		}

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
		if (Other->FindTag(L"Golem"))//敵
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			GetStage()->GetSharedGameObject<Golem>(L"Golem")->AddDamage(100);

			return;
		}
		if (Other->FindTag(L"ShotEnemy"))//敵
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			auto pos = m_transform->GetPosition();
			m_EfkPlay = ObjectFactory::Create<EfkPlay>(m_sparkEffect, pos, Vec3(1.0f));
			return;
		}
		if (Other->FindTag(L"FallingRocks"))//落石
		{
			GetStage()->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		
	}

}