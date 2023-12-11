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
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);
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
	}

	//ダメージ関数
	void Player::AddPlayerDamage(int damage) {
		m_Life -= damage;
	}

	void Player::PlayerDead() {
		if (m_Life <= 0) {
			PostEvent(1.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameOverStage");
		}
	}
	
	void Player::OnCreate() {
		auto ptr = AddComponent<Transform>();
		ptr->SetScale(0.2f, 0.2f, 0.2f);
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(Vec3(0.0f, 0.85f, 0.0f));

		//衝突判定を付ける
		auto ptrColl = AddComponent<CollisionCapsule>();

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
		ptrDraw->AddAnimation(L"Wait", 0, 1, true, 10);
		ptrDraw->AddAnimation(L"Walk", 5,40 ,false, 10);
		ptrDraw->AddAnimation(L"Attack", 50, 60, false,10);
		ptrDraw->ChangeCurrentAnimation(L"Walk");



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
	}

	void Player::OnUpdate() {
		MovePlayer();
		PlayerDead();
		auto& app = App::GetApp();
		float delta = app->GetElapsedTime(); // 前フレームからの経過時間（60FPS）

		float elapsedTime = App::GetApp()->GetElapsedTime();
		//アニメーションを更新する
		auto ptrDraw = GetComponent<BcPNTBoneModelDraw>();
		ptrDraw->UpdateAnimation(elapsedTime);		


		auto device = app->GetInputDevice(); // インプットデバイスオブジェクトを取得する
		auto& pad = device.GetControlerVec()[0]; // １個目のコントローラーの状態を取得する
		// Xボタンが押されたら弾を発射する
		if (pad.wPressedButtons & BUTTON_SHOT)
		{
			ptrDraw->ChangeCurrentAnimation(L"Attack");

			// プレイヤーが所属している「ステージ」を取得し、
			// そこにBulletオブジェクトを追加する
			GetStage()->AddGameObject<Wave>(GetThis<Player>()); //自分自身のオブジェクトのポインタを取得する
		}
	}

	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Golem"))
		{
			AddPlayerDamage(5);
		}
	}

	void Player::OnCollisionExit(shared_ptr<GameObject>& Other) {
		
	}

}
//end basecross

