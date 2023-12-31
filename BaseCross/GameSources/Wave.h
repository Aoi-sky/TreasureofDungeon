#pragma once
#include "stdafx.h"


namespace basecross {
	class Wave :public GameObject {
		// 自分自身のトランスフォームコンポーネント（Update処理の最適化のために宣言）
		std::shared_ptr<Transform> m_transform;

		shared_ptr<EfkEffect> m_damageEffect;
		shared_ptr<EfkPlay> m_DamageEfkPlay; // エフェクト実行オブジェクト
		wstring m_damageEffectStr;

		float m_time;

		float m_speed;//弾のスピード
		Vec3 m_forward;//移動方向を表すベクトル
		Vec3 m_Velocity;
		std::shared_ptr<GameObject>m_owner;

	public:
		Wave(const std::shared_ptr<Stage>& stage, const shared_ptr<GameObject>& owner) :
			GameObject(stage),
			m_owner(owner),
			m_time(0),
			m_speed(2.0f),
			m_forward(Vec3(0))
		{
		}

		void OnCreate()override;
		void OnUpdate()override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};
}