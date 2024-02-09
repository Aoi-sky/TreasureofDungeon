/*!
@file PlayerHpGauge.h
@brief プレイヤーのHPバー
*/

#pragma once
#include "stdafx.h"
#include "Geuge.h"
#include <vector>

namespace basecross {
	class PlayerStaminaBar : public GameObject {

		shared_ptr<Gauge> m_bar[2];// スタミナバーの情報を入れる変数
	public:
		// コンストラクタ
		PlayerStaminaBar(const std::shared_ptr<Stage>& stage) :
			GameObject(stage)
		{
		}

		void OnCreate() override;// 初期化
		void OnUpdate() override;// 更新処理
		void OnDraw() override;	// 描画処理
	};
}
//end basecross

