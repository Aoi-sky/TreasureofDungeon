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
		m_transform->SetScale(Vec3(4.3f));
		m_transform->SetRotation(Vec3(0.0f));
		m_transform->SetPosition(m_startPos);

		// PathSearch�R���|�[�l���g�̎擾
		auto MapPtr = m_cellMapPtr.lock();
		if (MapPtr)
		{
			AddComponent<PathSearch>(MapPtr);
		}
		//CollisionSphere�Փ˔����t����
		auto ptrColl = AddComponent<CollisionCapsule>();
		ptrColl->SetDrawActive(false);
		ptrColl->SetFixed(false);

		//�`��R���|�[�l���g�̐ݒ�
		SetAlphaActive(true);
		SetDrawActive(true);

		// �V�K�h���[�R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<BcPNTBoneModelDraw>();
		ptrDraw->SetMultiMeshResource(L"GOLEM");
		ptrDraw->SetMeshToTransformMatrix(m_differenceMatrix);
		ptrDraw->SetSpecularColor(Col4(1, 1, 1, 1));
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetLightingEnabled(false);

		// �A�j���[�V�����̐ݒ�
		ptrDraw->AddAnimation(L"WAIT", 0, 1, true);
		ptrDraw->AddAnimation(L"BOOTING", 0, 90, false);
		ptrDraw->AddAnimation(L"WALKSTART", 100, 30, false);
		ptrDraw->AddAnimation(L"WALKING1", 130, 60, false);
		ptrDraw->AddAnimation(L"WALKING2", 130, 60, false);
		ptrDraw->AddAnimation(L"WALKFINISH", 190, 15, false);
		ptrDraw->AddAnimation(L"ATTACKSTART_SWINGDOWN", 210, 40, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_SWINGDOWN", 250, 50, false);
		ptrDraw->AddAnimation(L"ATTACKSTART_PUNCH", 310, 45, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_PUNCH", 355, 75, false);
		ptrDraw->AddAnimation(L"ATTACKSTART_RAMMING", 440, 60, false);
		ptrDraw->AddAnimation(L"ATTACKING_RAMMING1", 500, 30, false);
		ptrDraw->AddAnimation(L"ATTACKING_RAMMING2", 500, 30, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_RAMMING1", 530, 30, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_RAMMING2", 560, 30, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_RAMMING3", 600, 160, false);
		ptrDraw->AddAnimation(L"STUN_RAMMING_FORWARD", 770, 60, false);
		ptrDraw->AddAnimation(L"STUN_RAMMING_BEHIND", 840, 30, false);
		ptrDraw->AddAnimation(L"STUN_NORMAL_FORWARD", 880, 50, false);
		ptrDraw->AddAnimation(L"STUN_NORMAL_BEHIND", 940, 30, false);
		ptrDraw->AddAnimation(L"STUN1", 970, 1, false);
		ptrDraw->AddAnimation(L"STUN2", 970, 1, false);
		ptrDraw->AddAnimation(L"STUN_RECOVERY", 980, 120, false);
		ptrDraw->AddAnimation(L"DEATH", 600, 1, false);

		// �ŏ��̃A�j���[�V�������w��
		ptrDraw->ChangeCurrentAnimation(L"BOOTING");

		// �e��ǉ�
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMultiMeshResource(L"GOLEM");
		ptrShadow->SetMeshToTransformMatrix(m_differenceMatrix);

		// �v���C���[�֘A�̂̃|�C���^���擾
		m_playerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		m_playerTrans = m_playerPtr->GetComponent<Transform>();


		// �^�O�̐ݒ�
		AddTag(L"Enemy");
		AddTag(L"Golem");
	}

	void Golem::OnUpdate() {
		// �J�E���g�����Z
		m_countTime++;

		switch (m_motion) {
		case Walking1:
		case Walking2:
		case Attacking_Ramming1:
		case Attacking_Ramming2:
		case AttackFinish_Ramming3:
			MoveGolem();
			break;

		default:
			break;
		}

		if (m_status.life <= 0 && m_motion != Death) {
			// �̗͂�0�ȉ��̏ꍇ�A���[�V������ύX
			m_motion = Death;
		}

		// �A�j���[�V�������X�V
		AnimationUpdate();

		// ���g�̍��W�ƌ������擾
		auto golemPos = m_transform->GetPosition();
		auto golemForward = m_transform->GetForward();
		golemForward *= -1.0f / golemForward.length();
		auto areaPos = Vec3(0.0f);

		// �A�j���[�V�������I�����Ă��邩���`�F�b�N
		auto ptrDraw = GetComponent<BcPNTBoneModelDraw>();
		if (ptrDraw->IsTargetAnimeEnd()) {
			switch (m_currentMotion) {
			case eMotion::Booting:
				m_motion = WalkStart;
				m_countTime = 0;
				break;

			case eMotion::WalkStart:
				m_motion = Walking1;
				m_countTime = 0;
				break;

			case eMotion::Walking1:
				// �J�E���g���U���Ԋu�ɒB���������`�F�b�N
				if (m_countTime > m_status.attackInterspace) {
					// �v���C���[�����ʕ����ɋ��邩���`�F�b�N
					if (AngleCalculation(m_playerTrans, false) <= 15) {
 						m_motion = WalkFinish;
						m_countTime = 0;
						break;
					}
				}
				m_motion = Walking2;
				break;

			case eMotion::Walking2:
				// �J�E���g���U���Ԋu�ɒB���������`�F�b�N
				if (m_countTime > m_status.attackInterspace) {
					// �v���C���[�����ʕ����ɋ��邩���`�F�b�N
					if (AngleCalculation(m_playerTrans, false) <= 15) {
						m_motion = WalkFinish;
						m_countTime = 0;
						break;
					}
				}
				m_motion = Walking1;
				break;

			case eMotion::WalkFinish:
				// �U���͈͓��Ƀv���C���[�����邩���`�F�b�N
				m_canSwingDown = CheckAttackArea(eAttack::SwingDown);
				m_canPunch = CheckAttackArea(eAttack::Punch);

				// ���݂̏󋵂ɉ����čU�����[�V���������肷��
				if (m_canSwingDown && !m_canPunch) {
					m_motion = AttackStart_Swingdown;
					m_countTime = 0;

					// �U���͈͂̒��S���W���v�Z
					areaPos = golemPos + golemForward * m_attackStatus.SwingdownRange;
					areaPos.y = 0.8f;
					// �U���͈͂̕`��
					GetStage()->AddGameObject<FillSprite>(areaPos, m_attackStatus.SwingdownRange, 90);
					break;
				}
				if (!m_canSwingDown && m_canPunch) {
					m_motion = AttackStart_Punch;
					m_countTime = 0;

					// �U���͈͂̒��S���W���v�Z
					areaPos = golemPos + golemForward * m_attackStatus.PunchRange;
					areaPos.y = 0.8f;
					// �U���͈͂̕`��
					GetStage()->AddGameObject<FillSprite>(areaPos, m_attackStatus.PunchRange, 98);
					break;
				}
				// �ǂ���̍U�����\�ȏꍇ�̓����_���ɑI�������
				if (m_canSwingDown && m_canPunch) {
					int random = rand() % 2;
					switch (random)
					{
					case 0:
						m_motion = AttackStart_Swingdown;
						m_countTime = 0;

						// �U���͈͂̒��S���W���v�Z
						areaPos = golemPos + golemForward * m_attackStatus.SwingdownRange;
						areaPos.y = 0.8f;
						// �U���͈͂̕`��
						GetStage()->AddGameObject<FillSprite>(areaPos, m_attackStatus.SwingdownRange, 90);
						break;

					case 1:
						m_motion = AttackStart_Punch;
						m_countTime = 0;

						// �U���͈͂̒��S���W���v�Z
						areaPos = golemPos + golemForward * m_attackStatus.PunchRange;
						areaPos.y = 0.8f;
						// �U���͈͂̕`��
						GetStage()->AddGameObject<FillSprite>(areaPos, m_attackStatus.PunchRange, 98);
						break;

					default:
						break;
					}
					break;
				}

				// ��L�̍U���͈͓��ɂ����Ȃ��ꍇ�̓v���C���[�Ɍ������ēːi�U��������
				m_motion = AttackStart_Ramming;
				m_countTime = 0;
				break;

			case eMotion::AttackStart_Swingdown:
				// �U���͈͓��Ƀv���C���[��������_���[�W
				if (CheckAttackArea(SwingDown)) {
					m_playerPtr->AddPlayerDamage(25);

				}
				m_motion = AttackFinish_Swingdown;
				m_countTime = 0;
				break;

			case eMotion::AttackFinish_Swingdown:
				m_motion = WalkStart;
				m_countTime = 0;
				break;

			case eMotion::AttackStart_Punch:
				// �U���͈͓��Ƀv���C���[��������_���[�W
				if (CheckAttackArea(Punch)) {
					m_playerPtr->AddPlayerDamage(25);
				}
				m_motion = AttackFinish_Punch;
				m_countTime = 0;
				break;

			case eMotion::AttackFinish_Punch:
				m_motion = WalkStart;
				m_countTime = 0;
				break;

			case eMotion::AttackStart_Ramming:
				if (m_stopRammingFlg) {
					m_motion = AttackFinish_Ramming1;
					m_stopRammingFlg = false;
					m_countTime = 0;
					break;
				}
				else {
					m_motion = Attacking_Ramming1;
					m_rammingPos = m_transform->GetPosition();
					m_countTime = 0;
					break;
				}

			case eMotion::Attacking_Ramming1:
				// �J�E���g���U���Ԋu�ɒB�����Ȃ�
				if (m_countTime > m_status.attackInterspace) {
					m_motion = AttackFinish_Ramming1;
					m_countTime = 0;
					break;
				}
				// �ːi�U���𒆎~����t���O��true�Ȃ�
				if (m_stopRammingFlg) {
					m_motion = AttackFinish_Ramming1;
					m_countTime = 0;
					break;
				}
				m_motion = Attacking_Ramming2;
				break;

			case eMotion::Attacking_Ramming2:
				// �J�E���g���U���Ԋu�ɒB�����Ȃ�
				if (m_countTime > m_status.attackInterspace) {
					m_motion = AttackFinish_Ramming1;
					m_countTime = 0;
					break;
				}
				// �ːi�U���𒆎~����t���O��true�Ȃ�
				if (m_stopRammingFlg) {
					m_motion = AttackFinish_Ramming1;
					m_countTime = 0;
					break;
				}
				m_motion = Attacking_Ramming1;
				break;

			case eMotion::AttackFinish_Ramming1:
				m_motion = AttackFinish_Ramming2;
				m_stopRammingFlg = false;
				m_countTime = 0;
				break;

			case eMotion::AttackFinish_Ramming2:
				m_motion = WalkStart;
				m_countTime = 0;
				break;

			case eMotion::AttackFinish_Ramming3:
				m_motion = WalkStart;
				m_countTime = 0;
				break;

			case eMotion::Stun_Ramming_Forward:
				m_motion = Stun1;
				m_countTime = 0;
				break;

			case eMotion::Stun_Ramming_Behind:
				m_motion = Stun1;
				m_countTime = 0;
				break;

			case eMotion::Stun_Normal_Forward:
				m_motion = Stun1;
				m_countTime = 0;
				break;

			case eMotion::Stun_Normal_Behind:
				m_motion = Stun1;
				m_countTime = 0;
				break;

			case eMotion::Stun1:
				if (m_stunTotalTime < m_stunDuration) {
					m_motion = Stun2;
				}
				else {
					m_motion = Stun_Recovery;
				}

				m_stunTotalTime++;
				m_countTime = 0;
				break;

			case eMotion::Stun2:
				if (m_stunTotalTime < m_stunDuration) {
					m_motion = Stun1;
				}
				else {
					m_motion = Stun_Recovery;
				}

				m_stunTotalTime++;
				m_countTime = 0;
				break;

			case eMotion::Stun_Recovery:
				m_motion = WalkStart;
				m_stunDuration = 0;
				m_stunTotalTime = 0;
				m_countTime = 0;
				break;

			case eMotion::Death:
				// �S�[��������������
				PostEvent(1.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToClearStage");
				//GetStage()->RemoveGameObject<Golem>(GetThis<Golem>());
				SetDrawActive(false);
				break;

			default:
				break;
			}
		}

		// �X�^���U�����󂯂Ă��āA���݂̃��[�V�����������𖞂����Ă���ꍇ�A�X�^����ԂɈڍs
		if (m_stunAttackFlg) {
			switch (m_motion)
			{
			case WalkStart:
			case Walking1:
			case Walking2:
			case WalkFinish:
			case AttackFinish_Swingdown:
			case AttackFinish_Punch:
			case AttackFinish_Ramming2:
			case AttackFinish_Ramming3:
			case Stun_Recovery:
				// �v���C���[���O���ɋ����ꍇ
				if (m_rockAngle <= 60.0f) {
					m_motion = Stun_Normal_Forward;
				}
				else {
					m_motion = Stun_Normal_Behind;
				}
				break;

			case AttackFinish_Ramming1:
				if (m_rockAngle <= 60.0f) {
					m_motion = Stun_Ramming_Forward;
				}
				else {
					m_motion = Stun_Ramming_Behind;
				}
				break;

			case Stun1:
			case Stun2:
				break;

			default:
				m_stunDuration = 0;
				break;
			}

			m_stunAttackFlg = false;
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

		// ���[�V�����ɉ����čĐ�����
		switch (m_motion)
		{
		case AttackStart_Punch:
			ptrDraw->UpdateAnimation(deltaTime * 1.5f);
			break;
		default:
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			break;
		}
	}

	void Golem::MoveGolem() {
		// �v���C���[�̃|�C���^���擾
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		// �v���C���[�̍��W���擾
		auto playerPos = ptrPlayer->GetComponent<Transform>()->GetPosition();

		// ���g�̍��W���擾
		auto golemPos = m_transform->GetPosition();
		// �v���C���[�ւ̊p�x(�S�[�����̐��ʕ����)
		float Angle = AngleCalculation(m_playerTrans, true);

		// �f���^�^�C�����擾
		float deltaTime = App::GetApp()->GetElapsedTime();

		if (m_motion == Walking1 || m_motion == Walking2) {
			// �v���C���[�ւ̊p�x�ɉ����ăS�[�������v���C���[�����։�]������
			if (Angle > 0.0f) {
				if (Angle > m_attackStatus.NormalTurningSpeed) {
					m_rotation.y += m_attackStatus.NormalTurningSpeed * (XM_PI / 180.0f);
				}
				else {
					m_rotation.y += Angle * (XM_PI / 180.0f);
				}
			}
			if (Angle < 0.0f) {
				if (Angle < -m_attackStatus.NormalTurningSpeed) {
					m_rotation.y += -m_attackStatus.NormalTurningSpeed * (XM_PI / 180.0f);
				}
				else {
					m_rotation.y += Angle * (XM_PI / 180.0f);
				}
			}

			m_transform->SetRotation(m_rotation);
			m_forward = m_transform->GetForward();
		}

		if (m_motion == Attacking_Ramming1 || m_motion == Attacking_Ramming2) {
			if (Angle > 0) {
				m_rotation.y += m_attackStatus.RammingTurningSpeed * (XM_PI / 180.0f);
			}
			if (Angle < 0) {
				m_rotation.y -= m_attackStatus.RammingTurningSpeed * (XM_PI / 180.0f);
			}

			m_transform->SetRotation(m_rotation);
			m_forward = m_transform->GetForward();
		}

		// �v�Z���������x��m_velocity�ɉ��Z
		m_velocity = m_forward * deltaTime;

		// m_velocity�̑��ʂ�m_status.speed�Ɠ����ɂȂ�悤��������
		m_velocity *= m_status.speed / m_velocity.length();

		// �ːi���͈ړ��ʂ𑝉�������
		if (m_motion == Attacking_Ramming1 || m_motion == Attacking_Ramming2) {
			m_velocity *= m_attackStatus.RammingSpeed;
		}
		// �������Ȃ���~�܂�
		if (m_motion == AttackFinish_Ramming3) {
			if ((120 - m_countTime) * 2 / 120 > 0) {
				m_velocity *= m_attackStatus.RammingSpeed * float((120 - m_countTime) * 2 / 120 > 0);
			}
			else {
				m_velocity = Vec3(0.0f);
			}
		}

		// ���W���X�V
		golemPos -= m_velocity;
		m_transform->SetPosition(golemPos);

		// �ːi�𒆎~���邩���`�F�b�N
		if (m_motion == Attacking_Ramming1 || m_motion == Attacking_Ramming2) {
			// �ːi�U�����v���C���[��ʂ�߂��Ă��邩���`�F�b�N
			float golemMomentum = sqrt((golemPos.x - m_rammingPos.x) * (golemPos.x - m_rammingPos.x) + (golemPos.z - m_rammingPos.z) * (golemPos.z - m_rammingPos.z));
			float playerMomentum = sqrt((playerPos.x - m_rammingPos.x) * (playerPos.x - m_rammingPos.x) + (playerPos.z - m_rammingPos.z) * (playerPos.z - m_rammingPos.z));

			// �ʂ�߂��Ă����ꍇ�͓ːi�𒆎~����
			if (golemMomentum > playerMomentum) {
				m_stopRammingFlg = true;
			}

			// �ːi�U�����Ƀv���C���[�̋߂��ɂ��邩���`�F�b�N
			//float DistancetoPlayer = sqrt((golemPos.x - playerPos.x) * (golemPos.x - playerPos.x) + (golemPos.z - playerPos.z) * (golemPos.z - playerPos.z));

			//// �v���C���[���߂��ɂ����ꍇ�͓ːi�𒆎~����
			//if (DistancetoPlayer < 3.0f) {
			//	m_stopRammingFlg = true;
			//}
		}
	}

	bool Golem::CheckAttackArea(eAttack attack) {
		// �U���͈�
		float attackRange = 0.0f;

		// �U���ɉ����čU���͈͂���
		if (attack == SwingDown) {
			attackRange = m_attackStatus.SwingdownRange;
		}
		if (attack == Punch) {
			attackRange = m_attackStatus.PunchRange;
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

		// �U���͈͂̒��S���W����v���C���[�܂ł̋������v�Z
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

	void Golem::AddStun(int StunTime) {
		// �X�^���U�����󂯂��̂Ńt���O�𗧂Ă�
		m_stunAttackFlg = true;
		// �X�^���̌p�����Ԃ����Z�b�g����
		m_stunDuration = StunTime;
		// �X�^���̍��v���Ԃ����Z�b�g����
		m_stunTotalTime = 0;
	}

	float Golem::AngleCalculation(shared_ptr<Transform>& target, bool negativeValueFlg) {
		// �^�[�Q�b�g�̍��W���擾
		Vec3 targetPos = target->GetPosition();
		// �S�[�����̍��W���擾
		Vec3 golemPos = m_transform->GetPosition();

		// y���W�͍l�����Ȃ�
		targetPos.y = 0.0f;
		golemPos.y = 0.0f;

		// �����̔�r
		float length = (targetPos - golemPos).length();

		// �^�[�Q�b�g�܂ł̍��W
		Vec3 toTarget = targetPos - golemPos;
		toTarget.normalize();

		// �S�[�����̌����Ă������
		Vec3 forward = -m_transform->GetForward();
		forward.normalize();

		// ���ς̌v�Z
		float dot = forward.dot(toTarget);
		dot = min(dot, 1.0f);
		dot = max(dot, -1.0f);

		// �p�x
		float angle = XMConvertToDegrees(acosf(dot));

		float dot2 = (- toTarget.z * forward.x) - (-forward.z * toTarget.x);
		
		if (dot2 < 0) {
			if (negativeValueFlg) {
				angle *= -1;
			}
		}

		return angle;
	}

	void Golem::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		// �v���C���[�Ƃ̏Փ�
		if (Other->FindTag(L"Player"))
		{
			auto ptrColl =GetComponent<CollisionCapsule>();
			ptrColl->SetFixed(true);
			if (m_motion == Attacking_Ramming1 || m_motion == Attacking_Ramming2) {
				if (!m_stopRammingFlg) {
					m_playerPtr->AddPlayerDamage(30);
					m_stopRammingFlg = true;
				}
			}
			return;
		}

		// ���˂��ꂽ��΂Ƃ̏Փ�
		if (Other->FindTag(L"MoveFallingRocks"))
		{
			// �X�^���U�����󂯂�
			AddStun(300);
			m_rockAngle = AngleCalculation(Other->GetComponent<Transform>(), false);
			return;
		}

		if (m_motion == Attacking_Ramming1 || m_motion == Attacking_Ramming2) {
			// �ǂƂ̏Փ�
			if (Other->FindTag(L"Wall"))
			{
				m_stopRammingFlg = true;
				return;
			}

			// ���Ƃ̏Փ�
			if (Other->FindTag(L"FixedCylinder"))
			{
				m_stopRammingFlg = true;
				GetStage()->AddGameObject<FallingRocks>();
				return;
			}

			// ��΂Ƃ̏Փ�
			if (Other->FindTag(L"FallingRocks"))
			{
				m_stopRammingFlg = true;
				return;
			}
		}
	}

	void Golem::OnCollisionExit(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Player"))
		{
			auto ptrColl = GetComponent<CollisionCapsule>();
			ptrColl->SetFixed(false);
			return;
		}

	}

}