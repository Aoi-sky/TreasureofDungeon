/* !
@file ClearSprite.h
@brief クリアシーンの背景の実装
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	// コンストラクタ
	ClearSprite::ClearSprite(const shared_ptr<Stage>& stage) :
		Sprites(stage),
		m_DefaultSize(Vec2((float)App::GetApp()->GetGameWidth(), (float)App::GetApp()->GetGameHeight()))
	{
	}
	// デストラクタ
	ClearSprite::~ClearSprite() {}

	// 初期化
	void ClearSprite::OnCreate()
	{
		Sprites::CreateSprite(Vec3(-m_DefaultSize.x / 2, m_DefaultSize.y / 2, 0.0f), m_DefaultSize, L"CLEAR");
	}

	// 更新処理
	void ClearSprite::OnUpdate()
	{
		Sprites::OnUpdate();
	}
}