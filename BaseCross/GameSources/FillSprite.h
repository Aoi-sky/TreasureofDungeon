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
		vector<VertexPositionColor> m_vertices;
		// 頂点インデックス
		std::vector<uint16_t> m_indices;
		// 表示順を保存するインデックス
		vector<uint16_t> m_baseIndices = {
			0, 1, 2
		};
		// ドローコンポーネント
		shared_ptr<PCTStaticDraw> m_drawComp;
		// 中心座標
		Vec3 m_centerPos;
		// 半径
		float m_radius;
		// 色
		Col4 m_color;
		// 消去されるまでのフレーム数
		int m_maxCount;
		// 生成されてから経過したフレーム数
		int m_count;


	public:
		FillSprite::FillSprite(const shared_ptr<Stage>& stagePtr, // ステージのポインタ
			const Vec3& position, // 中心座標
			const float& radius, // 半径
			const int& maxCount // フレーム数
		) :
			GameObject(stagePtr), // ステージのポインタ
			m_centerPos(position),	 // 中心座標
			m_radius(radius), // 半径
			m_maxCount(maxCount) // フレーム数
		{
			m_count = 0;

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
