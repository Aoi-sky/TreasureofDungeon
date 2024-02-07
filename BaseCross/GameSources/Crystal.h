/*!
@file Crystal.h
@brief 水晶本体
@author 小宅碧
*/
#pragma once
#include "stdafx.h"

namespace basecross {
	// 水晶クラス
	class Crystal : public GameObject
	{
	protected:
		//// アニメーション定数
		enum eState
		{
			Wait, // 地中で待機
			Move, // 地上へ移動
			Attack, // 地上で静止
			Finish // 地中に移動、その後消滅
		};

		// 自分自身のトランスフォームコンポーネント
		std::shared_ptr<Transform> m_transform;
		// 初期座標
		Vec3 m_startPos;
		// Wait状態を維持する時間(秒)
		int m_standbyTime;
		// Move状態を終えるまでの所要時間(秒)
		int m_equiredTime;
		// Attack状態の持続時間(秒)
		int m_duration;
		// 生成されてからの経過時間(秒)
		float m_progressTime;
		// 命中後に水晶が戻っていく速度
		float m_speed;
		// ElapsedTimeを保持する変数
		float m_elapsedTime;
		// 水晶がプレイヤーに命中したかを保存する変数
		bool m_isHit;
		// 現在のフレームの状態を保存する変数
		eState m_currentState;	
		// 前フレームの状態を保存する変数
		eState m_pastState;
		// トランスフォームとモデルの差分行列
		Mat4x4 m_differenceMatrix;


	public:
		// コンストラクタ
		Crystal::Crystal(const shared_ptr<Stage>& stagePtr, // ステージのポインタ
			const Vec3 position, // 初期座標
			const int standbyTime, // Wait状態を維持する時間(秒)
			const int equiredTime, // Move状態を完了するまでの所要時間(秒)
			const int duration // Attack状態の持続時間(秒)
		) :
			GameObject(stagePtr), // ステージのポインタ
			m_startPos(position), // 初期座標
			m_standbyTime(standbyTime), // Wait状態を維持する時間(秒)
			m_equiredTime(equiredTime), // Move状態を完了するまでの所要時間(秒)
			m_duration(duration) // Attack状態の持続時間(秒)
		{
			m_differenceMatrix.affineTransformation(
				Vec3(0.4f, 0.4f, 0.4f),
				Vec3(0.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f)
			);

			m_progressTime = 0.0f;
			m_speed = 0.0f;
			m_isHit = false;
			m_currentState = Wait;
			m_pastState = Wait;
		}

		// デストラクタ
		virtual ~Crystal() {};

		/*!
		@brief	 クラスが最初に作成される時に実行される関数
		*/
		void OnCreate();

		/*!
		@brief	 毎フレーム実行される関数
		*/
		void OnUpdate();

		/*!
		@brief	 移動処理を行う関数
		*/
		void OnMove();

		/*!
		@brief	 命中後の処理を行う関数
		*/
		void OnHit();

		/*!
		@brief	 Stateを管理する関数
		*/
		void StateManagement();
	};
}