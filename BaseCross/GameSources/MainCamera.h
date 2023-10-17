#pragma once
#include "stdafx.h"


namespace basecross {
	class MainCamera :public Camera
	{
		float m_angle;//�J�����̉�荞��
		float m_distance;//�J�����܂ł̋���

		std::shared_ptr<Transform>m_targetTrans;//���_�̃|���S��

	public :
		MainCamera() :
			m_angle(XMConvertToRadians(270.0f)),
			m_distance(8.0f)
		{
		}

		void OnUpdate();

		float GetAngle() const
		{
			return m_angle;
		}

		void SetTarget(const std::shared_ptr<GameObject>& target)
		{
			m_targetTrans = target->GetComponent<Transform>();
		}
	};

}