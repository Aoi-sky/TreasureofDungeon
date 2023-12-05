/*!
@file FillSprite.cpp
@brief �X�v���C�g�p�I�u�W�F�N�g�̎���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void FillSprite::OnCreate()
	{
		// �J���[��ݒ�
		m_color = Col4(1.0f, 1.0f, 1.0f, 1.0f);

		// ���_�f�[�^�̋��ʃp�����[�^
		float halfWidth = m_width * 0.5f; // �����̕�
		float halfHeight = m_height * 0.5f; // �����̍���

		// ���_�f�[�^��ݒ�
		m_vertices = {
			{Vec3(-halfWidth, 0.0f,  halfHeight), m_color, Vec2(0.0f, 0.0f)},
			{Vec3(halfWidth, 0.0f, halfHeight), m_color, Vec2(1.0f, 0.0f)},
			{Vec3(-halfWidth, 0.0f, -halfHeight), m_color, Vec2(0.0f, 1.0f)},
			{Vec3(halfWidth, 0.0f, -halfHeight), m_color, Vec2(1.0f, 1.0f)}
		};

		// �g�����X�t�H�[���̐ݒ�
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(m_pos);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTrans->SetScale(1.0f, 1.0f, 1.0f);


		// ���_�f�[�^�ƃC���f�b�N�X�����ɉ摜�𐶐�����
		m_drawComp = AddComponent<PCTStaticDraw>();
		m_drawComp->SetOriginalMeshUse(true);
		m_drawComp->CreateOriginalMesh(m_vertices, m_indices);
		m_drawComp->SetTextureResource(m_texKey);

		// �����F�̕`��̐ݒ�
		SetAlphaActive(true);
	}

	void FillSprite::OnUpdate() {
		m_drawComp->UpdateVertices(m_vertices);
	}


}
