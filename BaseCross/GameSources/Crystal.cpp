/*!
@file Crystal.cpp
@brief �����{��
@author �����
*/
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	// �����N���X
	//--------------------------------------------------------------------------------------
	void Crystal::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		m_transform = GetComponent<Transform>();
		m_transform->SetScale(Vec3(2.0f));
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
		ptrDraw->SetMeshResource(L"CRYSTAL_R");
		ptrDraw->SetMeshToTransformMatrix(m_differenceMatrix);
		ptrDraw->SetSpecularColor(Col4(1, 1, 1, 1));
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetLightingEnabled(false);

		// �e��ǉ�
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"CRYSTAL_R");
		ptrShadow->SetMeshToTransformMatrix(m_differenceMatrix);

		// �^�O�̐ݒ�
		AddTag(L"Crystal");

		m_currentState = Wait;
		m_pastState = Wait;
	}

	void Crystal::OnUpdate() {
		CheckState();



		// �J�E���g�����Z
		m_elapsedTime++;
		// ���݂̏�Ԃ�O�t���[���̏�ԂƂ��ĕۑ�
		m_pastState = m_currentState;
	}

	void Crystal::CheckState() {
		// �o�ߎ��Ԃ��ҋ@���Ԃ��o�߂����ꍇ�̏���
		if (m_standbyTime <= m_elapsedTime && m_pastState == Wait) {
			m_currentState = Move;
		}



	}
}