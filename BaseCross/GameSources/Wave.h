#pragma once
#include "stdafx.h"


namespace basecross {
	class Wave :public GameObject {
		// �������g�̃g�����X�t�H�[���R���|�[�l���g�iUpdate�����̍œK���̂��߂ɐ錾�j
		std::shared_ptr<Transform> m_transform;

		shared_ptr<EfkEffect> m_sparkEffect;
		shared_ptr<EfkPlay> m_EfkPlay; // �G�t�F�N�g���s�I�u�W�F�N�g
		wstring m_sparkEffectStr;

		float m_time;

		float m_speed;//�e�̃X�s�[�h
		Vec3 m_forward;//�ړ�������\���x�N�g��
		Vec3 m_Velocity;
		std::shared_ptr<GameObject>m_owner;

	public:
		Wave(const std::shared_ptr<Stage>& stage, const shared_ptr<GameObject>& owner) :
			GameObject(stage),
			m_owner(owner),
			m_time(0),
			m_speed(5.0f),
			m_forward(Vec3(0))
		{
		}

		void OnCreate()override;
		void OnUpdate()override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};
}