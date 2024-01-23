#pragma once
#include "stdafx.h"


namespace basecross {
	class EnemyBullet :public GameObject {
		// �������g�̃g�����X�t�H�[���R���|�[�l���g�iUpdate�����̍œK���̂��߂ɐ錾�j
		std::shared_ptr<Transform> m_transform;
		float m_speed;//�e�̃X�s�[�h
		Vec3 m_forward;//�ړ�������\���x�N�g��
		int m_damege;//�v���C���[�ɗ^����_���[�W
		Vec3 m_Velocity;
		std::shared_ptr<GameObject>m_owner;

	public:
		EnemyBullet(const std::shared_ptr<Stage>& stage, const shared_ptr<GameObject>& owner) :
			GameObject(stage),
			m_owner(owner),
			m_speed(5.0f),
			m_damege(0),
			m_forward(Vec3(0))
		{
		}

		void MoveEnemyBullet();

		void OnCreate()override;
		void OnUpdate()override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};
}