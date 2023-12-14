/*!
@file ClearStage.h
@brief クリア画面
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class ClearStage :public Stage {
		shared_ptr<SoundItem> m_bgm; // サウンドアイテム

		void CreateViewLight();
		void CreateClearSprite();
		void PlayBGM();
	public:
		ClearStage():
			Stage()
		{
		}
		virtual ~ClearStage() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDestroy()override;

	};
}
//end basecross
