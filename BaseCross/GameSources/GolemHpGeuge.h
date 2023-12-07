/*!
@file GolemHpGauge.h
@brief �v���C���[��HP�o�[
*/

#pragma once
#include "stdafx.h"
#include "Geuge.h"
#include <vector>

namespace basecross {

	class GolemHpBar : public GameObject
	{
		// �ő�HP
		float m_maxHP;

		// HP�o�[�̏�������ϐ�
		shared_ptr<Gauge> m_bar[2];
	public:
		// �R���X�g���N�^
		GolemHpBar(const std::shared_ptr<Stage>& stage) :
			GameObject(stage),
			m_maxHP(0)
		{
		}

		void OnCreate() override;// ������
		void OnUpdate() override;// �X�V����
		void OnDraw() override;// �`�揈��
	};
}
//end basecross