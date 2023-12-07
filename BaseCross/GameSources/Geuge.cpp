/*!
@file Gauge.cpp
@brief �e��Q�[�W�̒��_�f�[�^�Ȃǂ̐ݒ�̎���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void Gauge::OnCreate(){
		SetVartices();// ���_�f�[�^�̐���

		m_indices = {
			0, 1, 2, // ����̎O�p(�|���S��)
			2, 1, 3  // �E���̎O�p(�|���S��)
		};

		m_transform = GetComponent<Transform>();// �g�����X�t�H�[���R���|�[�l���g�̎擾
		// �����ڂ̐ݒ�
		m_draw = AddComponent<PCTSpriteDraw>(m_vertices, m_indices);
		m_draw->SetTextureResource(m_TextureKey);

		// �`�惌�C���[��ݒ�
		SetDrawLayer((int)DrawLayer::ForeFront);
		SetAlphaActive(true);// ���ߏ�����L���ɂ���
	}

	void Gauge::SetVartices(){
		const Col4 white(1.0f, 1.0f, 1.0f, 1.0f); // �F��(�摜�t�@�C���̐F���̂܂�)��

		m_vertices = {// ���_�f�[�^�̐ݒ�
			{Vec3(0.0f			 , 0.0f			  , 0.0f), white, Vec2(0.0f, 0.0f)}, // ����
			{Vec3(m_DefaultSize.x, 0.0f			  , 0.0f), white, Vec2(1.0f, 0.0f)}, // �E��
			{Vec3(0.0f			 ,-m_DefaultSize.y, 0.0f), white, Vec2(0.0f, 1.0f)}, // ����
			{Vec3(m_DefaultSize.x,-m_DefaultSize.y, 0.0f), white, Vec2(1.0f, 1.0f)}, // �E��
		};
	}

	void Gauge::UpdateGaugeSize(int gaugeSizeLimit, float currentGaugeSize){
		float gaugeSizeDiff = m_DefaultSize.x / gaugeSizeLimit;// 1���Ƃ̃Q�[�W�̃T�C�Y���v�Z

		for (int i = 0; i < m_SquareVartex; i++)// ���_�f�[�^���X�V
		{
			// �e�N�X�`������HPBAR_GREEN�܂���HPBAR_RED�Ȃ�
			if (m_TextureKey == L"GREENHPBAR" || m_TextureKey == L"REDHPBAR"){// �E�����猸�炷
				float currentLostGauge = gaugeSizeLimit - currentGaugeSize;

				if (i % 2 == 1 && currentGaugeSize <= gaugeSizeLimit){
					m_vertices[i].position.x = m_DefaultSize.x - gaugeSizeDiff * currentLostGauge;
				}
			}
			else {
				if (i % 2 == 1 && currentGaugeSize <= gaugeSizeLimit){// �������瑝�₷
					m_vertices[i].position.x = gaugeSizeDiff * currentGaugeSize;
				}
			}
		}

		// �X�V���ꂽ�f�[�^�����Ƀ|���S������蒼��
		m_draw->UpdateVertices(m_vertices);
	}

	void Gauge::SetPosition(const Vec3& pos, float h){
		// ��ʂ̕��ƍ������擾����
		auto& app = App::GetApp();
		auto width = app->GetGameWidth();
		auto height = app->GetGameHeight();

		// ���_����ʍ���ɂ���B�܂��AY���W�͉������ɔ��]������
		m_transform->SetPosition(Vec3(-pos.x + (width * 0.5f), -pos.y + (height * h), pos.z));
	}
}