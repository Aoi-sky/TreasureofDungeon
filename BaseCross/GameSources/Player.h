/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class Player :public GameObject {
		
		Vec2 GetInputState() const;//プレイヤーのコントローラとキーボードの入力
		Vec3 GetMoveVector() const;// コントローラから方向ベクトルを得る
		void MovePlayer();//プレイヤーの移動
		
		float m_Speed;

	public:
		Player(const shared_ptr<Stage>& StagePtr);
		virtual ~Player() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

	};

}
//end basecross

