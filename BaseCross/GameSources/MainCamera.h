#pragma once
#include "stdafx.h"


namespace basecross {
	class MainCamera :public Camera
	{
		float m_angle;//カメラの回り込み
		float m_distance;//カメラまでの距離

		std::shared_ptr<Transform>m_targetTrans;//視点のポリゴン

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