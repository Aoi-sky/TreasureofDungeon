/*!
@file PlayerHpGauge.cpp
@brief プレイヤーのHPバーの実装
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void GolemHpBar::OnCreate() {
		auto golem = GetStage()->GetSharedGameObject<Golem>(L"Golem");// golemオブジェクトの取得

		// ゲージの生成
		m_bar[0] = ObjectFactory::Create<Gauge>(GetStage(), 400.0f, L"GREENHPBAR");
		m_bar[1] = ObjectFactory::Create<Gauge>(GetStage(), 400.0f, L"REDHPBAR");

		// ゲージサイズの設定
		m_bar[0]->UpdateGaugeSize(golem->GetGolemMaxLife(), golem->GetGolemCurrentLife());
		m_bar[1]->UpdateGaugeSize(golem->GetGolemMaxLife(), (float)golem->GetGolemMaxLife());

		// 画面の左上に表示
		m_bar[0]->SetPosition((float)App::GetApp()->GetGameWidth() / 2 + 200, 0.0f, 0.49f);
		m_bar[1]->SetPosition((float)App::GetApp()->GetGameWidth() / 2 + 200, 0.0f, 0.49f);

		auto m_TexKey = L"GREENHPBAR";
		if (m_TexKey == L"GREENHPBAR"){
			SetDrawLayer((int)DrawLayer::ForeFront);
		}
		m_TexKey = L"REDHPBAR";
		if (m_TexKey == L"REDHPBAR"){
			SetDrawLayer((int)DrawLayer::Front);
		}
	}

	void GolemHpBar::OnUpdate(){
		// ゲージサイズの更新
		auto golem = GetStage()->GetSharedGameObject<Golem>(L"Golem");
		m_bar[0]->UpdateGaugeSize(golem->GetGolemMaxLife(), golem->GetGolemCurrentLife());
		m_bar[1]->UpdateGaugeSize(golem->GetGolemMaxLife(), (float)golem->GetGolemMaxLife());
	}

	void GolemHpBar::OnDraw(){
		// ゲージの描画
		m_bar[1]->GameObject::OnDraw();
		m_bar[0]->GameObject::OnDraw();
	}
}