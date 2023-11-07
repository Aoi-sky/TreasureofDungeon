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
		//ビューの作成
		void CreateViewLight();
		//フィールドの作成
		void CreateField();
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
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
	};


}
//end basecross

