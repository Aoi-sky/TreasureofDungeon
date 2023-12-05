/* !
@file Gauge.h
@brief 各種ゲージの頂点データなどの設定
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Gauge : public GameObject
	{
		
		const Vec2 m_DefaultSize;// Gaugeの初期サイズ
		const int m_SquareVartex;// 頂点の数
		const wstring m_TextureKey;// テクスチャキー

		// スプライトの元になる頂点データと頂点インデックス
		vector<VertexPositionColorTexture> m_vertices;
		vector<uint16_t> m_indices;

		// ドローコンポーネントとトランスフォームコンポーネントを取得
		shared_ptr<PCTSpriteDraw> m_draw;
		shared_ptr<Transform> m_transform;

		int m_uvPosition; // 表示される位置を表す値
		float m_gaugeWidth; // 一単位毎に伸ばす・減らすゲージの横幅
		float m_uvShiftDistance; // 1ゲージごとにずらすUV座標の差

	public:
		Gauge(const shared_ptr<Stage>& stage, float defaultGaugeWidth, const wstring& textureKey) :
			GameObject(stage),
			m_DefaultSize(Vec2(defaultGaugeWidth, 24.0f)),
			m_TextureKey(textureKey),
			m_SquareVartex(4),
			m_uvPosition(1),
			m_gaugeWidth(0),
			m_uvShiftDistance(0.1f)
		{
		}
		// 初期化 
		void OnCreate() override;
		void UpdateGaugeSize(int gaugeSizeLimit, float currentGaugeSize);
		void SetVartices();
		virtual void SetPosition(const Vec3& pos, float h);
		virtual void SetPosition(const Vec2& pos, float h)
		{
			SetPosition(Vec3(pos.x, pos.y, 0.0f), h);
		}
		
		void SetPosition(float x, float y, float h)
		{
			SetPosition(Vec2(x, y), h);
		}
	};
}