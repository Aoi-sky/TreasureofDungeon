/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class Player :public GameObject {
		//�A�j���[�V����
	public:
		enum eMotion
		{
			Wait,//�ҋ@���[�V����
			WalkStart, // ���s�J�n
			Walking1, // ���s��(�����E)
			Walking2, // ���s��(�E����)
			WalkEnd1, // ���s�I��(�����I��)
			WalkEnd2, // ���s�I��(�E���I��)
			AttackStart,// �U��(�U�艺�낵�U��)
			AttackEnd,// �U��(�U�艺�낵�U��)
			Damage1,//��e(�y�x)
			Damage2,//��e(�d�x)
			Dead,//���S
		};

	protected:
		vector<wstring> m_motionKey = {
			L"Wait",
			L"WalkStart",
			L"Walking1",
			L"Walking2",
			L"WalkEnd1",
			L"WalkEnd2",
			L"AttackStart",
			L"AttackEnd",
			L"Damage1",
			L"Damage2",
			L"Dead",
		};

		// �g�����X�t�H�[���ƃ��f���̍����s��
		Mat4x4 m_differenceMatrix;

		Vec2 GetInputState() const;//�v���C���[�̃R���g���[���ƃL�[�{�[�h�̓���
		Vec3 GetMoveVector() const;// �R���g���[����������x�N�g���𓾂�
		void MovePlayer();//�v���C���[�̈ړ�
		const WORD BUTTON_SHOT = XINPUT_GAMEPAD_X;
		float m_Speed;
		int m_DefaultLife = 100;//�v���C���[�̃f�t�H���gHP
		int m_Life;//�v���C���[��HP
		// ���[�V�����L�[�̏����ݒ�
		eMotion m_currentMotion = Wait;
		eMotion m_pastMotion = Wait;
		// �G�ƐڐG���Ă��邩�𔻒肷��t���O
		bool HitFlg = false;
		// ���G����
		int invincibleTime = 90;
		// �o�ߎ���
		int totalTime = 0;

	public:
		Player(const shared_ptr<Stage>& StagePtr);
		virtual ~Player() {};

		void OnAttack();
		void HitCheck();
		void AddPlayerDamage(int damage, eMotion Motion);
		void PlayerDead();
		void AnimationUpdate();

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

