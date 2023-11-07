/*!
@file Golem.cpp
@brief ゴーレム本体
@author 小宅碧
*/

#include "iostream"
#include "stdafx.h"
#include "Project.h"
#include "math.h"
#include <vector>

namespace basecross {
	//--------------------------------------------------------------------------------------
	// 敵基底データとクラス
	//--------------------------------------------------------------------------------------
	void Golem::OnCreate() {
		//初期位置などの設定
		m_transform = GetComponent<Transform>();
		m_transform->SetScale(Vec3(3.0f));
		m_transform->SetRotation(Vec3(0.0f));
		m_transform->SetPosition(m_startPosition);


		// PathSearchコンポーネントの取得
		auto MapPtr = m_cellMapPtr.lock();
		if (MapPtr)
		{
			AddComponent<PathSearch>(MapPtr);
		}
		//CollisionSphere衝突判定を付ける
		auto ptrColl = AddComponent<CollisionCapsule>();
		ptrColl->SetDrawActive(false);
		ptrColl->SetFixed(true);
		ptrColl->SetDrawActive(true);

		//描画コンポーネントの設定
		SetAlphaActive(true);
		SetDrawActive(true);

		// 新規ドローコンポーネントの設定
		auto ptrDraw = AddComponent<BcPNTBoneModelDraw>();
		ptrDraw->SetMeshResource(L"GOLEM");
		ptrDraw->SetMeshToTransformMatrix(m_differenceMatrix);
		ptrDraw->SetSpecularColor(Col4(1, 1, 1, 1));
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetLightingEnabled(false);

		// アニメーションの設定
		ptrDraw->AddAnimation(L"WAIT", 0, 1, true);
		ptrDraw->AddAnimation(L"BOOTING", 0, 90, false);
		ptrDraw->AddAnimation(L"WALKSTART", 100, 30, false);
		ptrDraw->AddAnimation(L"WALKING", 130, 60, true);
		ptrDraw->AddAnimation(L"WALKFINISH", 190, 15, false);
		ptrDraw->AddAnimation(L"ATTACKSTART_SWINGDOWN", 210, 60, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_SWINGDOWN", 270, 30, false);
		ptrDraw->AddAnimation(L"ATTACKSTART_PUNCH", 310, 60, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_PUNCH", 370, 60, false);
		ptrDraw->AddAnimation(L"ATTACKSTART_RAMMING", 440, 60, false);
		ptrDraw->AddAnimation(L"ATTACKING_RAMMING", 500, 30, true);
		ptrDraw->AddAnimation(L"ATTACKFINISH_RAMMING", 530, 60, false);
		ptrDraw->AddAnimation(L"DEATH", 0, 1, true);

		// 最初のアニメーションを指定
		ptrDraw->ChangeCurrentAnimation(L"BOOTING");



		// 影を追加
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"GOLEM");
		ptrShadow->SetMeshToTransformMatrix(m_differenceMatrix);
		// タグの設定
		AddTag(L"ENEMY");
		AddTag(L"BOSS");

	}

	void Golem::OnUpdate() {
		// アニメーションを更新
		AnimationUpdate();

		if (m_motion == Walking) {
			// デルタタイムを取得
			float deltaTime = App::GetApp()->GetElapsedTime();
			// 攻撃用のカウントを加算
			m_countTime += deltaTime;
		}

		if (m_motion == Walking || m_motion == WalkFinish || m_motion == Attacking_Ramming || m_motion == AttackFinish_Ramming) {
			MoveGolem();
		}


		if (m_status.life <= 0 && m_motion != Death) {
			// 体力が0以下の場合、モーションを変更
			m_motion = Death;
		}

		// アニメーションを更新
		AnimationUpdate();

		// アニメーションが終了しているかをチェック
		auto ptrDraw = GetComponent<BcPNTBoneModelDraw>();
		if (ptrDraw->IsTargetAnimeEnd()) {
			if (m_currentMotion == eMotion::Booting) {
				m_motion = WalkStart;
			}

			if (m_currentMotion == eMotion::WalkStart) {
				m_motion = Walking;
			}

			if (m_currentMotion == eMotion::Walking) {
				// カウントが攻撃間隔に達したか
				if (m_countTime > m_status.attackInterspace) {
					m_motion = WalkFinish;
					m_countTime = 0;
				}
			}

			if (m_currentMotion == eMotion::WalkFinish) {
				bool CanSwingDown = CheckAttackArea(eMotion::AttackStart_Swingdown);
				bool CanPunch = CheckAttackArea(eMotion::AttackStart_Punch);

				// 現在の状況に応じて攻撃モーションを決定する
				if (CanSwingDown && !CanPunch) {
					m_motion = AttackStart_Swingdown;
				}
				if (!CanSwingDown && CanPunch) {
					m_motion = AttackStart_Punch;
				}
				// どちらの攻撃も可能な場合はランダムに選択される
				if (CanSwingDown && CanPunch) {
					int random = rand() % 2;
					if (random == 0) {
						m_motion = AttackStart_Swingdown;
					}
					else {
						m_motion = AttackStart_Punch;
					}
				}
				else {
					// 上記の攻撃範囲内にもいない場合はプレイヤーに向かって突進攻撃をする
					m_motion = AttackStart_Ramming;
				}
			}

			if (m_currentMotion == eMotion::AttackStart_Swingdown) {
				// 攻撃範囲内にプレイヤーが居たらダメージ
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				m_motion = AttackFinish_Swingdown;
			}

			if (m_currentMotion == eMotion::AttackFinish_Swingdown) {
				m_motion = WalkStart;
			}

			if (m_currentMotion == eMotion::AttackStart_Punch) {
				// 攻撃範囲内にプレイヤーが居たらダメージ
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				m_motion = AttackFinish_Punch;
			}

			if (m_currentMotion == eMotion::AttackFinish_Punch) {
				m_motion = WalkStart;
			}

			if (m_currentMotion == eMotion::AttackStart_Ramming) {
				m_motion = Attacking_Ramming;
			}

			if (m_currentMotion == eMotion::Attacking_Ramming) {

				m_motion = AttackFinish_Ramming;
			}

			if (m_currentMotion == eMotion::AttackFinish_Ramming) {
				m_motion = WalkStart;
			}
			if (m_currentMotion == eMotion::Death) {
				// ゴーレムを消去する
				GetStage()->RemoveGameObject<Golem>(GetThis<Golem>());
			}


		}
	}
	void Golem::AnimationUpdate()
	{
		// アニメーションの再生
		auto ptrDraw = GetComponent<BcPNTBoneModelDraw>();

		// アニメーションのタイプが変わっていたら
		if (m_motion != m_currentMotion || ptrDraw->GetCurrentAnimation() != m_motionKey.at(m_motion))
		{
			// タイプに応じてアニメーションを変更する
			ptrDraw->ChangeCurrentAnimation(m_motionKey.at(m_motion));
			m_currentMotion = m_motion;
		}

		// 前フレームからのデルタタイムを取得
		float deltaTime = App::GetApp()->GetElapsedTime();

		// モーションやツールに応じて再生する
		if (m_motion == Wait)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == WalkStart)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == Walking)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == WalkFinish)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == AttackStart_Swingdown)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == AttackFinish_Swingdown)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == AttackStart_Punch)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == AttackFinish_Punch)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == AttackStart_Ramming)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == Attacking_Ramming)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == AttackFinish_Ramming)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == Death)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}

	}

	void Golem::MoveGolem() {
		// プレイヤーの座標を取得
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto playerPos = ptrPlayer->GetComponent<Transform>()->GetPosition();

		// 自身の座標を取得
		auto ptrTrans = GetComponent<Transform>();
		auto golemPos = ptrTrans->GetPosition();

		// デルタタイムを取得
		float deltaTime = App::GetApp()->GetElapsedTime();

		// 計算した加速度を移動量に加算
		m_velocity += m_force * deltaTime;

		// 移動量はm_status.speedを上限とする
		if (m_status.speed < m_velocity.length()) {
			m_velocity *= m_status.speed / m_velocity.length();
		}


		if (m_motion == Walking || m_motion == WalkFinish) {
			// 座標を更新
			golemPos += m_velocity * deltaTime * 0.1f;
			ptrTrans->SetPosition(golemPos);

			// 移動方向の向きを取得
			float rad = -atan2(golemPos.z - m_currentPos.z, golemPos.x - m_currentPos.x);
			// ズレの修正のために角度を90°ずらす
			Vec3 rotation(0.0f, rad - XM_PIDIV2, 0.0f);

			if (m_rotation.y < rotation.y) {
				m_rotation.y += 3.0f * (XM_PI / 180.0f);
				if (m_rotation.y > rotation.y) {
					m_rotation.y -= 3.0f * (XM_PI / 180.0f);
				}

				// 計算した向きを自身に適用
				m_transform->SetRotation(m_rotation);
			}
		}
		else
		{
			// プレイヤーを発見出来なかった場合は移動量を0にする
			m_velocity = Vec3(0.0f, 0.0f, 0.0f);
		}

		// 現在の座標を前回のフレームの座標としてm_currentPosに保存
		m_currentPos = m_transform->GetPosition();
		// 加速度をリセットする
		m_force = Vec3(0.0f, 0.0f, 0.0f);
	}

	bool Golem::CheckAttackArea(eMotion motion) {
		// 攻撃範囲
		float attackRange;

		// 攻撃に応じて攻撃範囲を代入
		if (motion == AttackStart_Swingdown) {
			attackRange = 4.0f;
		}
		if (motion == AttackStart_Punch) {
			attackRange = 2.0f;
		}

		// 自身の座標を取得
		auto ptrTrans = GetComponent<Transform>();
		auto golemPos = ptrTrans->GetPosition();
		auto golemForward = ptrTrans->GetForward();
		golemForward *= -1.0f / golemForward.length();

		// 攻撃範囲の中心座標を計算
		auto areaPos = golemPos + golemForward * attackRange;

		// プレイヤーの座標を取得
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto playerPos = ptrPlayer->GetComponent<Transform>()->GetPosition();

		// 攻撃範囲の中心座標からプレーヤーまでの距離を計算
		float  targetToLength = Vec3(areaPos.x - playerPos.x, 0.0f, areaPos.z - playerPos.z).length();

		// 攻撃範囲内に居た場合true
		if (targetToLength < attackRange) {
			return true;
		}
		// 居なければfalse
		return false;
	}

	void Golem::AddDamage(int Damage) {
		m_status.life -= Damage;
	}
}