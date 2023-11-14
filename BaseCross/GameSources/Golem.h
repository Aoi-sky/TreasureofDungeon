/*!
@file Golem.h
@brief �S�[�����{��
@author �����
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	�Z���������g�����s���N���X�̃t���O
	//--------------------------------------------------------------------------------------
	enum class CellSearchedFlg {
		Failed, // ���s
		Seek, // �ǐ�
		Arrived // ����
	};

	// �X�e�[�^�X��ݒ�
	struct Status {
		int life; // �̗�
		float speed; // �ړ����x
		int attackInterspace; // �G�̍U���Ԋu
		float searchDistance; // �G�̊��m�͈�
	};

	// �X�e�[�^�X��ݒ�
	// �̗�, �ړ����x, �U���p�x(�t���[��)
	const Status statusList[1] = {
			{1000, 0.05f, 180}
	};

	// ���N���X
	class Golem : public GameObject
	{
	protected:

		// �A�j���[�V�����萔
		enum eMotion
		{
			Wait, // �ҋ@
			Booting, // �N��
			WalkStart, // ���s�J�n
			Walking1, // ���s��(���[�v��)
			Walking2, // ���s��(���[�v��)
			WalkFinish, // ���s�I��
			AttackStart_Swingdown,// �U���J�n(�U�艺�낵)
			AttackFinish_Swingdown, // �U���I��(�U�艺�낵)
			AttackStart_Punch,// �U���J�n(�p���`)
			AttackFinish_Punch, // �U���I��(�p���`)
			AttackStart_Ramming,// �U���J�n(�ːi)
			Attacking_Ramming1, // �U����(�ːi)(���[�v��)
			Attacking_Ramming2, // �U����(�ːi)(���[�v��)
			AttackFinish_Ramming, // �U���I��(�ːi)
			Death, // ���S
		};

		// �Z���}�b�v�̃|�C���^
		weak_ptr<StageCellMap> m_cellMapPtr;
		// �X�e�[�^�X��ۑ����郁���o�ϐ�
		Status m_status;
		// �O��̍U������o�߂����t���[����
		int m_countTime;
		// �X�|�[���n�_
		Vec3 m_startPos;
		// ��]
		Vec3 m_rotation;
		// �����x
		Vec3 m_force;
		// ���x
		Vec3 m_velocity;
		// �ړ�����
		Vec3 m_forward;
		// �O�t���[���̍��W
		Vec3 m_currentPos;
		// �ːi�U���J�n���̍��W
		Vec3 m_rammingPos;
		// �ːi�𒆒f���邩�𔻒肷��t���O
		bool m_stopRammingFlg;
		// �������g�̃g�����X�t�H�[���R���|�[�l���g
		std::shared_ptr<Transform> m_transform;
		// �ʉ߂���o�H
		vector<CellIndex> m_cellIVec;
		//���݂̎����̃Z���C���f�b�N�X
		int m_cellIndex;
		//�ڎw��(����)�Z���C���f�b�N�X
		int m_nextCellIndex;
		//�^�[�Q�b�g�̃Z���C���f�b�N�X
		int m_targetCellIndex;
		// �g�����X�t�H�[���ƃ��f���̍����s��
		Mat4x4 m_differenceMatrix;
		// �A�j���[�V�����̃L�[
		vector<wstring> m_motionKey;
		// �A�j���[�V�����̃^�C�v
		eMotion m_motion;
		// �O�t���[���̃A�j���[�V�����̃^�C�v
		eMotion m_currentMotion;


	public:
		// �R���X�g���N�^
		Golem::Golem(const shared_ptr<Stage>& stagePtr, // �X�e�[�W�̃|�C���^
			const shared_ptr<StageCellMap>& cellMapptr, // �Z���}�b�v�̃V�F�A�h�|�C���^
			const Vec3 position // �������W
		) :
			GameObject(stagePtr), // �X�e�[�W�̃|�C���^
			m_cellMapPtr(cellMapptr), // �Z���}�b�v�̃V�F�A�h�|�C���^
			m_startPos(position) // �������W
		{
			// ��Փx���������ꍇ�͂����ŃX�e�[�^�X���X�g�̎Q�Ɛ��ύX
			m_status = statusList[0];

			m_rotation = Vec3(0.0f);

			m_differenceMatrix.affineTransformation(
				Vec3(0.2f, 0.2f, 0.2f),
				Vec3(0.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(0.0f, -0.6f, 0.0f)
			);

			m_motionKey = {
				L"WAIT", // �N��
				L"BOOTING", // �N��
				L"WALKSTART", // ���s�J�n
				L"WALKING1", // ���s��(���[�v��)
				L"WALKING2", // ���s��(���[�v��)
				L"WALKFINISH", // ���s�I��
				L"ATTACKSTART_SWINGDOWN", // �U���J�n(�U�艺�낵)
				L"ATTACKFINISH_SWINGDOWN", // �U���I��(�U�艺�낵)
				L"ATTACKSTART_PUNCH", // �U���J�n(�p���`)
				L"ATTACKFINISH_PUNCH", // �U���I��(�p���`)
				L"ATTACKSTART_RAMMING", // �U���J�n(�ːi)
				L"ATTACKING_RAMMING1", // �U����(�ːi)(���[�v��)
				L"ATTACKING_RAMMING2", // �U����(�ːi)(���[�v��)
				L"ATTACKFINISH_RAMMING", // �U���I��(�ːi)
				L"DEATH" // ���S
			};

			m_motion = Booting;
			m_currentMotion = Booting;

		}

		// �f�X�g���N�^
		virtual ~Golem() {};

		/*!
		@brief	 �N���X���ŏ��ɍ쐬����鎞�Ɏ��s�����֐�
		*/
		void OnCreate();

		/*!
		@brief	 ���t���[�����s�����֐�
		*/
		void OnUpdate();

		/*!
		@brief �A�j���[�V�����Đ��֐�
		*/
		void AnimationUpdate();

		/*!
		@brief	 �ړ�������֐�
		*/
		void MoveGolem();

		/*!
		@brief	 �w�肳�ꂽ�U���͈͓̔��Ƀv���C���[�����邩���m�F����֐�
		@param[in]	 motion
		@return �U���͈͓��ɋ�����true
		*/
		bool CheckAttackArea(eMotion motion);

		/*!
		@brief	 �S�[�����Ƀ_���[�W��^����֐�
		*/
		void AddDamage(int damage);

		/*!
		@brief	 ���̃R���W�����ƏՓ˂������Ɏ��s�����֐�
		@param[in]	Other	�Փ˂����I�u�W�F�N�g�̃|�C���^
		*/
		void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

		/*!
		@brief	 ���̃R���W�����Ƃ̏Փ˂��������ꂽ���Ɏ��s�����֐�
		@param[in]	Other	�Փ˂��Ă����I�u�W�F�N�g�̃|�C���^
		*/
		void OnCollisionExit(shared_ptr<GameObject>& Other) override;
	};
}
