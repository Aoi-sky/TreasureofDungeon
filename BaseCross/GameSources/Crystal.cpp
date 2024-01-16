/*!
@file Crystal.cpp
@brief 水晶本体
@author 小宅碧
*/
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	// 水晶クラス
	//--------------------------------------------------------------------------------------
	void Crystal::OnCreate() {
		//初期位置などの設定
		m_transform = GetComponent<Transform>();
		m_transform->SetScale(Vec3(2.0f));
		m_transform->SetRotation(Vec3(0.0f));
		m_transform->SetPosition(m_startPos);

		//CollisionCapsule衝突判定を付ける
		auto ptrColl = AddComponent<CollisionCapsule>();
		ptrColl->SetDrawActive(true);
		ptrColl->SetFixed(false);

		//描画コンポーネントの設定
		SetAlphaActive(true);
		SetDrawActive(true);

		// 新規ドローコンポーネントの設定
		auto ptrDraw = AddComponent<BcPNStaticDraw>();
		ptrDraw->SetMeshResource(L"CRYSTAL_R");
		ptrDraw->SetMeshToTransformMatrix(m_differenceMatrix);
		ptrDraw->SetSpecularColor(Col4(1, 1, 1, 1));
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetLightingEnabled(false);

		// 影を追加
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"CRYSTAL_R");
		ptrShadow->SetMeshToTransformMatrix(m_differenceMatrix);

		// タグの設定
		AddTag(L"Crystal");

		m_currentState = Wait;
		m_pastState = Wait;
	}

	void Crystal::OnUpdate() {
		CheckState();



		// カウントを加算
		m_elapsedTime++;
		// 現在の状態を前フレームの状態として保存
		m_pastState = m_currentState;
	}

	void Crystal::CheckState() {
		// 経過時間が待機時間を経過した場合の処理
		if (m_standbyTime <= m_elapsedTime && m_pastState == Wait) {
			m_currentState = Move;
		}



	}
}