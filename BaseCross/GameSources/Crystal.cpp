/*!
@file Crystal.cpp
@brief �����{��
@author �����
*/
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void Crystal::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		m_transform = GetComponent<Transform>();
		m_transform->SetScale(Vec3(2.5f));
		m_transform->SetRotation(Vec3(0.0f));
		m_transform->SetPosition(m_startPos);

		//CollisionCapsule�Փ˔����t����
		auto ptrColl = AddComponent<CollisionCapsule>();
		ptrColl->SetDrawActive(true);
		ptrColl->SetFixed(false);

		//�`��R���|�[�l���g�̐ݒ�
		SetAlphaActive(true);
		SetDrawActive(true);

		// �V�K�h���[�R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<BcPNStaticDraw>();
		ptrDraw->SetMeshResource(L"CRYSTAL");
		ptrDraw->SetMeshToTransformMatrix(m_differenceMatrix);
		ptrDraw->SetSpecularColor(Col4(1, 1, 1, 1));
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetLightingEnabled(false);

		// �e��ǉ�
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"CRYSTAL");
		ptrShadow->SetMeshToTransformMatrix(m_differenceMatrix);

		// �^�O�̐ݒ�
		AddTag(L"Crystal");

		// �X�s�[�h�̌v�Z


		// �U���͈͂̕`��


	}

	void Crystal::OnUpdate() {
		// ElapsedTime���擾
		m_elapsedTime = App::GetApp()->GetElapsedTime();

		// ���݂̏�Ԃɉ���������������
		StateManagement();



		// �o�ߎ��Ԃ����Z
		m_progressTime += m_elapsedTime;

		// ���݂̏�Ԃ�O�t���[���̏�ԂƂ��ĕۑ�
		m_pastState = m_currentState;
	}

	void Crystal::OnMove() {
		// ���W���擾
		auto pos = m_transform->GetPosition();
		// ElapsedTime���擾
		float elapsedTime = App::GetApp()->GetElapsedTime();



	}

	void Crystal::StateManagement() {
		switch (m_pastState)
		{
		case Wait:
			// �o�ߎ��Ԃ��ҋ@���Ԃ��o�߂����ꍇ�̏���
			if (m_standbyTime > m_progressTime) {
				m_currentState = Move;
			}
			break;
		case Move:
			// �ړ�����
			OnMove();

			// �o�ߎ��Ԃ��ҋ@���Ԃ��o�߂����ꍇ�̏���
			if (m_equiredTime > m_progressTime - m_standbyTime) {
				m_currentState = Move;
			}
			// ���������ꍇ�A�����̏�Ԃ�Finish�ɂ���
			if (m_isHit) {
				m_currentState = Finish;
			}
			break;
		case Attack:
			// �o�ߎ��Ԃ�Attack�̎������Ԃ��o�߂����ꍇ�̏���
			if (m_duration <= m_progressTime - (m_standbyTime + m_equiredTime)) {
				m_currentState = Finish;
			}
			// ���������ꍇ�A�����̏�Ԃ�Finish�ɂ���
			if (m_isHit) {
				m_currentState = Finish;
			}

			break;
		case Finish:

			break;
		default:
			m_currentState = Wait;
			break;
		}




	}
}