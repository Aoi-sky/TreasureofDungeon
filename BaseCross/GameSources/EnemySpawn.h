/*!
@file EnemySpawn.h
@brief ìGÇÃèoåª
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class EnemySpawn :public GameObject {

		float m_golemHp;

		bool spawnTrigger;//70%
		bool spawnTrigger2;//50%
		bool spawnTrigger3;//30%

	public:
		EnemySpawn(const std::shared_ptr<Stage>& stage) :
			GameObject(stage),
			m_golemHp(1000.0f),
			spawnTrigger(true),
			spawnTrigger2(true),
			spawnTrigger3(true)
		{
		}

		void OnCreate() override;
		void OnUpdate() override;

		void Spawn(int spawn);
	};

}
//end basecross