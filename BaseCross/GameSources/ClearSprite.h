/* !
@file ClearSprite.h
@brief �N���A�V�[���̔w�i
*/

#pragma once
#include "stdafx.h"
#include "Sprites.h"

namespace basecross {
	class ClearSprite : public Sprites {
		const Vec2 m_DefaultSize; // �����T�C�Y

	public:
		// �R���X�g���N�^
		ClearSprite(const shared_ptr<Stage>& stage);
		// �f�X�g���N�^
		~ClearSprite();

		void OnCreate() override; // ������
		void OnUpdate() override; // �X�V����

	};
}