/*!
@file FillSprite.cpp
@brief スプライト用オブジェクトの実装
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void FillSprite::OnCreate()
	{
		// 表示する円の設定
		// 色の設定
		m_color = Col4(1.0f, 0.0f, 0.0f, 0.5f);
		// 円の分割数
		const int numAngles = 60;


		// バーテックスを設定
		// バーテックスの座標
		Vec3 pos;
		// バーテックスの座標と色
		VertexPositionColor vertex;
		// ラジアン角
		float rad;


		// 中心となるバーテックスを設定
		m_vertices.push_back(VertexPositionColor(Vec3(0.0f), m_color));

		// 0度方向に戻す為に分割数+2回ループする
		for (int i = 0; i < numAngles + 2; i++) {
			// 分割数を元に角度を計算
			rad = XMConvertToRadians(float(i * (360 / numAngles)));

			pos = Vec3(cosf(rad), 0.0f, sinf(rad)) * m_radius;
			vertex = VertexPositionColor(pos, m_color); // 内側の円

			// 頂点データを設定
			m_vertices.push_back(vertex);
		}

		for (int i = 0; i < numAngles; i++) {
			// baseIndicesの数だけループ
			for (int j = 0; j < 3; j++) {
				if (j == 0) {
					m_indices.push_back(0);
				}
				else {
					m_indices.push_back(j + i);
				}
			}
		}


		// トランスフォームの設定
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(m_centerPos);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTrans->SetScale(1.0f, 1.0f, 1.0f);


		// 頂点データとインデックスを元に画像を生成する
		m_drawComp = AddComponent<PCTStaticDraw>();
		m_drawComp->SetOriginalMeshUse(true);
		m_drawComp->CreateOriginalMesh(m_vertices, m_indices);
		m_drawComp->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 0.5f));

		// 透明色の描画の設定
		SetAlphaActive(true);
	}

	void FillSprite::OnUpdate() {
		// 経過フレーム数が消滅時間を超えていたら消去
		if (m_count >= m_maxCount) {
			GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
		}
		m_drawComp->UpdateVertices(m_vertices);
		m_count++;
	}


}
