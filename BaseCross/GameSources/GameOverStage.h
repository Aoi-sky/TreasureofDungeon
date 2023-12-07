/*!
@file GameOverStage.h
@brief �Q�[���I�[�o�[���
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
