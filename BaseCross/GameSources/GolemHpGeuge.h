/*!
@file GolemHpGauge.h
@brief プレイヤーのHPバー
*/

#pragma once
#include "stdafx.h"
#include "Geuge.h"
#include <vector>

namespace basecross {

	class GolemHpBar : public GameObject
	{
		// 最大HP
		float m_maxHP;

		// HPバーの情報を入れる変数
		shared_ptr<Gauge> m_bar[2];
	public:
		// コンストラクタ
		GolemHpBar(const std::shared_ptr<Stage>& stage) :
			GameObject(stage),
			m_maxHP(0)
		{
		}

		void OnCreate() override;// 初期化
		void OnUpdate() override;// 更新処理
		void OnDraw() override;// 描画処理
	};
}
//end basecross