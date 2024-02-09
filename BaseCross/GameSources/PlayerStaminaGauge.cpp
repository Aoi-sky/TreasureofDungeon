/*!
@file PlayerHpGauge.cpp
@brief �v���C���[��HP�o�[�̎���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void PlayerStaminaBar::OnCreate() {
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");// Player�I�u�W�F�N�g�̎擾

		// �Q�[�W�̐���
		m_bar[0] = ObjectFactory::Create<Gauge>(GetStage(), 400.0f, L"ORANGESTAMINABAR");
		m_bar[1] = ObjectFactory::Create<Gauge>(GetStage(), 400.0f, L"OCHERSTAMINABAR");

		// �Q�[�W�T�C�Y�̐ݒ�
		m_bar[0]->UpdateGaugeSize(player->GetMaxStamina(), player->GetCurrentStamina());
		m_bar[1]->UpdateGaugeSize(player->GetMaxStamina(), player->GetMaxStamina());

		// ��ʂ̉����ɕ\��
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
		// �Q�[�W�T�C�Y�̍X�V
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		if (!player->GetDrawActive() || player->GetCurrentLife() <= 0) {
			return;// �������~����
		}

		m_bar[0]->UpdateGaugeSize(player->GetMaxStamina(), player->GetCurrentStamina());
		m_bar[1]->UpdateGaugeSize(player->GetMaxStamina(), player->GetMaxStamina());
	}

	void PlayerStaminaBar::OnDraw() {
		// �Q�[�W�̕`��
		m_bar[1]->GameObject::OnDraw();
		m_bar[0]->GameObject::OnDraw();
	}
}