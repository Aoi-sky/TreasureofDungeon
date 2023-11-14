/*!
@file GimmickMove.cpp
@brief 落石の移動
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void MoveFallingRocks::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"FALLINGROCKS");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		//ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"FALLINGROCKS");
		ptrDraw->SetTextureResource(L"PLAYER");
		ptrDraw->SetOwnShadowActive(true);

		//向きをベクトルで所得
		m_Positon = m_owner->GetComponent<Transform>()->GetPosition();
		auto ptrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player");
		m_forward = ptrPlayer->GetComponent<Transform>()->GetForward();

		m_transform = GetComponent<Transform>();//コンポーネントの取得
		m_transform->SetPosition(m_Positon.x, 3.0f, m_Positon.z);//座標設定
		m_transform->SetScale(Vec3(5.0f, 5.0f, 5.0f));//サイズ調整
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンをつける
		//衝突判定
		ptrColl->SetAfterCollision(AfterCollision::None);
		ptrColl->SetSleepActive(true);

		AddTag(L"MoveFallingRocks");//タグの設定
	}

	void MoveFallingRocks::OnUpdate() {
		float delta = App::GetApp()->GetElapsedTime();//デルタタイムを取得

		//移動処理
		auto pos = m_transform->GetPosition();
		pos += m_forward * m_speed * delta;
		m_transform->SetPosition(pos.x,5,pos.z);

	}
	void MoveFallingRocks::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"FixedCylinder") || Other->FindTag(L"FallingRocks"))//柱
		{
			GetStage()->RemoveGameObject<MoveFallingRocks>(GetThis<MoveFallingRocks>());
			return;
		}
		if (Other->FindTag(L"Wall"))//壁
		{
			GetStage()->RemoveGameObject<MoveFallingRocks>(GetThis<MoveFallingRocks>());
			return;
		}
		if (Other->FindTag(L"Enemy"))
		{
			GetStage()->RemoveGameObject<MoveFallingRocks>(GetThis<MoveFallingRocks>());
			return;
		}
	}
}
//end basecross
