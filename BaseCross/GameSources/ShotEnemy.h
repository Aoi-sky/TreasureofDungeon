/*!
@file ShotEnemy.h
@brief ‹…‚ð”ò‚Î‚µ‚Ä‚­‚é“G
*/

#pragma once
#include "stdafx.h"
#include "Enemy.h"

namespace basecross {
	class ShotEnemy :public GameObject {

		std::shared_ptr<Transform> m_transform;

	public:
		ShotEnemy(const shared_ptr<Stage>& stage);
		virtual ~ShotEnemy();
		

		virtual void OnCreate()override;
		virtual void OnUpdate() override;

	};
}