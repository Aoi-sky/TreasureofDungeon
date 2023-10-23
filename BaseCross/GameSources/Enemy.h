#pragma once
#include "stdafx.h"

namespace basecross {
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
	public:
		Enemy(const shared_ptr<Stage>& StagePtr, const shared_ptr<StageCellMap>& CellMap, const Vec3& Position);
		Enemy();
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate() override;
		//スタート地点
		const Vec3& GetStartPosition()const {
			return m_StartPosition;
		}
		//目的の場所をサーチしSeek行動をする
		bool Search(const Vec3& TargetPos);
		//目的の場所にarrive行動をとる
		void ArriveBehavior(const Vec3& TargetPos);
	};
}
