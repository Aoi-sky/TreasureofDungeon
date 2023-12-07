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
		const WORD BUTTON_SHOT = XINPUT_GAMEPAD_X;
		float m_Speed;
		int m_DefaultLife = 100;//�v���C���[�̃f�t�H���gHP
		int m_Life;//�v���C���[��HP

	public:
		Player(const shared_ptr<Stage>& StagePtr);
		virtual ~Player() {};

		void AddPlayerDamage(int damage);
		void PlayerDead();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;

		int GetMaxLife()
		{
			return m_DefaultLife;
		}
		int GetCurrentLife()
		{
			return m_Life;
		}
	};

}
//end basecross

