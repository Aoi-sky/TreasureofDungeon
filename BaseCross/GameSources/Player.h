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
		int m_DefaultLife = 100;//プレイヤーのデフォルトHP
		int m_Life;//プレイヤーのHP

		//アニメーション
	protected:
		enum eMotion
		{
			Wait,//待機モーション
			Walk, // 歩行開始
			Attack,// 攻撃(振り下ろす攻撃)
			Attack2,//攻撃(薙ぎ払い攻撃)
			Damage,//軽い
			Damage2//重い
		};
		// トランスフォームとモデルの差分行列
		Mat4x4 m_differenceMatrix;


	public:
		Player(const shared_ptr<Stage>& StagePtr);
		virtual ~Player() {};

		void AddPlayerDamage(int damage);
		void PlayerDead();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;

		int GetMaxLife()
		{
			return m_DefaultLife;
		}
		int GetCurrentLife()
		{
			return m_Life;
		}
	};

}
//end basecross

