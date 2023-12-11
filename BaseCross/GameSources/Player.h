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

		//�A�j���[�V����
	protected:
		enum eMotion
		{
			Wait,//�ҋ@���[�V����
			Walk, // ���s�J�n
			Attack,// �U��(�U�艺�낷�U��)
			Attack2,//�U��(�ガ�����U��)
			Damage,//�y��
			Damage2//�d��
		};
		// �g�����X�t�H�[���ƃ��f���̍����s��
		Mat4x4 m_differenceMatrix;


	public:
		Player(const shared_ptr<Stage>& StagePtr);
		virtual ~Player() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
	};

}
//end basecross

