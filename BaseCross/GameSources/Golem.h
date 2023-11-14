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
		float searchDistance; // 敵の感知範囲
	};

	// ステータスを設定
	// 体力, 移動速度, 攻撃頻度(フレーム)
	const Status statusList[1] = {
			{1000, 0.05f, 180}
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
			AttackFinish_Ramming, // 攻撃終了(突進)
			Death, // 死亡
		};

		// セルマップのポインタ
		weak_ptr<StageCellMap> m_cellMapPtr;
		// ステータスを保存するメンバ変数
		Status m_status;
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
		// 突進を中断するかを判定するフラグ
		bool m_stopRammingFlg;
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
			// 難易度が増えた場合はここでステータスリストの参照先を変更
			m_status = statusList[0];

			m_rotation = Vec3(0.0f);

			m_differenceMatrix.affineTransformation(
				Vec3(0.2f, 0.2f, 0.2f),
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
				L"ATTACKFINISH_RAMMING", // 攻撃終了(突進)
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
		@param[in]	 motion
		@return 攻撃範囲内に居たらtrue
		*/
		bool CheckAttackArea(eMotion motion);

		/*!
		@brief	 ゴーレムにダメージを与える関数
		*/
		void AddDamage(int damage);

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
