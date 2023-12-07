/*!
@file Gauge.cpp
@brief 各種ゲージの頂点データなどの設定の実装
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void Gauge::OnCreate(){
		SetVartices();// 頂点データの生成

		m_indices = {
			0, 1, 2, // 左上の三角(ポリゴン)
			2, 1, 3  // 右下の三角(ポリゴン)
		};

		m_transform = GetComponent<Transform>();// トランスフォームコンポーネントの取得
		// 見た目の設定
		m_draw = AddComponent<PCTSpriteDraw>(m_vertices, m_indices);
		m_draw->SetTextureResource(m_TextureKey);

		// 描画レイヤーを設定
		SetDrawLayer((int)DrawLayer::ForeFront);
		SetAlphaActive(true);// 透過処理を有効にする
	}

	void Gauge::SetVartices(){
		const Col4 white(1.0f, 1.0f, 1.0f, 1.0f); // 色を白(画像ファイルの色そのまま)に

		m_vertices = {// 頂点データの設定
			{Vec3(0.0f			 , 0.0f			  , 0.0f), white, Vec2(0.0f, 0.0f)}, // 左上
			{Vec3(m_DefaultSize.x, 0.0f			  , 0.0f), white, Vec2(1.0f, 0.0f)}, // 右上
			{Vec3(0.0f			 ,-m_DefaultSize.y, 0.0f), white, Vec2(0.0f, 1.0f)}, // 左下
			{Vec3(m_DefaultSize.x,-m_DefaultSize.y, 0.0f), white, Vec2(1.0f, 1.0f)}, // 右下
		};
	}

	void Gauge::UpdateGaugeSize(int gaugeSizeLimit, float currentGaugeSize){
		float gaugeSizeDiff = m_DefaultSize.x / gaugeSizeLimit;// 1ごとのゲージのサイズを計算

		for (int i = 0; i < m_SquareVartex; i++)// 頂点データを更新
		{
			// テクスチャ名がHPBAR_GREENまたはHPBAR_REDなら
			if (m_TextureKey == L"GREENHPBAR" || m_TextureKey == L"REDHPBAR"){// 右側から減らす
				float currentLostGauge = gaugeSizeLimit - currentGaugeSize;

				if (i % 2 == 1 && currentGaugeSize <= gaugeSizeLimit){
					m_vertices[i].position.x = m_DefaultSize.x - gaugeSizeDiff * currentLostGauge;
				}
			}
			else {
				if (i % 2 == 1 && currentGaugeSize <= gaugeSizeLimit){// 左側から増やす
					m_vertices[i].position.x = gaugeSizeDiff * currentGaugeSize;
				}
			}
		}

		// 更新されたデータを元にポリゴンを作り直す
		m_draw->UpdateVertices(m_vertices);
	}

	void Gauge::SetPosition(const Vec3& pos, float h){
		// 画面の幅と高さを取得する
		auto& app = App::GetApp();
		auto width = app->GetGameWidth();
		auto height = app->GetGameHeight();

		// 原点を画面左上にする。また、Y座標は下向きに反転させる
		m_transform->SetPosition(Vec3(-pos.x + (width * 0.5f), -pos.y + (height * h), pos.z));
	}
}