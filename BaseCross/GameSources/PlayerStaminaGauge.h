/*!
@file PlayerHpGauge.h
@brief �v���C���[��HP�o�[
*/

#pragma once
#include "stdafx.h"
#include "Geuge.h"
#include <vector>

namespace basecross {
	class PlayerStaminaBar : public GameObject {

		shared_ptr<Gauge> m_bar[2];// �X�^�~�i�o�[�̏�������ϐ�
	public:
		// �R���X�g���N�^
		PlayerStaminaBar(const std::shared_ptr<Stage>& stage) :
			GameObject(stage)
		{
		}

		void OnCreate() override;// ������
		void OnUpdate() override;// �X�V����
		void OnDraw() override;	// �`�揈��
	};
}
//end basecross

