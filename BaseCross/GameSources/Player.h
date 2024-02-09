/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	class Player :public GameObject {
	public:
		//モーション管理用enum
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
			Dash1,//ダッシュ開始
			Dash2,//ダッシュ中
			Dash3,//ダッシュ後硬直
			Dash4,//ダッシュ後→待機
		};

		// ステータス管理用enum
		enum ePlayeStatus
		{
			MaxLife, // 最大体力
			MaxStamina, // 最大スタミナ
			StaminaConsumption, // 1回のダッシュで消費するスタミナ
			StaminaRecoverySpeed, // スタミナ回復速度(1秒辺りの回復量)
			InvincibleTime, // 無敵時間
			OffensiveAbility, // 基礎攻撃力
			WalkSpeed, // 歩行時の移動速度
			DashSpeed, // ダッシュ時の移動速度
			StepSpeed, // ダッシュ時の短距離移動の速度
			AttackSpeed, // 攻撃速度(1で通常速度)
			AttackBuff, // 攻撃バフの効果量(効果量1で基礎の100%分強化)
			StaminaBuff, // スタミナバフの効果量(効果量1で基礎の100%分強化)
			AttackBuffLimit, // 攻撃バフの上限回数
			StaminaBuffLimit, // スタミナバフの上限回数
			size,
		};
	protected:
		// モーションキー
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
			L"Dash1",
			L"Dash2",
			L"Dash3",
			L"Dash4",
		};
		// 攻撃に使うキーを保持
		const WORD BUTTON_ATTACK = XINPUT_GAMEPAD_B;
		// ダッシュに使うキーを保持
		const WORD BUTTON_DASH = XINPUT_GAMEPAD_A;
		// 自身のトランスフォームコンポーネントを保持
		shared_ptr<Transform> m_ptrTrans;
		// 現在のフレームのモーションを保持
		eMotion m_currentMotion;
		// 前フレームのモーションを保持
		eMotion m_pastMotion;
		// トランスフォームとモデルの差分行列
		Mat4x4 m_differenceMatrix;
		// ステータスを保持する配列
		vector<float> m_status;
		// 現在のフレームの向きを保持
		Vec3 m_currentAngle;
		// 現在HP
		float m_life;
		// 現在スタミナ
		float m_stamina;
		// 攻撃バフを拾った回数
		int AttackBuffCount;
		// スタミナバフを拾った回数
		int StaminaBuffCount;
		// 無敵時間
		int m_invincibleTime;
		// 経過時間
		int m_totalTime;
		// ゴーレムと接触しているかの判定を保持
		bool m_hitFlg;
		
		// エフェクト関連
		shared_ptr<EfkEffect> m_damageEffect;
		shared_ptr<EfkPlay> m_damageEfkPlay;
		wstring m_damageEffectStr;

	public:
		// コンストラクタ
		Player::Player(const shared_ptr<Stage>& StagePtr) :
			GameObject(StagePtr)
		{
			// トランスフォームとモデルの差分行列を代入
			m_differenceMatrix.affineTransformation(
				Vec3(0.5f, 0.5f, 0.5f),
				Vec3(0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -2.0f, 0.0f)
			);

			// csvからプレイヤーのステータスを代入
			const auto& status = CSVLoader::LoadFile("PlayerStatus");
			m_status.resize(ePlayeStatus::size);
			m_status.at(MaxLife) = stof(status.at(MaxLife).at(1));
			m_status.at(MaxStamina) = stof(status.at(MaxStamina).at(1));
			m_status.at(StaminaConsumption) = stof(status.at(StaminaConsumption).at(1));
			m_status.at(StaminaRecoverySpeed) = stof(status.at(StaminaRecoverySpeed).at(1));
			m_status.at(InvincibleTime) = stof(status.at(InvincibleTime).at(1));
			m_status.at(OffensiveAbility) = stof(status.at(OffensiveAbility).at(1));
			m_status.at(WalkSpeed) = stof(status.at(WalkSpeed).at(1));
			m_status.at(DashSpeed) = stof(status.at(DashSpeed).at(1));
			m_status.at(StepSpeed) = stof(status.at(StepSpeed).at(1));
			m_status.at(AttackSpeed) = stof(status.at(AttackSpeed).at(1));
			m_status.at(AttackBuff) = stof(status.at(AttackBuff).at(1));
			m_status.at(StaminaBuff) = stof(status.at(StaminaBuff).at(1));
			m_status.at(AttackBuffLimit) = stof(status.at(AttackBuffLimit).at(1));
			m_status.at(StaminaBuffLimit) = stof(status.at(StaminaBuffLimit).at(1));

			// 各変数の初期化
			m_life = m_status.at(MaxLife);
			m_stamina = m_status.at(MaxStamina);
			m_currentMotion = Wait;
			m_pastMotion = Wait;
			m_hitFlg = false;
			m_invincibleTime = 90;
			m_totalTime = 0;
			AttackBuffCount = 0;
			StaminaBuffCount = 0;
		}

		/*!
		@brief	 クラスが最初に作成される時に実行される関数
		*/
		virtual void OnCreate() override;

		/*!
		@brief	 毎フレーム実行される関数
		*/
		virtual void OnUpdate() override;

		/*!
		@brief	 左スティックの入力を得る関数
		@return 左スティックの入力方向
		*/
		Vec2 GetInputState() const;

		/*!
		@brief 移動方向のベクトルを得る関数
		@return 移動方向のベクトル
		*/
		Vec3 GetMoveVector() const;

		/*!
		@brief 回転方向(y軸)を得る関数
		@return 回転方向(y軸)
		*/
		float  GetMoveAngle();

		/*!
		@brief 移動処理
		*/
		void OnMove();

		/*!
		@brief 死亡をチェックする関数
		*/
		void PlayerDeadCheck();

		/*!
		@brief スタミナの回復処理
		*/
		void StaminaRecovery();

		/*!
		@brief ゴーレムとの接触に関する処理
		*/
		void HitCheck();

		/*!
		@brief ゲームパッドからの入力を確認する関数
		*/
		void GamepadInputCheck();

		/*!
		@brief アニメーションの更新処理
		*/
		void AnimationUpdate();

		/*!
		@brief 攻撃処理
		*/
		void OnAttack();

		/*!
		@brief ダメージを与える関数
		@param[in]	damage 与えるダメージ
		@param[in]	Motion 被弾時のモーション
		*/
		void AddDamage(int damage, eMotion Motion);

		/*!
		@brief	 他のコリジョンと衝突した時に実行される関数
		@param[in]	Other 衝突したオブジェクトのポインタ
		*/
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

		/*!
		@brief	 他のコリジョンとの衝突が解消された時に実行される関数
		@param[in]	Other 衝突したオブジェクトのポインタ
		*/
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;

		/*!
		@brief 最大体力を取得する関数
		@return 最大体力
		*/
		float GetMaxLife()
		{
			return m_status.at(MaxLife);
		}

		/*!
		@brief 現在体力を取得する関数
		@return 現在体力
		*/
		float GetCurrentLife()
		{
			return m_life;
		}

		/*!
		@brief 攻撃力を取得する関数
		@return 攻撃力
		*/
		float GetOffensiveAbility() const
		{
			return m_status.at(OffensiveAbility);
		}

		/*!
		@brief 最大スタミナを取得する関数
		@return 最大スタミナ
		*/
		float GetMaxStamina() const
		{
			return m_status.at(MaxStamina);
		}

		/*!
		@brief 現在スタミナを取得する関数
		@return 現在スタミナ
		*/
		float GetCurrentStamina()
		{
			return  m_stamina;
		}

	};

}
//end basecross

