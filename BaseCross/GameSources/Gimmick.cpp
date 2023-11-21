/*!
@file Gimmick.cpp
@brief 落石などの実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	FallingRocks::FallingRocks(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_Scale(Vec3(3.0f, 3.0f, 3.0f)),
		m_Rotation(Vec3(0,0,0)),
		m_Position(Vec3(0, 0, 0))
	{
		m_differenceMatrix.affineTransformation(
			Vec3(0.8f, 0.8f, 0.8f),
			Vec3(0.0f),
			Vec3(0.0f, XM_PIDIV2, 0.0f),
			Vec3(0.0f, 0.2f, 0.0f)
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
		m_StartPos.x = rand() % 15 * xp;
		m_StartPos.z = rand() % 30 * zm;
		m_StartPos.y = 15.0f;

		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_StartPos);
		
		//衝突判定を付ける
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetDrawActive(false);
		ptrColl->SetFixed(false);

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

		//エフェクトの初期化
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		m_damageEffectStr = DataDir + L"Effects\\" + L"damage.efk";
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto ShEfkInterface = scene->GetEfkInterface();
		m_damageEffect = ObjectFactory::Create<EfkEffect>(ShEfkInterface, m_damageEffectStr);

	}

	void FallingRocks::OnUpdate(){
		// 地面より下に落下しない為の処理
		auto ptrTransform = GetComponent<Transform>();
		Vec3 pos = ptrTransform->GetPosition();
		auto ptrGra = AddComponent<Gravity>();

		// 一度でも床に落ちていた場合は地面に固定する
		if (m_fallenFlg) {
			pos.y = 1.5f;
			// 重力を0に設定
			ptrGra->SetGravityVerocityZero();
		}
		pos.x = m_StartPos.x;
		pos.z = m_StartPos.z;
		// 座標を地面上に設定
		ptrTransform->SetPosition(pos);
	}

	void FallingRocks::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Wave"))
		{
			GetStage()->RemoveGameObject<FallingRocks>(GetThis<FallingRocks>());
			GetStage()->AddGameObject<MoveFallingRocks>(GetThis<FallingRocks>());

			auto pos = GetComponent<Transform>()->GetPosition();
			m_EfkPlay = ObjectFactory::Create<EfkPlay>(m_damageEffect, pos, Vec3(1.0f));

			return;

		}
		if (Other->FindTag(L"Enemy"))//敵
		{
			GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
			return;
		}
		if (Other->FindTag(L"FixedCylinder")|| Other->FindTag(L"FallingRocks"))//柱・落石
		{
			GetStage()->RemoveGameObject<GameObject>(GetThis<GameObject>());
			//GetStage()->AddGameObject<FallingRocks>();
			return;
		}
		if (Other->FindTag(L"Floor"))//敵
		{
			m_fallenFlg = true;
			return;
		}

	}

	void FallingRocks::OnCollisionExit(shared_ptr<GameObject>& Other) {

	}

}
//end basecross
