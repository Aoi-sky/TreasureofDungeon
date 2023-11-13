/*!
@file Gimmick.cpp
@brief 落石などの実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	FallingRocks::FallingRocks(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_Scale(Vec3(5.0f, 5.0f, 5.0f)),
		m_Rotation(Vec3(0,0,0)),
		m_Position(Vec3(0, 0, 0))
	{
		m_differenceMatrix.affineTransformation(
			Vec3(0.4f, 0.4f, 0.4f),
			Vec3(0.0f),
			Vec3(0.0f, XM_PIDIV2, 0.0f),
			Vec3(0.0f, -0.3f, 0.0f)
		);

	}
	FallingRocks::~FallingRocks() {}

	void FallingRocks::OnCreate(){
		int i = rand() % 3;
		int xp,zm;
		if (i == 0){
			xp = 1;
			zm = 1;
		}else if (i == 1){
			xp = -1;
			zm = 1;
		}else if (i == 2){
			xp = 1;
			zm = -1;
		}else{
			xp = -1;
			zm = -1;
		}
		int x = rand() % 15 * xp;
		int z = rand() % 30 * zm;

		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(Vec3(float(x), 15.0f, float(z)));
		
		//衝突判定を付ける
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(false);
		ptrColl->SetFixed(true);

		//重力をつける
		auto ptrGra = AddComponent<Gravity>();

		//影をつける
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(L"FALLINGROCKS");
		//描画処理
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"FALLINGROCKS");
		ptrDraw->SetTextureResource(L"PLAYER");
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->SetMeshToTransformMatrix(m_differenceMatrix);

		AddTag(L"FallingRocks");
	}

	void FallingRocks::OnUpdate(){
		// 地面より下に落下しない為の処理
		auto ptrTransform = GetComponent<Transform>();
		Vec3 pos = ptrTransform->GetPosition();
		auto ptrGra = AddComponent<Gravity>();

		if (pos.y < 2.0f) {
			pos.y = 2.0f;
			// 座標を地面上に設定
			ptrTransform->SetPosition(pos);
			// 重力を0に設定
			ptrGra->SetGravityVerocityZero();
		}
	}

	void FallingRocks::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Enemy"))//敵
		{
			GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
			return;
		}
		if (Other->FindTag(L"FixedCylinder")|| Other->FindTag(L"FallingRocks"))//柱・落石
		{
			GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
			GetStage()->AddGameObject<FallingRocks>();
			return;
		}
		if (Other->FindTag(L"Floor"))//床
		{

		}
	}

	void FallingRocks::OnCollisionExit(shared_ptr<GameObject>& Other) {

	}

}
//end basecross
