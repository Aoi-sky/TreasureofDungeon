/*!
@file PlayerHpGauge.h
@brief �v���C���[��HP�o�[
*/

#pragma once
#include "stdafx.h"
#include "Geuge.h"
#include <vector>

namespace basecross {
	class PlayerHpBar : public GameObject{
		
		float m_maxHP;// �ő�HP
		shared_ptr<Gauge> m_bar[2];// HP�o�[�̏�������ϐ�
	public:
		// �R���X�g���N�^
		PlayerHpBar(const std::shared_ptr<Stage>& stage) :
			GameObject(stage),
			m_maxHP(0)
		{
		}

		void OnCreate() override;// ������
		void OnUpdate() override;// �X�V����
		void OnDraw() override;	// �`�揈��
	};
}
//end basecross

