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
		m_transform->SetScale(Vec3(3.5f));
		m_transform->SetRotation(Vec3(0.0f));
		m_transform->SetPosition(m_startPos);


		// PathSearchコンポーネントの取得
		auto MapPtr = m_cellMapPtr.lock();
		if (MapPtr)
		{
			AddComponent<PathSearch>(MapPtr);
		}
		//CollisionSphere衝突判定を付ける
		auto ptrColl = AddComponent<CollisionCapsule>();
		ptrColl->SetDrawActive(false);
		ptrColl->SetFixed(false);

		//描画コンポーネントの設定
		SetAlphaActive(true);
		SetDrawActive(true);

		// 新規ドローコンポーネントの設定
		auto ptrDraw = AddComponent<BcPNTBoneModelDraw>();
		ptrDraw->SetMultiMeshResource(L"GOLEM");
		ptrDraw->SetMeshToTransformMatrix(m_differenceMatrix);
		ptrDraw->SetSpecularColor(Col4(1, 1, 1, 1));
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetLightingEnabled(false);

		// アニメーションの設定
		ptrDraw->AddAnimation(L"WAIT", 0, 1, true);
		ptrDraw->AddAnimation(L"BOOTING", 0, 90, false);
		ptrDraw->AddAnimation(L"WALKSTART", 100, 30, false);
		ptrDraw->AddAnimation(L"WALKING1", 130, 60, false);
		ptrDraw->AddAnimation(L"WALKING2", 130, 60, false);
		ptrDraw->AddAnimation(L"WALKFINISH", 190, 15, false);
		ptrDraw->AddAnimation(L"ATTACKSTART_SWINGDOWN", 210, 60, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_SWINGDOWN", 270, 30, false);
		ptrDraw->AddAnimation(L"ATTACKSTART_PUNCH", 310, 60, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_PUNCH", 370, 60, false);
		ptrDraw->AddAnimation(L"ATTACKSTART_RAMMING", 440, 60, false);
		ptrDraw->AddAnimation(L"ATTACKING_RAMMING1", 500, 30, false);
		ptrDraw->AddAnimation(L"ATTACKING_RAMMING2", 500, 30, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_RAMMING", 530, 60, false);
		ptrDraw->AddAnimation(L"DEATH", 600, 1, true);

		// 最初のアニメーションを指定
		ptrDraw->ChangeCurrentAnimation(L"BOOTING");



		// 影を追加
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMultiMeshResource(L"GOLEM");
		ptrShadow->SetMeshToTransformMatrix(m_differenceMatrix);
		// タグの設定
		AddTag(L"Enemy");
		AddTag(L"Golem");

	}

	void Golem::OnUpdate() {
		switch (m_motion) {
		case Walking1:
		case Walking2:
		case Attacking_Ramming1:
		case Attacking_Ramming2:
			// 攻撃用のカウントを加算
			m_countTime++;
			MoveGolem();
			break;

		default:
			break;
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
				m_motion = Walking1;
			}

			if (m_currentMotion == eMotion::Walking1) {
				// カウントが攻撃間隔に達したか
				if (m_countTime > m_status.attackInterspace) {
					m_motion = WalkFinish;
					m_countTime = 0;
				}
				else {
					m_motion = Walking2;
				}
			}

			if (m_currentMotion == eMotion::Walking2) {
				// カウントが攻撃間隔に達したか
				if (m_countTime > m_status.attackInterspace) {
					m_motion = WalkFinish;
					m_countTime = 0;
				}
				else {
					m_motion = Walking1;
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
				if (m_countTime > m_status.attackInterspace || m_stopRammingFlg) {
					m_motion = AttackFinish_Ramming;
					m_stopRammingFlg = false;
				}
				else {
					m_motion = Attacking_Ramming1;
					m_rammingPos = m_transform->GetPosition();
				}
			}

			if (m_currentMotion == eMotion::Attacking_Ramming1) {
				// カウントが攻撃間隔に達する、または突進攻撃を中止するフラグがtrueなら
				if (m_countTime > m_status.attackInterspace || m_stopRammingFlg) {
					m_motion = AttackFinish_Ramming;
					GetStage()->AddGameObject<FallingRocks>();
					m_countTime = 0;
				}
				else {
					m_motion = Attacking_Ramming2;
				}
			}

			if (m_currentMotion == eMotion::Attacking_Ramming2) {
				// カウントが攻撃間隔に達する、または突進攻撃を中止するフラグがtrueなら
				if (m_countTime > m_status.attackInterspace || m_stopRammingFlg) {
					m_motion = AttackFinish_Ramming;
					m_countTime = 0;
				}
				else {
					m_motion = Attacking_Ramming1;
				}
			}

			if (m_currentMotion == eMotion::AttackFinish_Ramming) {
				m_motion = WalkStart;
				m_stopRammingFlg = false;
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
		if (m_motion == Booting)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
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
		if (m_motion == Walking1)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == Walking2)
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
		if (m_motion == Attacking_Ramming1)
		{
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			return;
		}
		if (m_motion == Attacking_Ramming2)
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
		// プレイヤーのポインタを取得
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		// プレイヤーの座標を取得
		auto playerPos = ptrPlayer->GetComponent<Transform>()->GetPosition();

		// 自身の座標を取得
		auto golemPos = m_transform->GetPosition();

		// デルタタイムを取得
		float deltaTime = App::GetApp()->GetElapsedTime();

		if (m_motion == Walking1 || m_motion == Walking2) {
			// 敵の向きををプレイヤーの方向に向ける
			float rad = -atan2(golemPos.z - playerPos.z, golemPos.x - playerPos.x);

			// 角度を90°ずらす
			Vec3 Rotation(0.0f, rad + XM_PIDIV2, 0.0f);
			// 現在の向きをプレイヤーの方向へに近づける
			if (m_rotation.y < Rotation.y) {
				m_rotation.y += 1.5f * (XM_PI / 180.0f);
			}
			if (m_rotation.y > Rotation.y) {
				m_rotation.y -= 1.5f * (XM_PI / 180.0f);
			}

			m_transform->SetRotation(m_rotation);
			m_forward = m_transform->GetForward();
		}

		if (m_motion == Attacking_Ramming1 || m_motion == Attacking_Ramming2) {
			// 敵の向きををプレイヤーの方向に向ける
			float rad = -atan2(golemPos.z - playerPos.z, golemPos.x - playerPos.x);

			// 角度を90°ずらす
			Vec3 Rotation(0.0f, rad + XM_PIDIV2, 0.0f);
			// 現在の向きをプレイヤーの方向へに近づける
			if (m_rotation.y < Rotation.y) {
				m_rotation.y += 0.3f * (XM_PI / 180.0f);
			}
			if (m_rotation.y > Rotation.y) {
				m_rotation.y -= 0.3f * (XM_PI / 180.0f);
			}

			m_transform->SetRotation(m_rotation);
			m_forward = m_transform->GetForward();
		}

		// 計算した加速度をm_velocityに加算
		m_velocity = m_forward * deltaTime;

		// m_velocityの総量をm_status.speedと同じになるよう調整する
		m_velocity *= m_status.speed / m_velocity.length();

		// 突進中は移動量を増加させる
		if (m_motion == Attacking_Ramming1 || m_motion == Attacking_Ramming2) {
			m_velocity *= 4.0f;
		}

		
		// 座標を更新
		//golemPos += m_velocity * deltaTime * 0.1f;
		golemPos -= m_velocity;
		m_transform->SetPosition(golemPos);


		// 現在の座標を前回のフレームの座標としてm_currentPosに保存
		m_currentPos = m_transform->GetPosition();

		// 突進を中止するかをチェック
		if (m_motion == Attacking_Ramming1 || m_motion == Attacking_Ramming2) {
			// 突進攻撃がプレイヤーを通り過ぎているかをチェック
			float golemMomentum = sqrt((golemPos.x - m_rammingPos.x) * (golemPos.x - m_rammingPos.x) + (golemPos.z - m_rammingPos.z) * (golemPos.z - m_rammingPos.z));
			float playerMomentum = sqrt((playerPos.x - m_rammingPos.x) * (playerPos.x - m_rammingPos.x) + (playerPos.z - m_rammingPos.z) * (playerPos.z - m_rammingPos.z));

			// 通り過ぎていた場合は突進を中止する
			if (golemMomentum > playerMomentum) {
				m_stopRammingFlg = true;
			}

			// 突進攻撃中にプレイヤーの近くにいるかをチェック
			float DistancetoPlayer = sqrt((golemPos.x - playerPos.x) * (golemPos.x - playerPos.x) + (golemPos.z - playerPos.z) * (golemPos.z - playerPos.z));

			// プレイヤーが近くにいた場合は突進を中止する
			if (DistancetoPlayer < 3.0f) {
				m_stopRammingFlg = true;
			}
		}
	}

	bool Golem::CheckAttackArea(eMotion motion) {
		// 攻撃範囲
		float attackRange;

		// 攻撃に応じて攻撃範囲を代入
		if (motion == AttackStart_Swingdown) {
			attackRange = 8.0f;
		}
		if (motion == AttackStart_Punch) {
			attackRange = 6.0f;
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

		// 攻撃範囲の中心座標からプレイヤーまでの距離を計算
		float  targetToLength = Vec3(areaPos.x - playerPos.x, 0.0f, areaPos.z - playerPos.z).length();

		// 攻撃範囲内に居た場合true
		if (targetToLength < attackRange) {
			return true;
		}
		// 居なければfalse
		return false;
		if (playerPos.x != 0) {
			m_motion = Death;
		}
	}

	void Golem::AddDamage(int Damage) {
		m_status.life -= Damage;
	}

	void Golem::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		// プレイヤーとの衝突
		if (Other->FindTag(L"Player"))
		{
			auto ptrColl =GetComponent<CollisionCapsule>();
			ptrColl->SetFixed(true);
			return;
		}
		if (m_motion == Attacking_Ramming1 || m_motion == Attacking_Ramming2) {
			// 壁との衝突
			if (Other->FindTag(L"Wall"))
			{
				m_stopRammingFlg = true;
				return;
			}

			// 柱との衝突
			if (Other->FindTag(L"FixedCylinder"))
			{
				m_stopRammingFlg = true;
				return;
			}

			// 落石との衝突
			if (Other->FindTag(L"FallingRocks"))
			{
				m_stopRammingFlg = true;
				return;
			}
		}
		if (Other->FindTag(L"MoveFallingRocks"))
		{
			GetStage()->RemoveGameObject<Golem>(GetThis<Golem>());
			PostEvent(1.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToClearStage");
			return;
		}
	}

	void Golem::OnCollisionExit(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Player"))
		{
			auto ptrColl = GetComponent<CollisionCapsule>();
			ptrColl->SetFixed(false);
			return;
		}

	}

}