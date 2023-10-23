#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//�@�G(Enemy)
	//--------------------------------------------------------------------------------------
	class Enemy : public GameObject {
		Vec3 m_StartPosition;
		Vec3 m_Force;
		Vec3 m_Velocity;
		weak_ptr<StageCellMap> m_CelMap;
		vector<CellIndex> m_CellPath;
		//���݂̎����̃Z���C���f�b�N�X
		int m_CellIndex;
		//�߂����i���́j�̃Z���C���f�b�N�X
		int m_NextCellIndex;
		//�^�[�Q�b�g�̃Z���C���f�b�N�X
		int m_TargetCellIndex;
	public:
		Enemy(const shared_ptr<Stage>& StagePtr, const shared_ptr<StageCellMap>& CellMap, const Vec3& Position);
		Enemy();
		//������
		virtual void OnCreate()override;
		virtual void OnUpdate() override;
		//�X�^�[�g�n�_
		const Vec3& GetStartPosition()const {
			return m_StartPosition;
		}
		//�ړI�̏ꏊ���T�[�`��Seek�s��������
		bool Search(const Vec3& TargetPos);
		//�ړI�̏ꏊ��arrive�s�����Ƃ�
		void ArriveBehavior(const Vec3& TargetPos);
	};
}
