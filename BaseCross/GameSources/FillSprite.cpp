/*!
@file FillSprite.cpp
@brief �X�v���C�g�p�I�u�W�F�N�g�̎���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void FillSprite::OnCreate()
	{
		// �\������~�̐ݒ�
		// �F�̐ݒ�
		m_color = Col4(1.0f, 0.0f, 0.0f, 0.5f);
		// �~�̕�����
		const int numAngles = 60;


		// �o�[�e�b�N�X��ݒ�
		// �o�[�e�b�N�X�̍��W
		Vec3 pos;
		// �o�[�e�b�N�X�̍��W�ƐF
		VertexPositionColor vertex;
		// ���W�A���p
		float rad;


		// ���S�ƂȂ�o�[�e�b�N�X��ݒ�
		m_vertices.push_back(VertexPositionColor(Vec3(0.0f), m_color));

		// 0�x�����ɖ߂��ׂɕ�����+2�񃋁[�v����
		for (int i = 0; i < numAngles + 2; i++) {
			// �����������Ɋp�x���v�Z
			rad = XMConvertToRadians(float(i * (360 / numAngles)));

			pos = Vec3(cosf(rad), 0.0f, sinf(rad)) * m_radius;
			vertex = VertexPositionColor(pos, m_color); // �����̉~

			// ���_�f�[�^��ݒ�
			m_vertices.push_back(vertex);
		}

		for (int i = 0; i < numAngles; i++) {
			// baseIndices�̐��������[�v
			for (int j = 0; j < 3; j++) {
				if (j == 0) {
					m_indices.push_back(0);
				}
				else {
					m_indices.push_back(j + i);
				}
			}
		}


		// �g�����X�t�H�[���̐ݒ�
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(m_centerPos);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTrans->SetScale(1.0f, 1.0f, 1.0f);


		// ���_�f�[�^�ƃC���f�b�N�X�����ɉ摜�𐶐�����
		m_drawComp = AddComponent<PCTStaticDraw>();
		m_drawComp->SetOriginalMeshUse(true);
		m_drawComp->CreateOriginalMesh(m_vertices, m_indices);
		m_drawComp->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 0.5f));

		// �����F�̕`��̐ݒ�
		SetAlphaActive(true);
	}

	void FillSprite::OnUpdate() {
		// �o�߃t���[���������Ŏ��Ԃ𒴂��Ă��������
		if (m_count >= m_maxCount) {
			GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
		}
		m_drawComp->UpdateVertices(m_vertices);
		m_count++;
	}


}
