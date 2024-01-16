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
		// Attack状態になるまでの待機時間
		int m_standbyTime;
		// Move・Attack状態を合計した持続時間
		int m_duration;
		// 水晶が生成されてからの経過時間
		int m_elapsedTime;
		// 水晶が突き出てくる速度
		float m_speed;
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
			const int standbyTime, // Attack状態までの待機時間
			const int duration // Attack状態の持続時間
		) :
			GameObject(stagePtr), // ステージのポインタ
			m_startPos(position), // 初期座標
			m_standbyTime(standbyTime), // Attack状態までの待機時間
			m_duration(duration) // Attack状態の持続時間
		{
			m_differenceMatrix.affineTransformation(
				Vec3(0.4f, 0.4f, 0.4f),
				Vec3(0.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f)
			);

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
		@brief	 水晶の状態を管理する関数
		*/
		void CheckState();
	};
}