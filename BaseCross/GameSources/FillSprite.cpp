/*!
@file FillSprite.cpp
@brief スプライト用オブジェクトの実装
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void FillSprite::OnCreate()
	{
		// カラーを設定
		m_color = Col4(1.0f, 1.0f, 1.0f, 1.0f);

		// 頂点データの共通パラメータ
		float halfWidth = m_width * 0.5f; // 半分の幅
		float halfHeight = m_height * 0.5f; // 半分の高さ

		// 頂点データを設定
		m_vertices = {
			{Vec3(-halfWidth, 0.0f,  halfHeight), m_color, Vec2(0.0f, 0.0f)},
			{Vec3(halfWidth, 0.0f, halfHeight), m_color, Vec2(1.0f, 0.0f)},
			{Vec3(-halfWidth, 0.0f, -halfHeight), m_color, Vec2(0.0f, 1.0f)},
			{Vec3(halfWidth, 0.0f, -halfHeight), m_color, Vec2(1.0f, 1.0f)}
		};

		// トランスフォームの設定
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(m_pos);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTrans->SetScale(1.0f, 1.0f, 1.0f);


		// 頂点データとインデックスを元に画像を生成する
		m_drawComp = AddComponent<PCTStaticDraw>();
		m_drawComp->SetOriginalMeshUse(true);
		m_drawComp->CreateOriginalMesh(m_vertices, m_indices);
		m_drawComp->SetTextureResource(m_texKey);

		// 透明色の描画の設定
		SetAlphaActive(true);
	}

	void FillSprite::OnUpdate() {
		m_drawComp->UpdateVertices(m_vertices);
	}


}
