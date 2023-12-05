/*!
@file FillSprite.h
@brief スプライト表示用オブジェクト
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class FillSprite : public GameObject
	{
		// 頂点データ
		vector<VertexPositionColorTexture> m_vertices;
		// 頂点インデックス
		vector<uint16_t> m_indices = {
			0, 1, 2,
			2, 1, 3
		};
		// ドローコンポーネント
		shared_ptr<PCTStaticDraw> m_drawComp;
		// 中心座標
		Vec3 m_pos;
		// 幅
		float m_width;
		// 高さ
		float m_height;
		// 色
		Col4 m_color;
		// 透明色を描画するかのフラグ
		bool m_drawAlpha;
		// テクスチャキー
		wstring m_texKey;

	public:
		FillSprite::FillSprite(const shared_ptr<Stage>& stagePtr, // ステージのポインタ
			const Vec3& position, // 中心座標
			const float& width, // 幅
			const float& height, // 高さ
			const wstring& texKey // テクスチャキー
		) :
			GameObject(stagePtr), // ステージのポインタ
			m_pos(position),	 // 中心座標
			m_width(width),	 // 幅
			m_height(height), // 高さ
			m_texKey(texKey) // テクスチャキー
		{
		}

		/*!
		@brief 生成時に呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム呼び出される関数
		*/
		void OnUpdate() override;
	};

}
//end basecross
