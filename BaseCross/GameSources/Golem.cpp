/*!
@file Golem.cpp
@brief �S�[�����{��
@author �����
*/

#include "iostream"
#include "stdafx.h"
#include "Project.h"
#include "math.h"
#include <vector>

namespace basecross {
	//--------------------------------------------------------------------------------------
	// �G���f�[�^�ƃN���X
	//--------------------------------------------------------------------------------------
	void Golem::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		m_transform = GetComponent<Transform>();
		m_transform->SetScale(Vec3(3.0f));
		m_transform->SetRotation(Vec3(0.0f));
		m_transform->SetPosition(m_startPosition);


		// PathSearch�R���|�[�l���g�̎擾
		auto MapPtr = m_cellMapPtr.lock();
		if (MapPtr)
		{
			AddComponent<PathSearch>(MapPtr);
		}
		//CollisionSphere�Փ˔����t����
		auto ptrColl = AddComponent<CollisionCapsule>();
		ptrColl->SetDrawActive(false);
		ptrColl->SetFixed(true);
		ptrColl->SetDrawActive(true);

		//�`��R���|�[�l���g�̐ݒ�
		SetAlphaActive(true);
		SetDrawActive(true);

		// �V�K�h���[�R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<BcPNTBoneModelDraw>();
		ptrDraw->SetMeshResource(L"GOLEM");
		ptrDraw->SetMeshToTransformMatrix(m_differenceMatrix);
		ptrDraw->SetSpecularColor(Col4(1, 1, 1, 1));
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetLightingEnabled(false);

		// �A�j���[�V�����̐ݒ�
		ptrDraw->AddAnimation(L"WAIT", 0, 1, true);
		ptrDraw->AddAnimation(L"BOOTING", 0, 90, false);
		ptrDraw->AddAnimation(L"WALKSTART", 100, 30, false);
		ptrDraw->AddAnimation(L"WALKING", 130, 60, true);
		ptrDraw->AddAnimation(L"WALKFINISH", 190, 15, false);
		ptrDraw->AddAnimation(L"ATTACKSTART_SWINGDOWN", 210, 60, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_SWINGDOWN", 270, 30, false);
		ptrDraw->AddAnimation(L"ATTACKSTART_PUNCH", 310, 60, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_PUNCH", 370, 60, false);
		ptrDraw->AddAnimation(L"ATTACKSTART_RAMMING", 440, 60, false);
		ptrDraw->AddAnimation(L"ATTACKING_RAMMING", 500, 30, true);
		ptrDraw->AddAnimation(L"ATTACKFINISH_RAMMING", 530, 60, false);
		ptrDraw->AddAnimation(L"DEATH", 0, 1, true);

		// �ŏ��̃A�j���[�V�������w��
		ptrDraw->ChangeCurrentAnimation(L"BOOTING");



		// �e��ǉ�
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"GOLEM");
		ptrShadow->SetMeshToTransformMatrix(m_differenceMatrix);
		// �^�O�̐ݒ�
		AddTag(L"ENEMY");
		AddTag(L"BOSS");

	}

	void Golem::OnUpdate() {
		// �A�j���[�V�������X�V
		AnimationUpdate();

		if (m_motion == Walking) {
			// �f���^�^�C�����擾
			float deltaTime = App::GetApp()->GetElapsedTime();
			// �U���p�̃J�E���g�����Z
			m_countTime += deltaTime;
		}

		if (m_motion == Walking || m_motion == WalkFinish || m_motion == Attacking_Ramming || m_motion == AttackFinish_Ramming) {
			MoveGolem();
		}


		if (m_status.life <= 0 && m_motion != Death) {
			// �̗͂�0�ȉ��̏ꍇ�A���[�V������ύX
			m_motion = Death;
		}

		// �A�j���[�V�������X�V
		AnimationUpdate();

		// �A�j���[�V�������I�����Ă��邩���`�F�b�N
		auto ptrDraw = GetComponent<BcPNTBoneModelDraw>();
		if (ptrDraw->IsTargetAnimeEnd()) {
			if (m_currentMotion == eMotion::Booting) {
				m_motion = WalkStart;
			}

			if (m_currentMotion == eMotion::WalkStart) {
				m_motion = Walking;
			}

			if (m_currentMotion == eMotion::Walking) {
				// �J�E���g���U���Ԋu�ɒB������
				if (m_countTime > m_status.attackInterspace) {
					m_motion = WalkFinish;
					m_countTime = 0;
				}
			}

			if (m_currentMotion == eMotion::WalkFinish) {
				bool CanSwingDown = CheckAttackArea(eMotion::AttackStart_Swingdown);
				bool CanPunch = CheckAttackArea(eMotion::AttackStart_Punch);

				// ���݂̏󋵂ɉ����čU�����[�V���������肷��
				if (CanSwingDown && !CanPunch) {
					m_motion = AttackStart_Swingdown;
				}
				if (!CanSwingDown && CanPunch) {
					m_motion = AttackStart_Punch;
				}
				// �ǂ���̍U�����\�ȏꍇ�̓����_���ɑI�������
				if (CanSwingDown && CanPunch) {
					int random = rand() % 2;
					if (random == 0) {
						m_motion = AttackStart_Swingdown;
					}
					else {
						m_motion = AttackStart_Punch;
					}
				}
				else {
					// ��L�̍U���͈͓��ɂ����Ȃ��ꍇ�̓v���C���[�Ɍ������ēːi�U��������
					m_motion = AttackStart_Ramming;
				}
			}

			if (m_currentMotion == eMotion::AttackStart_Swingdown) {
				// �U���͈͓��Ƀv���C���[��������_���[�W
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				m_motion = AttackFinish_Swingdown;
			}

			if (m_currentMotion == eMotion::AttackFinish_Swingdown) {
				m_motion = WalkStart;
			}

			if (m_currentMotion == eMotion::AttackStart_Punch) {
				// �U���͈͓��Ƀv���C���[��������_���[�W
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				m_motion = AttackFinish_Punch;
			}

			if (m_currentMotion == eMotion::AttackFinish_Punch) {
				m_motion = WalkStart;
			}

			if (m_currentMotion == eMotion::AttackStart_Ramming) {
				m_motion = Attacking_Ramming;
			}

			if (m_currentMotion == eMotion::Attacking_Ramming) {

				m_motion = AttackFinish_Ramming;
			}

			if (m_currentMotion == eMotion::AttackFinish_Ramming) {
				m_motion = WalkStart;
			}
			if (m_currentMotion == eMotion::Death) {
				// �S�[��������������
				GetStage()->RemoveGameObject<Golem>(GetThis<Golem>());
			}


		}
	}
	void Golem::AnimationUpdate()
	{
		// �A�j���[�V�����̍Đ�
		auto ptrDraw = GetComponent<BcPNTBoneModelDraw>();

		// �A�j���[�V�����̃^�C�v���ς���Ă�����
		if (m_motion != m_currentMotion || ptrDraw->GetCurrentAnimation() != m_motionKey.at(m_motion))
		{
			// �^�C�v�ɉ����ăA�j���[�V������ύX����
			ptrDraw->ChangeCurrentAnimation(m_motionKey.at(m_motion));
			m_currentMotion = m_motion;
		}

		// �O�t���[������̃f���^�^�C�����擾
		float deltaTime = App::GetApp()->GetElapsedTime();

		// ���[�V������c�[���ɉ����čĐ�����
		if (m_motion == Wait)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == WalkStart)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == Walking)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == WalkFinish)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == AttackStart_Swingdown)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == AttackFinish_Swingdown)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == AttackStart_Punch)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == AttackFinish_Punch)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == AttackStart_Ramming)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == Attacking_Ramming)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == AttackFinish_Ramming)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == Death)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}

	}

	void Golem::MoveGolem() {
		// �v���C���[�̍��W���擾
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto playerPos = ptrPlayer->GetComponent<Transform>()->GetPosition();

		// ���g�̍��W���擾
		auto ptrTrans = GetComponent<Transform>();
		auto golemPos = ptrTrans->GetPosition();

		// �f���^�^�C�����擾
		float deltaTime = App::GetApp()->GetElapsedTime();

		// �v�Z���������x���ړ��ʂɉ��Z
		m_velocity += m_force * deltaTime;

		// �ړ��ʂ�m_status.speed������Ƃ���
		if (m_status.speed < m_velocity.length()) {
			m_velocity *= m_status.speed / m_velocity.length();
		}


		if (m_motion == Walking || m_motion == WalkFinish) {
			// ���W���X�V
			golemPos += m_velocity * deltaTime * 0.1f;
			ptrTrans->SetPosition(golemPos);

			// �ړ������̌������擾
			float rad = -atan2(golemPos.z - m_currentPos.z, golemPos.x - m_currentPos.x);
			// �Y���̏C���̂��߂Ɋp�x��90�����炷
			Vec3 rotation(0.0f, rad - XM_PIDIV2, 0.0f);

			if (m_rotation.y < rotation.y) {
				m_rotation.y += 3.0f * (XM_PI / 180.0f);
				if (m_rotation.y > rotation.y) {
					m_rotation.y -= 3.0f * (XM_PI / 180.0f);
				}

				// �v�Z�������������g�ɓK�p
				m_transform->SetRotation(m_rotation);
			}
		}
		else
		{
			// �v���C���[�𔭌��o���Ȃ������ꍇ�͈ړ��ʂ�0�ɂ���
			m_velocity = Vec3(0.0f, 0.0f, 0.0f);
		}

		// ���݂̍��W��O��̃t���[���̍��W�Ƃ���m_currentPos�ɕۑ�
		m_currentPos = m_transform->GetPosition();
		// �����x�����Z�b�g����
		m_force = Vec3(0.0f, 0.0f, 0.0f);
	}

	bool Golem::CheckAttackArea(eMotion motion) {
		// �U���͈�
		float attackRange;

		// �U���ɉ����čU���͈͂���
		if (motion == AttackStart_Swingdown) {
			attackRange = 4.0f;
		}
		if (motion == AttackStart_Punch) {
			attackRange = 2.0f;
		}

		// ���g�̍��W���擾
		auto ptrTrans = GetComponent<Transform>();
		auto golemPos = ptrTrans->GetPosition();
		auto golemForward = ptrTrans->GetForward();
		golemForward *= -1.0f / golemForward.length();

		// �U���͈͂̒��S���W���v�Z
		auto areaPos = golemPos + golemForward * attackRange;

		// �v���C���[�̍��W���擾
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto playerPos = ptrPlayer->GetComponent<Transform>()->GetPosition();

		// �U���͈͂̒��S���W����v���[���[�܂ł̋������v�Z
		float  targetToLength = Vec3(areaPos.x - playerPos.x, 0.0f, areaPos.z - playerPos.z).length();

		// �U���͈͓��ɋ����ꍇtrue
		if (targetToLength < attackRange) {
			return true;
		}
		// ���Ȃ����false
		return false;
	}

	void Golem::AddDamage(int Damage) {
		m_status.life -= Damage;
	}
}