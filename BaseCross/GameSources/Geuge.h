/* !
@file Gauge.h
@brief �e��Q�[�W�̒��_�f�[�^�Ȃǂ̐ݒ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Gauge : public GameObject
	{
		
		const Vec2 m_DefaultSize;// Gauge�̏����T�C�Y
		const int m_SquareVartex;// ���_�̐�
		const wstring m_TextureKey;// �e�N�X�`���L�[

		// �X�v���C�g�̌��ɂȂ钸�_�f�[�^�ƒ��_�C���f�b�N�X
		vector<VertexPositionColorTexture> m_vertices;
		vector<uint16_t> m_indices;

		// �h���[�R���|�[�l���g�ƃg�����X�t�H�[���R���|�[�l���g���擾
		shared_ptr<PCTSpriteDraw> m_draw;
		shared_ptr<Transform> m_transform;

		int m_uvPosition; // �\�������ʒu��\���l
		float m_gaugeWidth; // ��P�ʖ��ɐL�΂��E���炷�Q�[�W�̉���
		float m_uvShiftDistance; // 1�Q�[�W���Ƃɂ��炷UV���W�̍�

	public:
		Gauge(const shared_ptr<Stage>& stage, float defaultGaugeWidth, const wstring& textureKey) :
			GameObject(stage),
			m_DefaultSize(Vec2(defaultGaugeWidth, 24.0f)),
			m_TextureKey(textureKey),
			m_SquareVartex(4),
			m_uvPosition(1),
			m_gaugeWidth(0),
			m_uvShiftDistance(0.1f)
		{
		}
		// ������ 
		void OnCreate() override;
		void UpdateGaugeSize(int gaugeSizeLimit, float currentGaugeSize);
		void SetVartices();
		virtual void SetPosition(const Vec3& pos, float h);
		virtual void SetPosition(const Vec2& pos, float h)
		{
			SetPosition(Vec3(pos.x, pos.y, 0.0f), h);
		}
		
		void SetPosition(float x, float y, float h)
		{
			SetPosition(Vec2(x, y), h);
		}
	};
}