/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)

	{}

	Vec2 Player::GetInputState() const{
		Vec2 ret;
		//コントローラ取得
		auto controllerVec = App::GetApp()->GetInputDevice().GetControlerVec();
		ret.x = 0.0f;
		ret.y = 0.0f;
		if (controllerVec[0].bConnected)
		{
			ret.x = controllerVec[0].fThumbLX;
			ret.y = controllerVec[0].fThumbLY;
		}

		//キーボード取得
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (KeyState.m_bPushKeyTbl['W'])
		{
			ret.y = 1.0f;
		}
		if (KeyState.m_bPushKeyTbl['S'])
		{
			ret.y = -1.0f;
		}
		if (KeyState.m_bPushKeyTbl['A'])
		{
			ret.x = -1.0f;
		}
		if (KeyState.m_bPushKeyTbl['D'])
		{
			ret.x = 1.0f;
		}
		return ret;
	}

	Vec3 Player::GetMoveVector() const {
		Vec3 angle(0, 0, 0);
		auto inPut = GetInputState();//入力の取得
		float moveX = inPut.x;
		float moveZ = inPut.y;
		if (moveX != 0 || moveZ != 0) {
			float moveLength = 0;	//動いた時のスピード
			auto ptrTransform = GetComponent<Transform>();
			auto ptrCamera = OnGetDrawCamera();
			//進行方向の向きを計算
			auto front = ptrTransform->GetPosition() - ptrCamera->GetEye();
			front.y = 0;
			front.normalize();
			//進行方向向きからの角度を算出
			float frontAngle = atan2(front.z, front.x);
			//コントローラの向き計算
			Vec2 moveVec(moveX, moveZ);
			float moveSize = moveVec.length();
			//コントローラの向きから角度を計算
			float cntlAngle = atan2(-moveX, moveZ);
			//トータルの角度を算出
			float totalAngle = frontAngle + cntlAngle;
			//角度からベクトルを作成
			angle = Vec3(cos(totalAngle), 0, sin(totalAngle));
			//正規化する
			angle.normalize();
			//移動サイズを設定。
			angle *= moveSize;
			//Y軸は変化させない
			angle.y = 0;
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

	void Player::OnCreate() {
		auto ptr = AddComponent<Transform>();
		ptr->SetScale(0.5f, 1.0f, 0.5f);
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(Vec3(0.0f, 0.1, 0.0f));

		auto ptrColl = AddComponent<CollisionRect>();

		//各パフォーマンスを得る
		//GetStage()->SetCollisionPerformanceActive(true);
		//GetStage()->SetUpdatePerformanceActive(true);
		//GetStage()->SetDrawPerformanceActive(true);

		auto ptrGracity = AddComponent<Gravity>();

		//GetStage()->SetCollisionPerformanceActive(true);
		//GetStage()->SetUpdatePerformanceActive(true);
		//GetStage()->SetDrawPerformanceActive(true);

		//影をつける（シャドウマップを描画する）
		auto shadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto  ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//ptrDraw->SetTextureResource(L"TRACE_TX");
		SetAlphaActive(true);


		//カメラを得る
		//auto ptrCamera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		//if (ptrCamera) {
		//	//MyCameraである
		//	//MyCameraに注目するオブジェクト（プレイヤー）の設定
		//	ptrCamera->SetTargetObject(GetThis<GameObject>());
		//	ptrCamera->SetTargetToAt(Vec3(0, 0.25f, 0));
		//}

	}

	void Player::OnUpdate() {

	}
}
//end basecross

