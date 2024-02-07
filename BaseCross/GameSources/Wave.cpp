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
		m_transform->SetPosition(ownerTrans->GetPosition() + m_forward * m_speed);//座標設定
		m_transform->SetScale(Vec3(1.5f,5.0f,1.5f));//サイズ調整
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンをつける
		//衝突判定
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetSleepActive(true);
		AddTag(L"Wave");//タグの設定

		//エフェクトの初期化
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		auto ShEfkInterface = App::GetApp()->GetScene<Scene>()->GetEfkInterface();
		m_damageEffectStr = DataDir + L"Effects\\" + L"damage.efk";
		m_damageEffect = ObjectFactory::Create<EfkEffect>(ShEfkInterface, m_damageEffectStr);


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

		const auto& stage = GetStage();

		if (Other->FindTag(L"FixedCylinder"))//柱
		{
			stage->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		if (Other->FindTag(L"Wall"))//壁
		{
			stage->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		if (Other->FindTag(L"Golem"))//敵
		{
			auto pos = m_transform->GetPosition();
			m_DamageEfkPlay = ObjectFactory::Create<EfkPlay>(m_damageEffect, pos, Vec3(0.1f));

			const auto& player = stage->GetSharedGameObject<Player>(L"Player");

			stage->RemoveGameObject<Wave>(GetThis<Wave>());
			stage->GetSharedGameObject<Golem>(L"Golem")->AddDamage(player->GetOffensiveAbility());
			return;
		}
		if (Other->FindTag(L"ShotEnemy"))//敵
		{
			auto pos = m_transform->GetPosition();
			m_DamageEfkPlay = ObjectFactory::Create<EfkPlay>(m_damageEffect, pos, Vec3(5.0f));
			stage->RemoveGameObject<Wave>(GetThis<Wave>());

			return;
		}
		if (Other->FindTag(L"FallingRocks"))//落石
		{
			stage->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		else {
			stage->RemoveGameObject<Wave>(GetThis<Wave>());
			return;
		}
		
	}

}