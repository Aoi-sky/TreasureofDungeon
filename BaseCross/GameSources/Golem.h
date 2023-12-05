/*!
@file Golem.h
@brief ゴーレム本体
@author 小宅碧
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	セル検索を使った行動クラスのフラグ
	//--------------------------------------------------------------------------------------
	enum class CellSearchedFlg {
		Failed, // 失敗
		Seek, // 追跡
		Arrived // 到着
	};

	// ステータスを設定
	struct Status {
		int life; // 体力
		float speed; // 移動速度
		int attackInterspace; // 敵の攻撃間隔
	};

	// ステータスを設定
	// 体力, 移動速度, 攻撃頻度(フレーム)
	const Status statusList[1] = {
			{1000, 0.05f, 180}
	};

	// 攻撃の詳細設定
	struct Attack {
		float PunchRange; // パンチの攻撃範囲
		float SwingdownRange; // 振り下ろしの攻撃範囲
		float RammingRange; // 突進の射程
		float RammingSpeed; // 突進時の移動速度増加倍率
		float NormalTurningSpeed; // 通常時のゴーレムの旋回速度
		float RammingTurningSpeed; // 突進時のゴーレムの旋回速度
	};

	const Attack AttackList[1] = {
		{6.0f, 8.0f, 20.0f, 4.0f, 3.0f, 0.3f}
	};

	// 基底クラス
	class Golem : public GameObject
	{
	protected:

		// アニメーション定数
		enum eMotion
		{
			Wait, // 待機
			Booting, // 起動
			WalkStart, // 歩行開始
			Walking1, // 歩行中(ループ可)
			Walking2, // 歩行中(ループ可)
			WalkFinish, // 歩行終了
			AttackStart_Swingdown,// 攻撃開始(振り下ろし)
			AttackFinish_Swingdown, // 攻撃終了(振り下ろし)
			AttackStart_Punch,// 攻撃開始(パンチ)
			AttackFinish_Punch, // 攻撃終了(パンチ)
			AttackStart_Ramming,// 攻撃開始(突進)
			Attacking_Ramming1, // 攻撃中(突進)(ループ可)
			Attacking_Ramming2, // 攻撃中(突進)(ループ可)
			AttackFinish_Ramming1, // 攻撃終了(突進)(スタンモーションの為に2分割)
			AttackFinish_Ramming2, // 攻撃終了(突進)(スタンモーションの為に2分割)
			AttackFinish_Ramming3, // 攻撃終了(突進)(減速しながら停止)
			Stun_Ramming_Forward, // スタン(突進後に前から岩)
			Stun_Ramming_Behind, // スタン(突進後に後ろから岩)
			Stun_Normal_Forward, // スタン(前から岩)
			Stun_Normal_Behind, // スタン(後ろから岩)
			Stun1, // スタン中(ループ可)
			Stun2, // スタン中(ループ可)
			Stun_Recovery, // スタン復帰
			Death // 死亡
		};

		// 攻撃定数
		enum eAttack
		{
			SwingDown, // 振り下ろし攻撃
			Punch, // パンチ攻撃
			Ramming // 突進攻撃
		};

		// 方向定数
		enum eDirection {
			Forward,
			Behind
		};

		// セルマップのポインタ
		weak_ptr<StageCellMap> m_cellMapPtr;
		// ステータスを保存するメンバ変数
		Status m_status;
		// 攻撃の詳細設定を保存するメンバ変数
		Attack m_attackStatus;
		// 前回の攻撃から経過したフレーム数
		int m_countTime;
		// スポーン地点
		Vec3 m_startPos;
		// 回転
		Vec3 m_rotation;
		// 加速度
		Vec3 m_force;
		// 速度
		Vec3 m_velocity;
		// 移動方向
		Vec3 m_forward;
		// 前フレームの座標
		Vec3 m_currentPos;
		// 突進攻撃開始時の座標
		Vec3 m_rammingPos;
		// 振り下ろし攻撃をするかを判定するフラグ
		bool m_canSwingDown;
		// パンチ攻撃をするかを判定するフラグ
		bool m_canPunch;
		// 突進を中断するかを判定するフラグ(trueで中断)
		bool m_stopRammingFlg;
		// スタン攻撃を受けたかを判定するフラグ(trueでスタン攻撃を受けた)
		bool m_stunAttackFlg;
		// スタンが継続する時間(スタンから復帰で0にリセット)
		int m_stunDuration;
		// スタン開始からの合計時間(スタンから復帰で0にリセット)
		int m_stunTotalTime;
		// 自分自身のトランスフォームコンポーネント
		std::shared_ptr<Transform> m_transform;
		// 通過する経路
		vector<CellIndex> m_cellIVec;
		//現在の自分のセルインデックス
		int m_cellIndex;
		//目指す(次の)セルインデックス
		int m_nextCellIndex;
		//ターゲットのセルインデックス
		int m_targetCellIndex;
		// トランスフォームとモデルの差分行列
		Mat4x4 m_differenceMatrix;
		// アニメーションのキー
		vector<wstring> m_motionKey;
		// アニメーションのタイプ
		eMotion m_motion;
		// 前フレームのアニメーションのタイプ
		eMotion m_currentMotion;
		// プレイヤーのポインタ
		weak_ptr<GameObject> m_playerPtr;
		// 衝突してきた岩の角度(ゴーレムの正面方向を0とする)
		float m_rockAngle;
	public:
		// コンストラクタ
		Golem::Golem(const shared_ptr<Stage>& stagePtr, // ステージのポインタ
			const shared_ptr<StageCellMap>& cellMapptr, // セルマップのシェアドポインタ
			const Vec3 position // 初期座標
		) :
			GameObject(stagePtr), // ステージのポインタ
			m_cellMapPtr(cellMapptr), // セルマップのシェアドポインタ
			m_startPos(position) // 初期座標
		{
			m_status = statusList[0];
			m_attackStatus = AttackList[0];

			m_rotation = Vec3(0.0f);

			m_differenceMatrix.affineTransformation(
				Vec3(0.25f, 0.25f, 0.25f),
				Vec3(0.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(0.0f, -0.6f, 0.0f)
			);

			m_motionKey = {
				L"WAIT", // 起動
				L"BOOTING", // 起動
				L"WALKSTART", // 歩行開始
				L"WALKING1", // 歩行中(ループ可)
				L"WALKING2", // 歩行中(ループ可)
				L"WALKFINISH", // 歩行終了
				L"ATTACKSTART_SWINGDOWN", // 攻撃開始(振り下ろし)
				L"ATTACKFINISH_SWINGDOWN", // 攻撃終了(振り下ろし)
				L"ATTACKSTART_PUNCH", // 攻撃開始(パンチ)
				L"ATTACKFINISH_PUNCH", // 攻撃終了(パンチ)
				L"ATTACKSTART_RAMMING", // 攻撃開始(突進)
				L"ATTACKING_RAMMING1", // 攻撃中(突進)(ループ可)
				L"ATTACKING_RAMMING2", // 攻撃中(突進)(ループ可)
				L"ATTACKFINISH_RAMMING1", // 攻撃終了(突進)(スタンモーションの為に2分割)
				L"ATTACKFINISH_RAMMING2", // 攻撃終了(突進)(スタンモーションの為に2分割)
				L"ATTACKFINISH_RAMMING3", // 攻撃終了(突進)(減速しながら停止)
				L"STUN_RAMMING_FORWARD", // スタン(突進後に前から岩)
				L"STUN_RAMMING_BEHIND", // スタン(突進後に後ろから岩)
				L"STUN_NORMAL_FORWARD", // スタン(前から岩)
				L"STUN_NORMAL_BEHIND", // スタン(後ろから岩)
				L"STUN1", // スタン中(ループ可)
				L"STUN2", // スタン中(ループ可)
				L"STUN_RECOVERY", // スタン復帰
				L"DEATH" // 死亡
			};

			m_motion = Booting;
			m_currentMotion = Booting;

		}

		// デストラクタ
		virtual ~Golem() {};

		/*!
		@brief	 クラスが最初に作成される時に実行される関数
		*/
		void OnCreate();

		/*!
		@brief	 毎フレーム実行される関数
		*/
		void OnUpdate();

		/*!
		@brief アニメーション再生関数
		*/
		void AnimationUpdate();

		/*!
		@brief	 移動をする関数
		*/
		void MoveGolem();

		/*!
		@brief	 指定された攻撃の範囲内にプレイヤーが居るかを確認する関数
		@param[in]	 Attack 攻撃の種類
		@return 攻撃範囲内に居たらtrue
		*/
		bool CheckAttackArea(eAttack Attack);

		/*!
		@brief	 ゴーレムにダメージを与える関数
		*/
		void AddDamage(int damage);

		/*!
		@brief	 ゴーレムがスタン攻撃を受けたフラグを立て、スタンの合計時間をリセットする関数
		@param[in]	 StunTime スタンするフレーム数
		*/
		void AddStun(int StunTime);

		/*!
		@brief	 ゴーレムの正面方向を基準として、ターゲットの方向を計算する関数
		@param[in]	 target ターゲットが所持しているTransformコンポーネントのポインタ
		@return ゴーレムの正面方向を基準とした角度を返す
		*/
		float AngleCalculation(shared_ptr<Transform>& target);

		/*!
		@brief	 他のコリジョンと衝突した時に実行される関数
		@param[in]	Other	衝突したオブジェクトのポインタ
		*/
		void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

		/*!
		@brief	 他のコリジョンとの衝突が解消された時に実行される関数
		@param[in]	Other	衝突していたオブジェクトのポインタ
		*/
		void OnCollisionExit(shared_ptr<GameObject>& Other) override;
	};
}
