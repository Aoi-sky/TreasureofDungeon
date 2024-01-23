/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		shared_ptr<SoundItem> m_bgm; // サウンドアイテム

		//ビューの作成
		void CreateViewLight();
		//壁の作成
		void CreateWall();
		//床の作成
		void CreateFloor();
		//柱の作成
		void CreatePiller();
		//プレイヤーの作成
		void CreatePlayer();
		//敵の作成
		void CreateEnemy();
		//セルマップの生成
		void CreateStageCellMap();
		// セルマップのコストを設定
		void SetCellMapCost();
		//落石ギミック
		void CreateFallingRocks();
		// ゴーレムの生成
		void CreateGolem();

		//BGMの生成
		void PlayBGM();
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		
		virtual void OnCreate()override;
		virtual void OnDestroy()override;
		virtual void OnDraw() override;
	};


}
//end basecross

