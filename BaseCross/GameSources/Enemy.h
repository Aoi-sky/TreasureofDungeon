#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
///	セル検索を使った行動クラスのフラグ
//--------------------------------------------------------------------------------------
	enum class CellSearchFlg {
		Failed,
		Seek,
		Arrived
	};
	//--------------------------------------------------------------------------------------
	//　敵(Enemy)
	//--------------------------------------------------------------------------------------
	class Enemy : public GameObject {
		Vec3 m_StartPosition;
		Vec3 m_Force;
		Vec3 m_Velocity;
		weak_ptr<StageCellMap> m_CelMap;
		vector<CellIndex> m_CellPath;
		//現在の自分のセルインデックス
		int m_CellIndex;
		//めざす（次の）のセルインデックス
		int m_NextCellIndex;
		//ターゲットのセルインデックス
		int m_TargetCellIndex;
		//敵のスピード
		float m_Speed;
	public:
		Enemy(const shared_ptr<Stage>& StagePtr, const shared_ptr<StageCellMap>& CellMap, const Vec3& Position);
		virtual ~Enemy();
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate() override;
		//スタート地点
		const Vec3& GetStartPosition()const {
			return m_StartPosition;
		}
		//目的の場所をサーチしSeek行動をする
		bool Search(const Vec3& TargetPos);
		//目的の場所をサーチしSeek行動をする
		CellSearchFlg SeekBehavior(const Vec3& TargetPos);
		//目的の場所にarrive行動をとる
		void ArriveBehavior(const Vec3& TargetPos);
	};
}
