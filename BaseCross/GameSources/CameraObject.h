#pragma once
#include "stdafx.h"


namespace basecross {
	class CameraObject :public GameObject {
		std::shared_ptr<Transform> m_transform;

		std::shared_ptr<Camera>m_owner;

	public:
		CameraObject(const std::shared_ptr<Stage>& stage, const shared_ptr<Camera>& owner) :
			GameObject(stage),
			m_owner(owner)
		{
		}

		void OnCreate()override;
		void OnUpdate()override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};
}