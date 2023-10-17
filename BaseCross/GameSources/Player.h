/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class Player :public GameObject {
		
		Vec2 GetInputState() const;//�v���C���[�̃R���g���[���ƃL�[�{�[�h�̓���
		Vec3 GetMoveVector() const;// �R���g���[����������x�N�g���𓾂�
		void MovePlayer();//�v���C���[�̈ړ�
		
		float m_Speed;

	public:
		Player(const shared_ptr<Stage>& StagePtr);
		virtual ~Player() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

	};

}
//end basecross

