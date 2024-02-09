/*!
@file Crystal.h
@brief �����{��
@author �����
*/
#pragma once
#include "stdafx.h"

namespace basecross {
	// �����N���X
	class Crystal : public GameObject
	{
	protected:
		//// �A�j���[�V�����萔
		enum eState
		{
			Wait, // �n���őҋ@
			Move, // �n��ֈړ�
			Attack, // �n��ŐÎ~
			Finish // �n���Ɉړ��A���̌����
		};

		// �������g�̃g�����X�t�H�[���R���|�[�l���g
		std::shared_ptr<Transform> m_transform;
		// �������W
		Vec3 m_startPos;
		// Wait��Ԃ��ێ����鎞��(�b)
		int m_standbyTime;
		// Move��Ԃ��I����܂ł̏��v����(�b)
		int m_equiredTime;
		// Attack��Ԃ̎�������(�b)
		int m_duration;
		// ��������Ă���̌o�ߎ���(�b)
		float m_progressTime;
		// ������ɐ������߂��Ă������x
		float m_speed;
		// ElapsedTime��ێ�����ϐ�
		float m_elapsedTime;
		// �������v���C���[�ɖ�����������ۑ�����ϐ�
		bool m_isHit;
		// ���݂̃t���[���̏�Ԃ�ۑ�����ϐ�
		eState m_currentState;	
		// �O�t���[���̏�Ԃ�ۑ�����ϐ�
		eState m_pastState;
		// �g�����X�t�H�[���ƃ��f���̍����s��
		Mat4x4 m_differenceMatrix;


	public:
		// �R���X�g���N�^
		Crystal::Crystal(const shared_ptr<Stage>& stagePtr, // �X�e�[�W�̃|�C���^
			const Vec3 position, // �������W
			const int standbyTime, // Wait��Ԃ��ێ����鎞��(�b)
			const int equiredTime, // Move��Ԃ���������܂ł̏��v����(�b)
			const int duration // Attack��Ԃ̎�������(�b)
		) :
			GameObject(stagePtr), // �X�e�[�W�̃|�C���^
			m_startPos(position), // �������W
			m_standbyTime(standbyTime), // Wait��Ԃ��ێ����鎞��(�b)
			m_equiredTime(equiredTime), // Move��Ԃ���������܂ł̏��v����(�b)
			m_duration(duration) // Attack��Ԃ̎�������(�b)
		{
			m_differenceMatrix.affineTransformation(
				Vec3(0.4f, 0.4f, 0.4f),
				Vec3(0.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f)
			);

			m_progressTime = 0.0f;
			m_speed = 0.0f;
			m_isHit = false;
			m_currentState = Wait;
			m_pastState = Wait;
		}

		// �f�X�g���N�^
		virtual ~Crystal() {};

		/*!
		@brief	 �N���X���ŏ��ɍ쐬����鎞�Ɏ��s�����֐�
		*/
		void OnCreate();

		/*!
		@brief	 ���t���[�����s�����֐�
		*/
		void OnUpdate();

		/*!
		@brief	 �ړ��������s���֐�
		*/
		void OnMove();

		/*!
		@brief	 ������̏������s���֐�
		*/
		void OnHit();

		/*!
		@brief	 State���Ǘ�����֐�
		*/
		void StateManagement();
	};
}