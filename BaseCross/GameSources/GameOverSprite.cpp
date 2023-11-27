/* !
@file GameOverSprite.h
@brief ゲームオーバーシーンの背景の実装
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	// コンストラクタ
	GameOverSprite::GameOverSprite(const shared_ptr<Stage>& stage) :
		Sprites(stage),
		m_DefaultSize(Vec2((float)App::GetApp()->GetGameWidth(), (float)App::GetApp()->GetGameHeight()))
	{
	}
	// デストラクタ
	GameOverSprite::~GameOverSprite() {}

	// 初期化
	void GameOverSprite::OnCreate()
	{
		Sprites::CreateSprite(Vec3(-m_DefaultSize.x / 2, m_DefaultSize.y / 2, 0.0f), m_DefaultSize, L"GAMEOVER");
	}

	// 更新処理
	void GameOverSprite::OnUpdate()
	{
		Sprites::OnUpdate();
	}
}