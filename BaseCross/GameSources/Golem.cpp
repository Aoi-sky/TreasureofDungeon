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
		m_transform->SetScale(Vec3(3.5f));
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
		ptrDraw->AddAnimation(L"ATTACKSTART_SWINGDOWN", 210, 60, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_SWINGDOWN", 270, 30, false);
		ptrDraw->AddAnimation(L"ATTACKSTART_PUNCH", 310, 60, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_PUNCH", 370, 60, false);
		ptrDraw->AddAnimation(L"ATTACKSTART_RAMMING", 440, 60, false);
		ptrDraw->AddAnimation(L"ATTACKING_RAMMING1", 500, 30, false);
		ptrDraw->AddAnimation(L"ATTACKING_RAMMING2", 500, 30, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_RAMMING", 530, 60, false);
		ptrDraw->AddAnimation(L"DEATH", 600, 1, true);

		// �ŏ��̃A�j���[�V�������w��
		ptrDraw->ChangeCurrentAnimation(L"BOOTING");



		// �e��ǉ�
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMultiMeshResource(L"GOLEM");
		ptrShadow->SetMeshToTransformMatrix(m_differenceMatrix);
		// �^�O�̐ݒ�
		AddTag(L"Enemy");
		AddTag(L"Golem");

	}

	void Golem::OnUpdate() {
		switch (m_motion) {
		case Walking1:
		case Walking2:
		case Attacking_Ramming1:
		case Attacking_Ramming2:
			// �U���p�̃J�E���g�����Z
			m_countTime++;
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

		// �A�j���[�V�������I�����Ă��邩���`�F�b�N
		auto ptrDraw = GetComponent<BcPNTBoneModelDraw>();
		if (ptrDraw->IsTargetAnimeEnd()) {
			if (m_currentMotion == eMotion::Booting) {
				m_motion = WalkStart;
			}

			if (m_currentMotion == eMotion::WalkStart) {
				m_motion = Walking1;
			}

			if (m_currentMotion == eMotion::Walking1) {
				// �J�E���g���U���Ԋu�ɒB������
				if (m_countTime > m_status.attackInterspace) {
					m_motion = WalkFinish;
					m_countTime = 0;
				}
				else {
					m_motion = Walking2;
				}
			}

			if (m_currentMotion == eMotion::Walking2) {
				// �J�E���g���U���Ԋu�ɒB������
				if (m_countTime > m_status.attackInterspace) {
					m_motion = WalkFinish;
					m_countTime = 0;
				}
				else {
					m_motion = Walking1;
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
				if (m_countTime > m_status.attackInterspace || m_stopRammingFlg) {
					m_motion = AttackFinish_Ramming;
					m_stopRammingFlg = false;
				}
				else {
					m_motion = Attacking_Ramming1;
					m_rammingPos = m_transform->GetPosition();
				}
			}

			if (m_currentMotion == eMotion::Attacking_Ramming1) {
				// �J�E���g���U���Ԋu�ɒB����A�܂��͓ːi�U���𒆎~����t���O��true�Ȃ�
				if (m_countTime > m_status.attackInterspace || m_stopRammingFlg) {
					m_motion = AttackFinish_Ramming;
					GetStage()->AddGameObject<FallingRocks>();
					m_countTime = 0;
				}
				else {
					m_motion = Attacking_Ramming2;
				}
			}

			if (m_currentMotion == eMotion::Attacking_Ramming2) {
				// �J�E���g���U���Ԋu�ɒB����A�܂��͓ːi�U���𒆎~����t���O��true�Ȃ�
				if (m_countTime > m_status.attackInterspace || m_stopRammingFlg) {
					m_motion = AttackFinish_Ramming;
					m_countTime = 0;
				}
				else {
					m_motion = Attacking_Ramming1;
				}
			}

			if (m_currentMotion == eMotion::AttackFinish_Ramming) {
				m_motion = WalkStart;
				m_stopRammingFlg = false;
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
		if (m_motion == Booting)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
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
		if (m_motion == Walking1)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == Walking2)
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
		if (m_motion == Attacking_Ramming1)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == Attacking_Ramming2)
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
		// �v���C���[�̃|�C���^���擾
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		// �v���C���[�̍��W���擾
		auto playerPos = ptrPlayer->GetComponent<Transform>()->GetPosition();

		// ���g�̍��W���擾
		auto golemPos = m_transform->GetPosition();

		// �f���^�^�C�����擾
		float deltaTime = App::GetApp()->GetElapsedTime();

		if (m_motion == Walking1 || m_motion == Walking2) {
			// �G�̌��������v���C���[�̕����Ɍ�����
			float rad = -atan2(golemPos.z - playerPos.z, golemPos.x - playerPos.x);

			// �p�x��90�����炷
			Vec3 Rotation(0.0f, rad + XM_PIDIV2, 0.0f);
			// ���݂̌������v���C���[�̕����ւɋ߂Â���
			if (m_rotation.y < Rotation.y) {
				m_rotation.y += 1.5f * (XM_PI / 180.0f);
			}
			if (m_rotation.y > Rotation.y) {
				m_rotation.y -= 1.5f * (XM_PI / 180.0f);
			}

			m_transform->SetRotation(m_rotation);
			m_forward = m_transform->GetForward();
		}

		if (m_motion == Attacking_Ramming1 || m_motion == Attacking_Ramming2) {
			// �G�̌��������v���C���[�̕����Ɍ�����
			float rad = -atan2(golemPos.z - playerPos.z, golemPos.x - playerPos.x);

			// �p�x��90�����炷
			Vec3 Rotation(0.0f, rad + XM_PIDIV2, 0.0f);
			// ���݂̌������v���C���[�̕����ւɋ߂Â���
			if (m_rotation.y < Rotation.y) {
				m_rotation.y += 0.3f * (XM_PI / 180.0f);
			}
			if (m_rotation.y > Rotation.y) {
				m_rotation.y -= 0.3f * (XM_PI / 180.0f);
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
			m_velocity *= 4.0f;
		}

		
		// ���W���X�V
		//golemPos += m_velocity * deltaTime * 0.1f;
		golemPos -= m_velocity;
		m_transform->SetPosition(golemPos);


		// ���݂̍��W��O��̃t���[���̍��W�Ƃ���m_currentPos�ɕۑ�
		m_currentPos = m_transform->GetPosition();

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
			float DistancetoPlayer = sqrt((golemPos.x - playerPos.x) * (golemPos.x - playerPos.x) + (golemPos.z - playerPos.z) * (golemPos.z - playerPos.z));

			// �v���C���[���߂��ɂ����ꍇ�͓ːi�𒆎~����
			if (DistancetoPlayer < 3.0f) {
				m_stopRammingFlg = true;
			}
		}
	}

	bool Golem::CheckAttackArea(eMotion motion) {
		// �U���͈�
		float attackRange;

		// �U���ɉ����čU���͈͂���
		if (motion == AttackStart_Swingdown) {
			attackRange = 8.0f;
		}
		if (motion == AttackStart_Punch) {
			attackRange = 6.0f;
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
		if (playerPos.x != 0) {
			m_motion = Death;
		}
	}

	void Golem::AddDamage(int Damage) {
		m_status.life -= Damage;
	}

	void Golem::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		// �v���C���[�Ƃ̏Փ�
		if (Other->FindTag(L"Player"))
		{
			auto ptrColl =GetComponent<CollisionCapsule>();
			ptrColl->SetFixed(true);
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
				return;
			}

			// ���΂Ƃ̏Փ�
			if (Other->FindTag(L"FallingRocks"))
			{
				m_stopRammingFlg = true;
				return;
			}
		}
		if (Other->FindTag(L"MoveFallingRocks"))
		{
			GetStage()->RemoveGameObject<Golem>(GetThis<Golem>());
			PostEvent(1.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToClearStage");
			return;
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