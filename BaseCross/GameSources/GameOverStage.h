/*!
@file GameOverStage.h
@brief ゲームオーバー画面
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class GameOverStage :public Stage {

		void CreateViewLight();
		void CreateGameOverSprite();

	public:
		GameOverStage() :
			Stage()
		{
		}
		virtual ~GameOverStage() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

}
//end basecross
