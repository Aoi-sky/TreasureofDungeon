/*!
@file FillSprite.h
@brief �X�v���C�g�\���p�I�u�W�F�N�g
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class FillSprite : public GameObject
	{
		// ���_�f�[�^
		vector<VertexPositionColorTexture> m_vertices;
		// ���_�C���f�b�N�X
		vector<uint16_t> m_indices = {
			0, 1, 2,
			2, 1, 3
		};
		// �h���[�R���|�[�l���g
		shared_ptr<PCTStaticDraw> m_drawComp;
		// ���S���W
		Vec3 m_pos;
		// ��
		float m_width;
		// ����
		float m_height;
		// �F
		Col4 m_color;
		// �����F��`�悷�邩�̃t���O
		bool m_drawAlpha;
		// �e�N�X�`���L�[
		wstring m_texKey;

	public:
		FillSprite::FillSprite(const shared_ptr<Stage>& stagePtr, // �X�e�[�W�̃|�C���^
			const Vec3& position, // ���S���W
			const float& width, // ��
			const float& height, // ����
			const wstring& texKey // �e�N�X�`���L�[
		) :
			GameObject(stagePtr), // �X�e�[�W�̃|�C���^
			m_pos(position),	 // ���S���W
			m_width(width),	 // ��
			m_height(height), // ����
			m_texKey(texKey) // �e�N�X�`���L�[
		{
		}

		/*!
		@brief �������ɌĂяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���Ăяo�����֐�
		*/
		void OnUpdate() override;
	};

}
//end basecross
