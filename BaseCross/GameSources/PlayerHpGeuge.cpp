/*!
@file PlayerHpGauge.cpp
@brief �v���C���[��HP�o�[�̎���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void PlayerHpBar::OnCreate(){
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");// Player�I�u�W�F�N�g�̎擾

		// �Q�[�W�̐���
		m_bar[0] = ObjectFactory::Create<Gauge>(GetStage(), 400.0f, L"GREENHPBAR");
		m_bar[1] = ObjectFactory::Create<Gauge>(GetStage(), 400.0f, L"REDHPBAR");

		// �Q�[�W�T�C�Y�̐ݒ�
		m_bar[0]->UpdateGaugeSize(player->GetMaxLife(), player->GetCurrentLife());
		m_bar[1]->UpdateGaugeSize(player->GetMaxLife(), (float)player->GetMaxLife());

		// ��ʂ̍���ɕ\��
		m_bar[0]->SetPosition((float)App::GetApp()->GetGameWidth() / 2 + 200.0f, App::GetApp()->GetGameHeight()-100.0f, 0.49f);
		m_bar[1]->SetPosition((float)App::GetApp()->GetGameWidth() / 2 + 200.0f, App::GetApp()->GetGameHeight() - 100.0f, 0.49f);

		auto m_TexKey = L"GREENHPBAR";
		if (m_TexKey == L"GREENHPBAR"){
			SetDrawLayer((int)DrawLayer::ForeFront);
		}
		m_TexKey = L"REDHPBAR";
		if (m_TexKey == L"REDHPBAR") {
			SetDrawLayer((int)DrawLayer::Front);
		}
	}

	void PlayerHpBar::OnUpdate(){
		// �Q�[�W�T�C�Y�̍X�V
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		m_bar[0]->UpdateGaugeSize(player->GetMaxLife(), player->GetCurrentLife());
		m_bar[1]->UpdateGaugeSize(player->GetMaxLife(), (float)player->GetMaxLife());
	}

	void PlayerHpBar::OnDraw(){
		// �Q�[�W�̕`��
		m_bar[1]->GameObject::OnDraw();
		m_bar[0]->GameObject::OnDraw();
	}
}