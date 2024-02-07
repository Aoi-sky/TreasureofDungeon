/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{	
	void Player::OnCreate() {
		m_ptrTrans = GetComponent<Transform>();
		m_ptrTrans->SetScale(0.5f, 0.5f, 0.5f);
		m_ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		m_ptrTrans->SetPosition(Vec3(0.0f, 3.0f, 0.0f));

		//衝突判定を付ける
		auto ptrColl = AddComponent<CollisionCapsule>();
		ptrColl->SetMakedHeight(3.0f);
		ptrColl->SetDrawActive(true);

		//各パフォーマンスを得る
		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);
		// 重力を設定
		auto ptrGracity = AddComponent<Gravity>();

		//影をつける（シャドウマップを描画する）
		auto shadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		shadowPtr->SetMultiMeshResource(L"M_PLAYER");

		//描画コンポーネントの設定
		auto  ptrDraw = AddComponent<BcPNTBoneModelDraw>();
		// 新規ドローコンポーネントの設定
		ptrDraw->SetMultiMeshResource(L"M_PLAYER");
		ptrDraw->SetMeshToTransformMatrix(m_differenceMatrix);
		//アニメーション設定
		ptrDraw->AddAnimation(L"Wait", 10, 30, true);
		ptrDraw->AddAnimation(L"WalkStart", 50, 10, false);
		ptrDraw->AddAnimation(L"Walking1", 60, 10, false);
		ptrDraw->AddAnimation(L"Walking2", 70, 10, false);
		ptrDraw->AddAnimation(L"WalkEnd1", 80, 10, false);
		ptrDraw->AddAnimation(L"WalkEnd2", 100, 10, false);
		ptrDraw->AddAnimation(L"AttackStart", 120, 35, false);
		ptrDraw->AddAnimation(L"AttackEnd", 155, 25, false);
		ptrDraw->AddAnimation(L"Damage1", 190, 10, false);
		ptrDraw->AddAnimation(L"Damage2", 210, 90, false);
		ptrDraw->AddAnimation(L"Dead", 210, 30, false);
		ptrDraw->AddAnimation(L"Dash1", 310, 5, false);
		ptrDraw->AddAnimation(L"Dash2", 315, 25, false);
		ptrDraw->AddAnimation(L"Dash3", 340, 5, false);
		ptrDraw->AddAnimation(L"Dash4", 345, 15, false);
		ptrDraw->ChangeCurrentAnimation(L"Wait");

		//透明処理
		SetAlphaActive(true);
		SetDrawActive(true);

		// タグの設定
		AddTag(L"Player");

		//カメラを取得
		auto ptrCamera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		if (ptrCamera) {
			//MyCameraである
			//MyCameraに注目するオブジェクト（プレイヤー）の設定
			ptrCamera->SetTargetObject(GetThis<GameObject>());
			ptrCamera->SetTargetToAt(Vec3(0, 0.25f, 0));
		}

		//エフェクトの初期化
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		auto ShEfkInterface = App::GetApp()->GetScene<Scene>()->GetEfkInterface();
		m_damageEffectStr = DataDir + L"Effects\\" + L"damage.efk";
		m_damageEffect = ObjectFactory::Create<EfkEffect>(ShEfkInterface, m_damageEffectStr);

	}

	void Player::OnUpdate() {
		// 移動処理
		OnMove();
		// プレイヤーの生死確認
		PlayerDeadCheck();
		// スタミナ回復
		StaminaRecovery();
		// 敵と接触しているかをチェック
		HitCheck();
		// 入力があるかチェック
		GamepadInputCheck();
		// プレイヤーのアニメーションをアップデートする
		AnimationUpdate();
	}

	Vec2 Player::GetInputState() const{
		Vec2 ret;
		auto controllerVec = App::GetApp()->GetInputDevice().GetControlerVec();//コントローラ取得
		ret.x = 0.0f;
		ret.y = 0.0f;
		if (controllerVec[0].bConnected)
		{
			ret.x = controllerVec[0].fThumbLX;
			ret.y = controllerVec[0].fThumbLY;
		}

		return ret;
	}
	//playerの移動
	Vec3 Player::GetMoveVector() const {
		Vec3 angle(0, 0, 0);
		auto inPut = GetInputState();//入力の取得
		float moveX = inPut.x;
		float moveZ = inPut.y;
		if (moveX != 0 || moveZ != 0) {
			float moveLength = 0;	//動いた時のスピード
			auto ptrTransform = GetComponent<Transform>();
			auto ptrCamera = OnGetDrawCamera();
			auto front = ptrTransform->GetPosition() - ptrCamera->GetEye();//進行方向の向きを計算
			front.y = 0;
			front.normalize();
			float frontAngle = atan2(front.z, front.x);//進行方向向きからの角度を算出
			//コントローラの向き計算
			Vec2 moveVec(moveX, moveZ);
			float moveSize = moveVec.length();
			float cntlAngle = atan2(-moveX, moveZ);//コントローラの向きから角度を計算
			float totalAngle = frontAngle + cntlAngle;//トータルの角度を算出
			angle = Vec3(cos(totalAngle), 0, sin(totalAngle));//角度からベクトルを作成
			angle.normalize();//正規化する
			angle *= moveSize;//移動サイズを設定
			angle.y = 0;//Y軸は変化させない
		}
		return angle;
	}

	float Player::GetMoveAngle()
	{
		Vec3 angle(0, 0, 0);
		auto inPut = GetInputState();//入力の取得
		float moveX = inPut.x;
		float moveZ = inPut.y;
		if (moveX != 0 || moveZ != 0) {
			float moveLength = 0;	//動いた時のスピード
			auto ptrTransform = GetComponent<Transform>();
			auto ptrCamera = OnGetDrawCamera();
			auto front = ptrTransform->GetPosition() - ptrCamera->GetEye();//進行方向の向きを計算
			front.y = 0;
			front.normalize();
			float frontAngle = atan2(front.z, front.x);//進行方向向きからの角度を算出
			//コントローラの向き計算
			Vec2 moveVec(moveX, moveZ);
			float moveSize = moveVec.length();
			float cntlAngle = atan2(moveX, moveZ);//コントローラの向きから角度を計算
			float cameraAngle = -dynamic_pointer_cast<MyCamera>(ptrCamera)->GetRadRX();
			return cntlAngle + cameraAngle;//トータルの角度を算出
		}
		return 0.0f;
	}

	void Player::OnMove() {
		float elapsedTime = App::GetApp()->GetElapsedTime();
		m_currentAngle = GetMoveVector();
		float rotY = GetMoveAngle();
		auto ptrTrans = GetComponent<Transform>();
		auto pos = GetComponent<Transform>()->GetPosition();
		auto forward = GetComponent<Transform>()->GetForward();

		if (m_currentAngle.length() > 0.0f && m_currentMotion == Wait) {
			// Wait中に入力があった場合、歩行モーションに変更
			m_currentMotion = WalkStart;
		}
		switch (m_currentMotion)
		{
		case Wait:
			if (m_currentAngle.length() > 0.0f)
			{
				ptrTrans->SetRotation(0.0f, rotY, 0.0f);
			}
			else
			{
				auto currentRotY = ptrTrans->GetQuaternion().toRotVec().y;
				ptrTrans->SetRotation(0.0f, currentRotY, 0.0f);
			}
			break;

		case WalkStart:
		case Walking1:
		case Walking2:
		case WalkEnd1:
		case WalkEnd2:
			if (m_currentAngle.length() > 0.0f) {
				pos += m_currentAngle * elapsedTime * m_status.at(WalkSpeed);
				ptrTrans->SetPosition(pos);
			}
			//回転の計算
			if (m_currentAngle.length() > 0.0f) {
				ptrTrans->SetRotation(0.0f, rotY, 0.0f);
			}
			break;
		case AttackStart:
			//回転の計算
			if (m_currentAngle.length() > 0.0f) {
				auto utilPtr = GetBehavior<UtilBehavior>();
				utilPtr->RotToHead(m_currentAngle, 1.0f);
			}
			break;
		case Dash1:
			if (m_currentAngle.length() > 0.0f) {
				ptrTrans->SetRotation(0.0f, rotY, 0.0f);
			}

			pos += forward * elapsedTime * m_status.at(StepSpeed);
			ptrTrans->SetPosition(pos);
			break;
		case Dash2:
			pos += forward * elapsedTime * m_status.at(DashSpeed);
			ptrTrans->SetPosition(pos);
			break;
		case Dash4:
			pos -= forward * elapsedTime * m_status.at(StepSpeed);
			ptrTrans->SetPosition(pos);
			break;
		default:
			break;
		}		
	}

	void Player::OnAttack() {
		// プレイヤーが所属している「ステージ」を取得し、
		// そこにBulletオブジェクトを追加する
		GetStage()->AddGameObject<Wave>(GetThis<Player>()); //自分自身のオブジェクトのポインタを取得する
	}

	void Player::HitCheck() {
		m_totalTime += m_totalTime < m_invincibleTime;
		if (m_hitFlg && m_totalTime >= m_invincibleTime) {
			AddDamage(10, Damage1);
			m_totalTime = 0;
		}
	}

	void Player::GamepadInputCheck() {
		auto& app = App::GetApp();
		auto device = app->GetInputDevice(); // インプットデバイスオブジェクトを取得する
		auto& pad = device.GetControlerVec()[0]; // １個目のコントローラーの状態を取得する
		switch (m_currentMotion)
		{
		case Wait:
		case WalkStart:
		case Walking1:
		case Walking2:
		case WalkEnd1:
		case WalkEnd2:
			// 指定されたボタンが押された場合
			if (pad.wPressedButtons & BUTTON_ATTACK)
			{
				// 攻撃モーションに変更
				m_currentMotion = AttackStart;
			}
		case AttackStart:
		case AttackEnd:
		case Dash3:
		case Dash4:
			// 指定されたボタンが押されていた場合
			if (pad.wPressedButtons & BUTTON_DASH)
			{
				// スタミナが足りていた場合
				if (m_stamina >= m_status.at(StaminaConsumption)) {
					// ダッシュモーションに変更
					m_currentMotion = Dash1;
					// スタミナ減少
					m_stamina -= m_status.at(StaminaConsumption);
				}
			}
			break;
		default:
			break;
		}
	}

	//ダメージ関数
	void Player::AddDamage(int damage, eMotion Motion) {
		switch (m_currentMotion)
		{
		case Wait:
		case WalkStart:
		case Walking1:
		case Walking2:
		case WalkEnd1:
		case WalkEnd2:
		case Damage1:
			// モーションを変更する
			m_currentMotion = Motion;
			break;
		case AttackStart:
		case AttackEnd:
			if (Motion == Damage2) {
				// 攻撃中は重度の被弾時のみモーションを変更する
				m_currentMotion = Motion;
			}
			break;

		default:
			break;
		}

		// 体力を減少させる
		m_life -= damage;
		if (m_life < 0) {
			m_life = 0;
		}
		if (damage != 0) {
		Vec3 pos = GetComponent<Transform>()->GetPosition();
		m_damageEfkPlay = ObjectFactory::Create<EfkPlay>(m_damageEffect, pos, Vec3(0.5f));
		}
	}

	void Player::PlayerDeadCheck() {
		if (m_life <= 0) {
			m_currentMotion = Dead;
		}
	}

	void Player::StaminaRecovery() {
		// ダッシュ中と死亡時以外のモーションのみスタミナ回復
		switch (m_currentMotion)
		{
		case Wait:
		case WalkStart:
		case Walking1:
		case Walking2:
		case WalkEnd1:
		case WalkEnd2:
		case AttackStart:
		case AttackEnd:
		case Damage1:
		case Damage2:
			// 前フレームからの経過時間に応じてスタミナ回復
			m_stamina += m_status.at(StaminaRecoverySpeed) * App::GetApp()->GetElapsedTime();
			if (m_stamina > m_status.at(MaxStamina)) {
				m_stamina = m_status.at(MaxStamina);
			}
			break;
		default:
			break;
		}
	}

	void Player::AnimationUpdate()
	{
		// モーションを再生
		auto ptrDraw = GetComponent<BcPNTBoneModelDraw>();
		// モーションのタイプが変わっていたら
		if (m_currentMotion != m_pastMotion || ptrDraw->GetCurrentAnimation() != m_motionKey.at(m_currentMotion))
		{
			// タイプに応じてモーションを変更する
			ptrDraw->ChangeCurrentAnimation(m_motionKey.at(m_currentMotion));
			m_pastMotion = m_currentMotion;
		}

		// 前フレームからのデルタタイムを取得
		float deltaTime = App::GetApp()->GetElapsedTime();

		// モーションに応じて再生する
		switch (m_currentMotion)
		{
		case Wait:
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			break;
		case WalkStart:
			ptrDraw->UpdateAnimation(deltaTime * 2.0f);
			break;
		case Walking1:
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			break;
		case Walking2:
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			break;
		case WalkEnd1:
			ptrDraw->UpdateAnimation(deltaTime * 2.0f);
			break;
		case WalkEnd2:
			ptrDraw->UpdateAnimation(deltaTime * 2.0f);
			break;
		case AttackStart:
			ptrDraw->UpdateAnimation(deltaTime * m_status.at(AttackSpeed));
			break;
		case AttackEnd:
			ptrDraw->UpdateAnimation(deltaTime * m_status.at(AttackSpeed));
			break;
		case Damage1:
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			break;
		case Damage2:
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			break;
		case Dead:
			ptrDraw->UpdateAnimation(deltaTime * 0.5f);
			break;
		case Dash1:
			ptrDraw->UpdateAnimation(deltaTime * 2.0f);
			break;
		case Dash2:
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			break;
		case Dash3:
			ptrDraw->UpdateAnimation(deltaTime * 2.0f);
			break;
		case Dash4:
			ptrDraw->UpdateAnimation(deltaTime * 2.0f);
			break;
		default:
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			break;
		}

		// モーションが終了しているかをチェック
		if (ptrDraw->IsTargetAnimeEnd()) {
			auto angle = GetMoveVector();

			switch (m_pastMotion)
			{
			case Wait:
				break;
			case WalkStart:
				if (angle.length() > 0.0f) {
					m_currentMotion = Walking1;
				}
				else {
					m_currentMotion = WalkEnd1;
				}
				break;
			case Walking1:
				if (angle.length() > 0.0f) {
					m_currentMotion = Walking2;
				}
				else {
					m_currentMotion = WalkEnd2;
				}
				break;
			case Walking2:
				if (angle.length() > 0.0f) {
					m_currentMotion = Walking1;
				}
				else {
					m_currentMotion = WalkEnd1;
				}
				break;
			case AttackStart:
				OnAttack();
				m_currentMotion = AttackEnd;
				break;
			case AttackEnd:
			case Damage1:
			case Damage2:
				if (angle.length() > 0.0f) {
					m_currentMotion = WalkStart;
				}
				else {
					m_currentMotion = Wait;
				}
				break;
			case Dead:
				SetDrawActive(false);
				PostEvent(1.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(),
					L"ToGameOverStage");
				break;
			case Dash1:
				m_currentMotion = Dash2;
				break;
			case Dash2:
				m_currentMotion = Dash3;
				break;
			case Dash3:
				m_currentMotion = Dash4;
				break;
			case Dash4:
				m_currentMotion = Wait;
				break;
			default:
				m_currentMotion = Wait;
				break;
			}
		}
	}

	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Golem"))
		{
			Golem::eMotion motion = GetStage()->GetSharedGameObject<Golem>(L"Golem")->GetGolemCurrentMotion();
			if (motion != Golem::eMotion::Death && motion != Golem::eMotion::Stun1 && motion != Golem::eMotion::Stun2) {
				m_hitFlg = true;
			}
		}
	}

	void Player::OnCollisionExit(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Golem"))
		{
			m_hitFlg = false;
		}
	}
}
//end basecross