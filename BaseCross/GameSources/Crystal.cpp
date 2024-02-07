/*!
@file Crystal.cpp
@brief 水晶本体
@author 小宅碧
*/
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void Crystal::OnCreate() {
		//初期位置などの設定
		m_transform = GetComponent<Transform>();
		m_transform->SetScale(Vec3(2.5f));
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
		ptrDraw->SetMeshResource(L"CRYSTAL");
		ptrDraw->SetMeshToTransformMatrix(m_differenceMatrix);
		ptrDraw->SetSpecularColor(Col4(1, 1, 1, 1));
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetLightingEnabled(false);

		// 影を追加
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"CRYSTAL");
		ptrShadow->SetMeshToTransformMatrix(m_differenceMatrix);

		// タグの設定
		AddTag(L"Crystal");

		// スピードの計算


		// 攻撃範囲の描写


	}

	void Crystal::OnUpdate() {
		// ElapsedTimeを取得
		m_elapsedTime = App::GetApp()->GetElapsedTime();

		// 現在の状態に応じた処理をする
		StateManagement();



		// 経過時間を加算
		m_progressTime += m_elapsedTime;

		// 現在の状態を前フレームの状態として保存
		m_pastState = m_currentState;
	}

	void Crystal::OnMove() {
		// 座標を取得
		auto pos = m_transform->GetPosition();
		// ElapsedTimeを取得
		float elapsedTime = App::GetApp()->GetElapsedTime();



	}

	void Crystal::StateManagement() {
		switch (m_pastState)
		{
		case Wait:
			// 経過時間が待機時間を経過した場合の処理
			if (m_standbyTime > m_progressTime) {
				m_currentState = Move;
			}
			break;
		case Move:
			// 移動処理
			OnMove();

			// 経過時間が待機時間を経過した場合の処理
			if (m_equiredTime > m_progressTime - m_standbyTime) {
				m_currentState = Move;
			}
			// 命中した場合、水晶の状態をFinishにする
			if (m_isHit) {
				m_currentState = Finish;
			}
			break;
		case Attack:
			// 経過時間がAttackの持続時間を経過した場合の処理
			if (m_duration <= m_progressTime - (m_standbyTime + m_equiredTime)) {
				m_currentState = Finish;
			}
			// 命中した場合、水晶の状態をFinishにする
			if (m_isHit) {
				m_currentState = Finish;
			}

			break;
		case Finish:

			break;
		default:
			m_currentState = Wait;
			break;
		}




	}
}