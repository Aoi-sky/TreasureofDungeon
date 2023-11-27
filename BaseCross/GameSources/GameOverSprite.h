/* !
@file GameOverSprite.h
@brief ゲームオーバーシーンの背景
*/

#pragma once
#include "stdafx.h"
#include "Sprites.h"

namespace basecross {
	class GameOverSprite : public Sprites {
		const Vec2 m_DefaultSize; // 初期サイズ

	public:
		// コンストラクタ
		GameOverSprite(const shared_ptr<Stage>& stage);
		// デストラクタ
		~GameOverSprite();

		void OnCreate() override; // 初期化
		void OnUpdate() override; // 更新処理

	};
}