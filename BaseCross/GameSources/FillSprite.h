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
		vector<VertexPositionColor> m_vertices;
		// ���_�C���f�b�N�X
		std::vector<uint16_t> m_indices;
		// �\������ۑ�����C���f�b�N�X
		vector<uint16_t> m_baseIndices = {
			0, 1, 2
		};
		// �h���[�R���|�[�l���g
		shared_ptr<PCTStaticDraw> m_drawComp;
		// ���S���W
		Vec3 m_centerPos;
		// ���a
		float m_radius;
		// �F
		Col4 m_color;
		// ���������܂ł̃t���[����
		int m_maxCount;
		// ��������Ă���o�߂����t���[����
		int m_count;


	public:
		FillSprite::FillSprite(const shared_ptr<Stage>& stagePtr, // �X�e�[�W�̃|�C���^
			const Vec3& position, // ���S���W
			const float& radius, // ���a
			const int& maxCount // �t���[����
		) :
			GameObject(stagePtr), // �X�e�[�W�̃|�C���^
			m_centerPos(position),	 // ���S���W
			m_radius(radius), // ���a
			m_maxCount(maxCount) // �t���[����
		{
			m_count = 0;

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
