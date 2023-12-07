/*!
@file PlayerHpGauge.cpp
@brief �v���C���[��HP�o�[�̎���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void GolemHpBar::OnCreate() {
		auto golem = GetStage()->GetSharedGameObject<Golem>(L"Golem");// golem�I�u�W�F�N�g�̎擾

		// �Q�[�W�̐���
		m_bar[0] = ObjectFactory::Create<Gauge>(GetStage(), 400.0f, L"GREENHPBAR");
		m_bar[1] = ObjectFactory::Create<Gauge>(GetStage(), 400.0f, L"REDHPBAR");

		// �Q�[�W�T�C�Y�̐ݒ�
		m_bar[0]->UpdateGaugeSize(golem->GetGolemMaxLife(), golem->GetGolemCurrentLife());
		m_bar[1]->UpdateGaugeSize(golem->GetGolemMaxLife(), (float)golem->GetGolemMaxLife());

		// ��ʂ̍���ɕ\��
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
		// �Q�[�W�T�C�Y�̍X�V
		auto golem = GetStage()->GetSharedGameObject<Golem>(L"Golem");
		m_bar[0]->UpdateGaugeSize(golem->GetGolemMaxLife(), golem->GetGolemCurrentLife());
		m_bar[1]->UpdateGaugeSize(golem->GetGolemMaxLife(), (float)golem->GetGolemMaxLife());
	}

	void GolemHpBar::OnDraw(){
		// �Q�[�W�̕`��
		m_bar[1]->GameObject::OnDraw();
		m_bar[0]->GameObject::OnDraw();
	}
}