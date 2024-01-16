/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_Speed(5.0f),
		m_Life(m_DefaultLife)
	{
		m_differenceMatrix.affineTransformation(
			Vec3(0.5f, 0.5f, 0.5f),
			Vec3(0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.8f, 0.0f)
		);
	}
	
	void Player::OnCreate() {
		auto ptr = AddComponent<Transform>();
		ptr->SetScale(0.5f, 0.5f, 0.5f);
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(Vec3(0.0f, 0.85f, 0.0f));

		//衝突判定を付ける
		auto ptrColl = AddComponent<CollisionCapsule>();
		ptrColl->SetDrawActive(false);

		//各パフォーマンスを得る
		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);

		auto ptrGracity = AddComponent<Gravity>();

		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);

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
		MovePlayer();
		PlayerDead();
		HitCheck();

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
			// Xボタンが押されたら攻撃モーションに変更
			if (pad.wPressedButtons & BUTTON_SHOT)
			{
				m_currentMotion = AttackStart;
			}
			break;
		default:
			break;
		}

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

	void Player::MovePlayer() {
		float elapsedTime = App::GetApp()->GetElapsedTime();
		auto angle = GetMoveVector();
		if (angle.length() > 0.0f && m_currentMotion == Wait) {
			m_currentMotion = WalkStart;
		}
		switch (m_currentMotion)
		{
		case Wait:
		case WalkStart:
		case Walking1:
		case Walking2:
		case WalkEnd1:
		case WalkEnd2:
			if (angle.length() > 0.0f) {
				auto pos = GetComponent<Transform>()->GetPosition();
				pos += angle * elapsedTime * m_Speed;
				GetComponent<Transform>()->SetPosition(pos);
			}
			//回転の計算
			if (angle.length() > 0.0f) {
				auto utilPtr = GetBehavior<UtilBehavior>();
				utilPtr->RotToHead(angle, 1.0f);
			}
			break;
		case AttackStart:
			//回転の計算
			if (angle.length() > 0.0f) {
				auto utilPtr = GetBehavior<UtilBehavior>();
				utilPtr->RotToHead(angle, 1.0f);
			}
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
		totalTime += totalTime < invincibleTime;
		if (HitFlg && totalTime >= invincibleTime) {
			AddPlayerDamage(10, Damage1);
			totalTime = 0;
		}
	}

	//ダメージ関数
	void Player::AddPlayerDamage(int damage, eMotion Motion) {
		switch (m_currentMotion)
		{
		case Wait:
		case WalkStart:
		case Walking1:
		case Walking2:
		case WalkEnd1:
		case WalkEnd2:
			m_currentMotion = Motion;
			break;
		case AttackStart:
		case AttackEnd:
			if (Motion == Damage2) {
				m_currentMotion = Motion;
			}
			break;

		default:
			break;
		}

		m_Life -= damage;
		if (m_Life < 0) {
			m_Life = 0;
		}

		Vec3 pos = GetComponent<Transform>()->GetPosition();
		m_DamegeEfkPlay = ObjectFactory::Create<EfkPlay>(m_damageEffect, pos, Vec3(0.5f));
	}

	void Player::PlayerDead() {
		if (m_Life <= 0) {
			m_currentMotion = Dead;
		}
	}

	void Player::AnimationUpdate()
	{
		// アニメーションの再生
		auto ptrDraw = GetComponent<BcPNTBoneModelDraw>();
		// アニメーションのタイプが変わっていたら
		if (m_currentMotion != m_pastMotion || ptrDraw->GetCurrentAnimation() != m_motionKey.at(m_currentMotion))
		{
			// タイプに応じてアニメーションを変更する
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
			ptrDraw->UpdateAnimation(deltaTime * 2.0f);
			break;
		case AttackEnd:
			ptrDraw->UpdateAnimation(deltaTime * 2.0f);
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
		default:
			ptrDraw->UpdateAnimation(deltaTime * 1.0f);
			break;
		}

		// アニメーションが終了しているかをチェック
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
				PostEvent(1.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameOverStage");
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
			HitFlg = true;
		}
	}

	void Player::OnCollisionExit(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Golem"))
		{
			HitFlg = false;
		}
	}

}
//end basecross

