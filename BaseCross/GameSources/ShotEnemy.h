/*!
@file ShotEnemy.h
@brief ’e‚ð”ò‚Î‚µ‚Ä‚­‚é“G
*/

#pragma once
#include "stdafx.h"
#include "Enemy.h"

namespace basecross {
	class ShotEnemy :public GameObject {

		std::shared_ptr<Transform> m_transform;

		Vec3 m_position;
		Vec3 m_rot;
		float m_speed;


		const float m_RecastCount; // ’e”­ŽËŠÔŠu
		float m_recastFlame;
	public:
		ShotEnemy(const shared_ptr<Stage>& stage,const Vec3& Position);
		virtual ~ShotEnemy();
		
		virtual void OnCreate()override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

	};
}