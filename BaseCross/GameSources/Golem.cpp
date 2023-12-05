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
		m_transform->SetScale(Vec3(4.3f));
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
		ptrDraw->AddAnimation(L"ATTACKSTART_SWINGDOWN", 210, 40, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_SWINGDOWN", 250, 50, false);
		ptrDraw->AddAnimation(L"ATTACKSTART_PUNCH", 310, 45, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_PUNCH", 355, 75, false);
		ptrDraw->AddAnimation(L"ATTACKSTART_RAMMING", 440, 60, false);
		ptrDraw->AddAnimation(L"ATTACKING_RAMMING1", 500, 30, false);
		ptrDraw->AddAnimation(L"ATTACKING_RAMMING2", 500, 30, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_RAMMING1", 530, 30, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_RAMMING2", 560, 30, false);
		ptrDraw->AddAnimation(L"ATTACKFINISH_RAMMING3", 600, 120, false);
		ptrDraw->AddAnimation(L"STUN_RAMMING_FORWARD", 770, 60, false);
		ptrDraw->AddAnimation(L"STUN_RAMMING_BEHIND", 840, 30, false);
		ptrDraw->AddAnimation(L"STUN_NORMAL_FORWARD", 880, 50, false);
		ptrDraw->AddAnimation(L"STUN_NORMAL_BEHIND", 940, 30, false);
		ptrDraw->AddAnimation(L"STUN1", 970, 1, false);
		ptrDraw->AddAnimation(L"STUN2", 970, 1, false);
		ptrDraw->AddAnimation(L"STUN_RECOVERY", 980, 120, false);
		ptrDraw->AddAnimation(L"DEATH", 600, 1, false);

		// 最初のアニメーションを指定
		ptrDraw->ChangeCurrentAnimation(L"BOOTING");



		// 影を追加
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMultiMeshResource(L"GOLEM");
		ptrShadow->SetMeshToTransformMatrix(m_differenceMatrix);

		m_playerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");

		// タグの設定
		AddTag(L"Enemy");
		AddTag(L"Golem");

	}

	void Golem::OnUpdate() {

		Debug::Log(AngleCalculation(m_playerPtr.lock()->GetComponent<Transform>()));

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
			switch (m_currentMotion) {
			case eMotion::Booting:
				m_motion = WalkStart;
				break;

			case eMotion::WalkStart:
				m_motion = Walking1;
				break;

			case eMotion::Walking1:
				// カウントが攻撃間隔に達したか
				if (m_countTime > m_status.attackInterspace) {
					m_motion = WalkFinish;
					m_countTime = 0;
				}
				else {
					m_motion = Walking2;
				}
				break;

			case eMotion::Walking2:
				// カウントが攻撃間隔に達したか
				if (m_countTime > m_status.attackInterspace) {
					m_motion = WalkFinish;
					m_countTime = 0;
				}
				else {
					m_motion = Walking1;
				}
				break;

			case eMotion::WalkFinish:
				// 攻撃範囲内にプレイヤーが居るかをチェック
				m_canSwingDown = CheckAttackArea(eAttack::SwingDown);
				m_canPunch = CheckAttackArea(eAttack::Punch);
				// 現在の状況に応じて攻撃モーションを決定する
				if (m_canSwingDown && !m_canPunch) {
					m_motion = AttackStart_Swingdown;
				}
				if (!m_canSwingDown && m_canPunch) {
					m_motion = AttackStart_Punch;
				}
				// どちらの攻撃も可能な場合はランダムに選択される
				if (m_canSwingDown && m_canPunch) {
					int random = rand() % 2;
					if (random == 0) {
						m_motion = AttackStart_Swingdown;
					}
					if (random == 1) {
						m_motion = AttackStart_Punch;
					}
				}
				else {
					// 上記の攻撃範囲内にもいない場合はプレイヤーに向かって突進攻撃をする
					m_motion = AttackStart_Ramming;
				}
				break;

			case eMotion::AttackStart_Swingdown:
				// 攻撃範囲内にプレイヤーが居たらダメージ
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				m_motion = AttackFinish_Swingdown;
				break;

			case eMotion::AttackFinish_Swingdown:
				m_motion = WalkStart;
				break;

			case eMotion::AttackStart_Punch:
				// 攻撃範囲内にプレイヤーが居たらダメージ
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				m_motion = AttackFinish_Punch;
				break;

			case eMotion::AttackFinish_Punch:
				m_motion = WalkStart;
				break;

			case eMotion::AttackStart_Ramming:
				if (m_countTime > m_status.attackInterspace || m_stopRammingFlg) {
					m_motion = AttackFinish_Ramming1;
					m_stopRammingFlg = false;
				}
				else {
					m_motion = Attacking_Ramming1;
					m_rammingPos = m_transform->GetPosition();
				}
				break;

			case eMotion::Attacking_Ramming1:
				// カウントが攻撃間隔に達する、または突進攻撃を中止するフラグがtrueなら
				if (m_countTime > m_status.attackInterspace) {
					m_motion = AttackFinish_Ramming3;
					m_countTime = 0;
				}
				if (m_stopRammingFlg) {
					m_motion = AttackFinish_Ramming1;
					m_countTime = 0;
				}
				else {
					m_motion = Attacking_Ramming2;
				}
				break;

			case eMotion::Attacking_Ramming2:
				// カウントが攻撃間隔に達したなら
				if (m_countTime > m_status.attackInterspace) {
					m_motion = AttackFinish_Ramming3;
					m_countTime = 0;
				}
				// 突進攻撃を中止するフラグがtrueなら
				if (m_stopRammingFlg) {
					m_motion = AttackFinish_Ramming1;
					m_countTime = 0;
				}
				else {
					m_motion = Attacking_Ramming1;
				}
				break;

			case eMotion::AttackFinish_Ramming1:
				m_motion = AttackFinish_Ramming2;
				m_stopRammingFlg = false;
				break;

			case eMotion::AttackFinish_Ramming2:
				m_motion = WalkStart;
				break;

			case eMotion::AttackFinish_Ramming3:
				m_motion = WalkStart;
				break;

			case eMotion::Stun_Ramming_Forward:
				m_motion = Stun1;
				break;

			case eMotion::Stun_Ramming_Behind:
				m_motion = Stun1;
				break;

			case eMotion::Stun_Normal_Forward:
				m_motion = Stun1;
				break;

			case eMotion::Stun_Normal_Behind:
				m_motion = Stun1;
				break;

			case eMotion::Stun1:
				if (m_stunTotalTime < m_stunDuration) {
					m_motion = Stun2;
				}
				else {
					m_motion = Stun_Recovery;
				}

				m_stunTotalTime++;
				break;

			case eMotion::Stun2:
				if (m_stunTotalTime < m_stunDuration) {
					m_motion = Stun1;
				}
				else {
					m_motion = Stun_Recovery;
				}

				m_stunTotalTime++;
				break;

			case eMotion::Stun_Recovery:
				m_motion = WalkStart;
				m_stunDuration = 0;
				m_stunTotalTime = 0;
				break;

			case eMotion::Death:
				// ゴーレムを消去する
				GetStage()->RemoveGameObject<Golem>(GetThis<Golem>());
				break;

			default:
				break;
			}
		}

		// スタン攻撃を受けていて、現在のモーションが条件を満たしている場合、スタン状態に移行
		if (m_stunAttackFlg) {
			switch (m_motion)
			{
			case WalkStart:
			case Walking1:
			case Walking2:
			case WalkFinish:
			case AttackFinish_Swingdown:
			case AttackFinish_Punch:
			case AttackFinish_Ramming2:
			case AttackFinish_Ramming3:
			case Stun_Recovery:
				// プレイヤーが前方に居た場合
				if (m_rockAngle <= 60.0f) {
					m_motion = Stun_Normal_Forward;
				}
				else {
					m_motion = Stun_Normal_Behind;
				}
				break;

			case AttackFinish_Ramming1:
				if (m_rockAngle <= 60.0f) {
					m_motion = Stun_Ramming_Forward;
				}
				else {
					m_motion = Stun_Ramming_Behind;
				}
				break;

			case Stun1:
			case Stun2:
				break;

			default:
				m_stunDuration = 0;
				break;
			}

			m_stunAttackFlg = false;
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

		// モーションに応じて再生する
		switch (m_motion)
		{
		case AttackStart_Punch:
			ptrDraw->UpdateAnimation(deltaTime * 1.5f);
			break;
		default:
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			break;
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
				if (Rotation.y - m_rotation.y > m_attackStatus.NormalTurningSpeed * (XM_PI / 180.0f)) {
					m_rotation.y += m_attackStatus.NormalTurningSpeed * (XM_PI / 180.0f);
				}
				else {
					m_rotation.y = Rotation.y;
				}
			}
			if (m_rotation.y > Rotation.y) {
				if (m_rotation.y - Rotation.y > m_attackStatus.NormalTurningSpeed * (XM_PI / 180.0f)) {
					m_rotation.y -= m_attackStatus.NormalTurningSpeed * (XM_PI / 180.0f);
				}
				else {
					m_rotation.y = Rotation.y;
				}
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
				m_rotation.y += m_attackStatus.RammingTurningSpeed * (XM_PI / 180.0f);
			}
			if (m_rotation.y > Rotation.y) {
				m_rotation.y -= m_attackStatus.RammingTurningSpeed * (XM_PI / 180.0f);
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
			m_velocity *= m_attackStatus.RammingSpeed;
		}

		
		// 座標を更新
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

	bool Golem::CheckAttackArea(eAttack attack) {
		// 攻撃範囲
		float attackRange = 0.0f;

		// 攻撃に応じて攻撃範囲を代入
		if (attack == SwingDown) {
			attackRange = m_attackStatus.SwingdownRange;
		}
		if (attack == Punch) {
			attackRange = m_attackStatus.PunchRange;
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

		if (attack == SwingDown || attack == Punch) {
			// 攻撃範囲の描写
			GetStage()->AddGameObject<FillSprite>(areaPos, attackRange, attackRange, wstring(L"ATTACKAREA"));
		}
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

	void Golem::AddStun(int StunTime) {
		// スタン攻撃を受けたのでフラグを立てる
		m_stunAttackFlg = true;
		// スタンの継続時間をリセットする
		m_stunDuration = StunTime;
		// スタンの合計時間をリセットする
		m_stunTotalTime = 0;
	}

	float Golem::AngleCalculation(shared_ptr<Transform>& target) {
		// ターゲットの座標を取得
		Vec3 targetPos = target->GetPosition();
		// ゴーレムの座標を取得
		Vec3 golemPos = m_transform->GetPosition();

		// y座標は考慮しない
		targetPos.y = 0.0f;
		golemPos.y = 0.0f;

		// 長さの比較
		float length = (targetPos - golemPos).length();

		// ターゲットまでの座標
		Vec3 toTarget = targetPos - golemPos;
		toTarget.normalize();

		// ゴーレムの向いている方向
		Vec3 forward = -m_transform->GetForward();
		forward.normalize();

		// 内積の計算
		float dot = forward.dot(toTarget);
		dot = min(dot, 1.0f);
		dot = max(dot, -1.0f);

		// 角度
		float angle = XMConvertToDegrees(acosf(dot));

		return angle;
	}

	void Golem::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		// プレイヤーとの衝突
		if (Other->FindTag(L"Player"))
		{
			auto ptrColl =GetComponent<CollisionCapsule>();
			ptrColl->SetFixed(true);
			return;
		}

		// 発射された岩石との衝突
		if (Other->FindTag(L"MoveFallingRocks"))
		{
			// スタン攻撃を受けた
			AddStun(300.0f);
			m_rockAngle = AngleCalculation(Other->GetComponent<Transform>());
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

			// 岩石との衝突
			if (Other->FindTag(L"FallingRocks"))
			{
				m_stopRammingFlg = true;
				return;
			}
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