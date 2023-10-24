#pragma once
#include "stdafx.h"




namespace basecross {
	class Wave :public GameObject {

		// 自分自身のトランスフォームコンポーネント（Update処理の最適化のために宣言）
		std::shared_ptr<Transform> m_transform;

		float m_speed;//弾のスピード
		Vec3 m_forward;//移動方向を表すベクトル
		Vec3 m_Emitter;
		Vec3 m_Velocity;
		std::shared_ptr<GameObject>m_owner;

	public:
		Wave(const std::shared_ptr<Stage>& stage, const shared_ptr<GameObject>& owner) :
			GameObject(stage),
			m_speed(5.0f),
			m_owner(owner),
			m_forward(Vec3(0))
		{
		}

		void OnCreate()override;
		void OnUpdate()override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};
	



}