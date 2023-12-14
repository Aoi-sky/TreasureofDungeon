/*!
@file GameOverStage.h
@brief �Q�[���I�[�o�[���
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class GameOverStage :public Stage {
		shared_ptr<SoundItem> m_bgm; // �T�E���h�A�C�e��

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
