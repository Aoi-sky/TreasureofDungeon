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
		// Attack��ԂɂȂ�܂ł̑ҋ@����
		int m_standbyTime;
		// Move�EAttack��Ԃ����v������������
		int m_duration;
		// ��������������Ă���̌o�ߎ���
		int m_elapsedTime;
		// �������˂��o�Ă��鑬�x
		float m_speed;
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
			const int standbyTime, // Attack��Ԃ܂ł̑ҋ@����
			const int duration // Attack��Ԃ̎�������
		) :
			GameObject(stagePtr), // �X�e�[�W�̃|�C���^
			m_startPos(position), // �������W
			m_standbyTime(standbyTime), // Attack��Ԃ܂ł̑ҋ@����
			m_duration(duration) // Attack��Ԃ̎�������
		{
			m_differenceMatrix.affineTransformation(
				Vec3(0.4f, 0.4f, 0.4f),
				Vec3(0.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f)
			);

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
		@brief	 �����̏�Ԃ��Ǘ�����֐�
		*/
		void CheckState();
	};
}