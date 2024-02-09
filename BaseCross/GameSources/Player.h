/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	class Player :public GameObject {
	public:
		//���[�V�����Ǘ��penum
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
			Dash1,//�_�b�V���J�n
			Dash2,//�_�b�V����
			Dash3,//�_�b�V����d��
			Dash4,//�_�b�V���と�ҋ@
		};

		// �X�e�[�^�X�Ǘ��penum
		enum ePlayeStatus
		{
			MaxLife, // �ő�̗�
			MaxStamina, // �ő�X�^�~�i
			StaminaConsumption, // 1��̃_�b�V���ŏ����X�^�~�i
			StaminaRecoverySpeed, // �X�^�~�i�񕜑��x(1�b�ӂ�̉񕜗�)
			InvincibleTime, // ���G����
			OffensiveAbility, // ��b�U����
			WalkSpeed, // ���s���̈ړ����x
			DashSpeed, // �_�b�V�����̈ړ����x
			StepSpeed, // �_�b�V�����̒Z�����ړ��̑��x
			AttackSpeed, // �U�����x(1�Œʏ푬�x)
			AttackBuff, // �U���o�t�̌��ʗ�(���ʗ�1�Ŋ�b��100%������)
			StaminaBuff, // �X�^�~�i�o�t�̌��ʗ�(���ʗ�1�Ŋ�b��100%������)
			AttackBuffLimit, // �U���o�t�̏����
			StaminaBuffLimit, // �X�^�~�i�o�t�̏����
			size,
		};
	protected:
		// ���[�V�����L�[
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
			L"Dash1",
			L"Dash2",
			L"Dash3",
			L"Dash4",
		};
		// �U���Ɏg���L�[��ێ�
		const WORD BUTTON_ATTACK = XINPUT_GAMEPAD_B;
		// �_�b�V���Ɏg���L�[��ێ�
		const WORD BUTTON_DASH = XINPUT_GAMEPAD_A;
		// ���g�̃g�����X�t�H�[���R���|�[�l���g��ێ�
		shared_ptr<Transform> m_ptrTrans;
		// ���݂̃t���[���̃��[�V������ێ�
		eMotion m_currentMotion;
		// �O�t���[���̃��[�V������ێ�
		eMotion m_pastMotion;
		// �g�����X�t�H�[���ƃ��f���̍����s��
		Mat4x4 m_differenceMatrix;
		// �X�e�[�^�X��ێ�����z��
		vector<float> m_status;
		// ���݂̃t���[���̌�����ێ�
		Vec3 m_currentAngle;
		// ����HP
		float m_life;
		// ���݃X�^�~�i
		float m_stamina;
		// �U���o�t���E������
		int AttackBuffCount;
		// �X�^�~�i�o�t���E������
		int StaminaBuffCount;
		// ���G����
		int m_invincibleTime;
		// �o�ߎ���
		int m_totalTime;
		// �S�[�����ƐڐG���Ă��邩�̔����ێ�
		bool m_hitFlg;
		
		// �G�t�F�N�g�֘A
		shared_ptr<EfkEffect> m_damageEffect;
		shared_ptr<EfkPlay> m_damageEfkPlay;
		wstring m_damageEffectStr;

	public:
		// �R���X�g���N�^
		Player::Player(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr)
		{
			// �g�����X�t�H�[���ƃ��f���̍����s�����
			m_differenceMatrix.affineTransformation(
				Vec3(0.5f, 0.5f, 0.5f),
				Vec3(0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -2.0f, 0.0f)
			);

			// csv����v���C���[�̃X�e�[�^�X����
			const auto& status = CSVLoader::LoadFile("PlayerStatus");
			m_status.resize(ePlayeStatus::size);
			m_status.at(MaxLife) = stof(status.at(MaxLife).at(1));
			m_status.at(MaxStamina) = stof(status.at(MaxStamina).at(1));
			m_status.at(StaminaConsumption) = stof(status.at(StaminaConsumption).at(1));
			m_status.at(StaminaRecoverySpeed) = stof(status.at(StaminaRecoverySpeed).at(1));
			m_status.at(InvincibleTime) = stof(status.at(InvincibleTime).at(1));
			m_status.at(OffensiveAbility) = stof(status.at(OffensiveAbility).at(1));
			m_status.at(WalkSpeed) = stof(status.at(WalkSpeed).at(1));
			m_status.at(DashSpeed) = stof(status.at(DashSpeed).at(1));
			m_status.at(StepSpeed) = stof(status.at(StepSpeed).at(1));
			m_status.at(AttackSpeed) = stof(status.at(AttackSpeed).at(1));
			m_status.at(AttackBuff) = stof(status.at(AttackBuff).at(1));
			m_status.at(StaminaBuff) = stof(status.at(StaminaBuff).at(1));
			m_status.at(AttackBuffLimit) = stof(status.at(AttackBuffLimit).at(1));
			m_status.at(StaminaBuffLimit) = stof(status.at(StaminaBuffLimit).at(1));

			// �e�ϐ��̏�����
			m_life = m_status.at(MaxLife);
			m_stamina = m_status.at(MaxStamina);
			m_currentMotion = Wait;
			m_pastMotion = Wait;
			m_hitFlg = false;
			m_invincibleTime = 90;
			m_totalTime = 0;
			AttackBuffCount = 0;
			StaminaBuffCount = 0;
		}

		/*!
		@brief	 �N���X���ŏ��ɍ쐬����鎞�Ɏ��s�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief	 ���t���[�����s�����֐�
		*/
		virtual void OnUpdate() override;

		/*!
		@brief	 ���X�e�B�b�N�̓��͂𓾂�֐�
		@return ���X�e�B�b�N�̓��͕���
		*/
		Vec2 GetInputState() const;

		/*!
		@brief �ړ������̃x�N�g���𓾂�֐�
		@return �ړ������̃x�N�g��
		*/
		Vec3 GetMoveVector() const;

		/*!
		@brief ��]����(y��)�𓾂�֐�
		@return ��]����(y��)
		*/
		float  GetMoveAngle();

		/*!
		@brief �ړ�����
		*/
		void OnMove();

		/*!
		@brief ���S���`�F�b�N����֐�
		*/
		void PlayerDeadCheck();

		/*!
		@brief �X�^�~�i�̉񕜏���
		*/
		void StaminaRecovery();

		/*!
		@brief �S�[�����Ƃ̐ڐG�Ɋւ��鏈��
		*/
		void HitCheck();

		/*!
		@brief �Q�[���p�b�h����̓��͂��m�F����֐�
		*/
		void GamepadInputCheck();

		/*!
		@brief �A�j���[�V�����̍X�V����
		*/
		void AnimationUpdate();

		/*!
		@brief �U������
		*/
		void OnAttack();

		/*!
		@brief �_���[�W��^����֐�
		@param[in]	damage �^����_���[�W
		@param[in]	Motion ��e���̃��[�V����
		*/
		void AddDamage(int damage, eMotion Motion);

		/*!
		@brief	 ���̃R���W�����ƏՓ˂������Ɏ��s�����֐�
		@param[in]	Other �Փ˂����I�u�W�F�N�g�̃|�C���^
		*/
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

		/*!
		@brief	 ���̃R���W�����Ƃ̏Փ˂��������ꂽ���Ɏ��s�����֐�
		@param[in]	Other �Փ˂����I�u�W�F�N�g�̃|�C���^
		*/
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;

		/*!
		@brief �ő�̗͂��擾����֐�
		@return �ő�̗�
		*/
		float GetMaxLife()
		{
			return m_status.at(MaxLife);
		}

		/*!
		@brief ���ݑ̗͂��擾����֐�
		@return ���ݑ̗�
		*/
		float GetCurrentLife()
		{
			return m_life;
		}

		/*!
		@brief �U���͂��擾����֐�
		@return �U����
		*/
		float GetOffensiveAbility() const
		{
			return m_status.at(OffensiveAbility);
		}

		/*!
		@brief �ő�X�^�~�i���擾����֐�
		@return �ő�X�^�~�i
		*/
		float GetMaxStamina() const
		{
			return m_status.at(MaxStamina);
		}

		/*!
		@brief ���݃X�^�~�i���擾����֐�
		@return ���݃X�^�~�i
		*/
		float GetCurrentStamina()
		{
			return  m_stamina;
		}

	};

}
//end basecross

