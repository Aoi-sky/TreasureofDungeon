/* !
@file GameOverSprite.h
@brief �Q�[���I�[�o�[�V�[���̔w�i
*/

#pragma once
#include "stdafx.h"
#include "Sprites.h"

namespace basecross {
	class GameOverSprite : public Sprites {
		const Vec2 m_DefaultSize; // �����T�C�Y

	public:
		// �R���X�g���N�^
		GameOverSprite(const shared_ptr<Stage>& stage);
		// �f�X�g���N�^
		~GameOverSprite();

		void OnCreate() override; // ������
		void OnUpdate() override; // �X�V����

	};
}