#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void CameraObject::OnCreate() {
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");//弾の形
		drawComp->SetDrawActive(true);

		//向きをベクトルで所得
		auto ownerTrans = m_owner->GetEye();
		m_transform = GetComponent<Transform>();//コンポーネントの取得
		m_transform->SetPosition(ownerTrans);//座標設定
		m_transform->SetScale(Vec3(5.0f));//サイズ調整
		auto ptrColl = AddComponent<CollisionSphere>();//コリジョンをつける
		ptrColl->SetAfterCollision(AfterCollision::None);//衝突判定
		ptrColl->SetSleepActive(true);
		ptrColl->SetDrawActive(true);

		AddTag(L"CameraObject");//タグの設定
	}

	void CameraObject::OnUpdate() {
		auto ownerTrans = m_owner->GetEye();
		m_transform->SetPosition(ownerTrans);
	}


	void CameraObject::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"Wall"))//柱
		{
			m_owner->OnUpdate2();
			//GetStage()->AddGameObject<MyCamera>()->CameraCollisionEnter();
			return;
		}
		if (Other->FindTag(L"FixedCylinder"))//壁
		{
			return;
		}
	}

}