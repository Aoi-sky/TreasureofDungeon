/*!
@file GameOverStage.h
@brief ゲームオーバー画面
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class GameOverStage :public Stage {
		shared_ptr<SoundItem> m_bgm; // サウンドアイテム

		void CreateViewLight();
		void CreateGameOverSprite();
		void PlayBGM();

	public:
		GameOverStage() :
			Stage()
		{
		}
		virtual ~GameOverStage() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDestroy()override;
	};

}
//end basecross
