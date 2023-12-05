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
	};

	// �X�e�[�^�X��ݒ�
	// �̗�, �ړ����x, �U���p�x(�t���[��)
	const Status statusList[1] = {
			{1000, 0.05f, 180}
	};

	// �U���̏ڍאݒ�
	struct Attack {
		float PunchRange; // �p���`�̍U���͈�
		float SwingdownRange; // �U�艺�낵�̍U���͈�
		float RammingRange; // �ːi�̎˒�
		float RammingSpeed; // �ːi���̈ړ����x�����{��
		float NormalTurningSpeed; // �ʏ펞�̃S�[�����̐��񑬓x
		float RammingTurningSpeed; // �ːi���̃S�[�����̐��񑬓x
	};

	const Attack AttackList[1] = {
		{6.0f, 8.0f, 20.0f, 4.0f, 3.0f, 0.3f}
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
			AttackFinish_Ramming1, // �U���I��(�ːi)(�X�^�����[�V�����ׂ̈�2����)
			AttackFinish_Ramming2, // �U���I��(�ːi)(�X�^�����[�V�����ׂ̈�2����)
			AttackFinish_Ramming3, // �U���I��(�ːi)(�������Ȃ����~)
			Stun_Ramming_Forward, // �X�^��(�ːi��ɑO�����)
			Stun_Ramming_Behind, // �X�^��(�ːi��Ɍ�납���)
			Stun_Normal_Forward, // �X�^��(�O�����)
			Stun_Normal_Behind, // �X�^��(��납���)
			Stun1, // �X�^����(���[�v��)
			Stun2, // �X�^����(���[�v��)
			Stun_Recovery, // �X�^�����A
			Death // ���S
		};

		// �U���萔
		enum eAttack
		{
			SwingDown, // �U�艺�낵�U��
			Punch, // �p���`�U��
			Ramming // �ːi�U��
		};

		// �����萔
		enum eDirection {
			Forward,
			Behind
		};

		// �Z���}�b�v�̃|�C���^
		weak_ptr<StageCellMap> m_cellMapPtr;
		// �X�e�[�^�X��ۑ����郁���o�ϐ�
		Status m_status;
		// �U���̏ڍאݒ��ۑ����郁���o�ϐ�
		Attack m_attackStatus;
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
		// �U�艺�낵�U�������邩�𔻒肷��t���O
		bool m_canSwingDown;
		// �p���`�U�������邩�𔻒肷��t���O
		bool m_canPunch;
		// �ːi�𒆒f���邩�𔻒肷��t���O(true�Œ��f)
		bool m_stopRammingFlg;
		// �X�^���U�����󂯂����𔻒肷��t���O(true�ŃX�^���U�����󂯂�)
		bool m_stunAttackFlg;
		// �X�^�����p�����鎞��(�X�^�����畜�A��0�Ƀ��Z�b�g)
		int m_stunDuration;
		// �X�^���J�n����̍��v����(�X�^�����畜�A��0�Ƀ��Z�b�g)
		int m_stunTotalTime;
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
		// �v���C���[�̃|�C���^
		weak_ptr<GameObject> m_playerPtr;
		// �Փ˂��Ă�����̊p�x(�S�[�����̐��ʕ�����0�Ƃ���)
		float m_rockAngle;
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
			m_status = statusList[0];
			m_attackStatus = AttackList[0];

			m_rotation = Vec3(0.0f);

			m_differenceMatrix.affineTransformation(
				Vec3(0.25f, 0.25f, 0.25f),
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
				L"ATTACKFINISH_RAMMING1", // �U���I��(�ːi)(�X�^�����[�V�����ׂ̈�2����)
				L"ATTACKFINISH_RAMMING2", // �U���I��(�ːi)(�X�^�����[�V�����ׂ̈�2����)
				L"ATTACKFINISH_RAMMING3", // �U���I��(�ːi)(�������Ȃ����~)
				L"STUN_RAMMING_FORWARD", // �X�^��(�ːi��ɑO�����)
				L"STUN_RAMMING_BEHIND", // �X�^��(�ːi��Ɍ�납���)
				L"STUN_NORMAL_FORWARD", // �X�^��(�O�����)
				L"STUN_NORMAL_BEHIND", // �X�^��(��납���)
				L"STUN1", // �X�^����(���[�v��)
				L"STUN2", // �X�^����(���[�v��)
				L"STUN_RECOVERY", // �X�^�����A
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
		@param[in]	 Attack �U���̎��
		@return �U���͈͓��ɋ�����true
		*/
		bool CheckAttackArea(eAttack Attack);

		/*!
		@brief	 �S�[�����Ƀ_���[�W��^����֐�
		*/
		void AddDamage(int damage);

		/*!
		@brief	 �S�[�������X�^���U�����󂯂��t���O�𗧂āA�X�^���̍��v���Ԃ����Z�b�g����֐�
		@param[in]	 StunTime �X�^������t���[����
		*/
		void AddStun(int StunTime);

		/*!
		@brief	 �S�[�����̐��ʕ�������Ƃ��āA�^�[�Q�b�g�̕������v�Z����֐�
		@param[in]	 target �^�[�Q�b�g���������Ă���Transform�R���|�[�l���g�̃|�C���^
		@return �S�[�����̐��ʕ�������Ƃ����p�x��Ԃ�
		*/
		float AngleCalculation(shared_ptr<Transform>& target);

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
