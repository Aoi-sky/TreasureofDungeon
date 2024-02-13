/*!
@file EnmeySpawn.cpp
@brief “G‚ÌoŒ»
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void EnemySpawn::OnCreate() {
	}

	void EnemySpawn::OnUpdate() {
		m_golemHp = float(GetStage()->GetSharedGameObject<Golem>(L"Golem")->GetGolemCurrentLife());
		
		if (spawnTrigger) {//70%
			if (m_golemHp < 700) {
				Spawn(0);
				spawnTrigger = false;
			}
		}

		if (spawnTrigger2) {//50%
			if (m_golemHp < 500) {
				Spawn(1);
				spawnTrigger2 = false;
			}
		}

		if (spawnTrigger3) {//30%
			if (m_golemHp < 300) {
				Spawn(2);
				spawnTrigger3 = false;
			}
		}
	}

	void EnemySpawn::Spawn(int spawn) {
		switch (spawn) {
		case 0://70%
			GetStage()->AddGameObject<ShotEnemy>(Vec3(10.0f, 1.0f, 20.0f));
			GetStage()->AddGameObject<ShotEnemy>(Vec3(10.0f, 1.0f, -20.0f));
			GetStage()->AddGameObject<ShotEnemy>(Vec3(-10.0f, 1.0f, 20.0f));
			GetStage()->AddGameObject<ShotEnemy>(Vec3(-10.0f, 1.0f, -20.0f));

			GetStage()->AddGameObject<FallingRocks>();
			GetStage()->AddGameObject<FallingRocks>();
			break;
		case 1://50%
			GetStage()->AddGameObject<ShotEnemy>(Vec3(0.0f, 1.0f, 20.0f));
			GetStage()->AddGameObject<ShotEnemy>(Vec3(10.0f, 1.0f, 0.0f));
			GetStage()->AddGameObject<ShotEnemy>(Vec3(-10.0f, 1.0f, 0.0f));
			GetStage()->AddGameObject<ShotEnemy>(Vec3(0.0f, 1.0f, -20.0f));

			GetStage()->AddGameObject<FallingRocks>();
			break;
		case 2://30%
			GetStage()->AddGameObject<ShotEnemy>(Vec3(-10.0f, 1.0f, 10.0f));
			GetStage()->AddGameObject<ShotEnemy>(Vec3(10.0f, 1.0f, 10.0f));
			GetStage()->AddGameObject<ShotEnemy>(Vec3(-10.0f, 1.0f, -10.0f));
			GetStage()->AddGameObject<ShotEnemy>(Vec3(10.0f, 1.0f, -10.0f));
			break;
		default:
			break;
		}
	}
}
//end basecross
