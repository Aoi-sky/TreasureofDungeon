/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class Player :public GameObject {
		//アニメーション
	public:
		enum eMotion
		{
			Wait,//待機モーション
			WalkStart, // 歩行開始
			Walking1, // 歩行中(左→右)
			Walking2, // 歩行中(右→左)
			WalkEnd1, // 歩行終了(左→終了)
			WalkEnd2, // 歩行終了(右→終了)
			AttackStart,// 攻撃(振り下ろし攻撃)
			AttackEnd,// 攻撃(振り下ろし攻撃)
			Damage1,//被弾(軽度)
			Damage2,//被弾(重度)
			Dead,//死亡
		};

	protected:
		vector<wstring> m_motionKey = {
			L"Wait",
			L"WalkStart",
			L"Walking1",
			L"Walking2",
			L"WalkEnd1",
			L"WalkEnd2",
			L"AttackStart",
			L"AttackEnd",
			L"Damage1",
			L"Damage2",
			L"Dead",
		};

		// トランスフォームとモデルの差分行列
		Mat4x4 m_differenceMatrix;

		Vec2 GetInputState() const;//プレイヤーのコントローラとキーボードの入力
		Vec3 GetMoveVector() const;// コントローラから方向ベクトルを得る
		void MovePlayer();//プレイヤーの移動
		const WORD BUTTON_SHOT = XINPUT_GAMEPAD_X;
		float m_Speed;
		int m_DefaultLife = 100;//プレイヤーのデフォルトHP
		int m_Life;//プレイヤーのHP
		// モーションキーの初期設定
		eMotion m_currentMotion = Wait;
		eMotion m_pastMotion = Wait;
		// 敵と接触しているかを判定するフラグ
		bool HitFlg = false;
		// 無敵時間
		int invincibleTime = 90;
		// 経過時間
		int totalTime = 0;

	public:
		Player(const shared_ptr<Stage>& StagePtr);
		virtual ~Player() {};

		void OnAttack();
		void HitCheck();
		void AddPlayerDamage(int damage, eMotion Motion);
		void PlayerDead();
		void AnimationUpdate();

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

