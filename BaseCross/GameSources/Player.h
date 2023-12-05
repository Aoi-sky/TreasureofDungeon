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
		const WORD BUTTON_SHOT = XINPUT_GAMEPAD_X;
		float m_Speed;
		int m_life;//プレイヤーのHP

	public:
		Player(const shared_ptr<Stage>& StagePtr);
		virtual ~Player() {};

		void AddPlayerDamage(int damage);
		void PlayerDead();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
	};

}
//end basecross

