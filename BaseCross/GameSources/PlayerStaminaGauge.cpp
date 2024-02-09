/*!
@file PlayerHpGauge.cpp
@brief プレイヤーのHPバーの実装
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void PlayerStaminaBar::OnCreate() {
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");// Playerオブジェクトの取得

		// ゲージの生成
		m_bar[0] = ObjectFactory::Create<Gauge>(GetStage(), 400.0f, L"ORANGESTAMINABAR");
		m_bar[1] = ObjectFactory::Create<Gauge>(GetStage(), 400.0f, L"OCHERSTAMINABAR");

		// ゲージサイズの設定
		m_bar[0]->UpdateGaugeSize(player->GetMaxStamina(), player->GetCurrentStamina());
		m_bar[1]->UpdateGaugeSize(player->GetMaxStamina(), player->GetMaxStamina());

		// 画面の下側に表示
		m_bar[0]->SetPosition((float)App::GetApp()->GetGameWidth() / 2 + 200.0f, App::GetApp()->GetGameHeight() - 100.0f, 0.49f);
		m_bar[1]->SetPosition((float)App::GetApp()->GetGameWidth() / 2 + 200.0f, App::GetApp()->GetGameHeight() - 100.0f, 0.49f);

		auto m_TexKey = L"ORANGESTAMINABAR";
		if (m_TexKey == L"ORANGESTAMINABAR") {
			SetDrawLayer((int)DrawLayer::ForeFront);
		}
		m_TexKey = L"OCHERSTAMINABAR";
		if (m_TexKey == L"OCHERSTAMINABAR") {
			SetDrawLayer((int)DrawLayer::Front);
		}
	}

	void PlayerStaminaBar::OnUpdate() {
		// ゲージサイズの更新
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		if (!player->GetDrawActive() || player->GetCurrentLife() <= 0) {
			return;// 処理を停止する
		}

		m_bar[0]->UpdateGaugeSize(player->GetMaxStamina(), player->GetCurrentStamina());
		m_bar[1]->UpdateGaugeSize(player->GetMaxStamina(), player->GetMaxStamina());
	}

	void PlayerStaminaBar::OnDraw() {
		// ゲージの描画
		m_bar[1]->GameObject::OnDraw();
		m_bar[0]->GameObject::OnDraw();
	}
}