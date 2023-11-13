/*!
@file Field.cpp
@brief フィールド生成
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//ボックス
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox() {}

	//初期化
	void FixedBox::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);
		//OBB衝突判定を付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		AddTag(L"FixedBox");//タグをつける
		auto shadowPtr = AddComponent<Shadowmap>();//影をつける（シャドウマップを描画する）
		//影の形（メッシュ）を設定
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetTextureResource(L"FIELD");//テクスチャを反映する
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetOwnShadowActive(true);

	}


	//壁
	Wall::Wall(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		FixedBox(StagePtr, Scale, Rotation, Position)
	{
	}
	Wall::~Wall() {}

	//初期化
	void Wall::OnCreate() {
		// 継承元のOnCreateの呼び出し
		FixedBox::OnCreate();

		// タグの設定
		AddTag(L"Wall");
	}


	//床
	Floor::Floor(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		FixedBox(StagePtr, Scale, Rotation, Position)
	{
	}
	Floor::~Floor() {}

	//初期化
	void Floor::OnCreate() {
		// 継承元のOnCreateの呼び出し
		FixedBox::OnCreate();

		// タグの設定
		AddTag(L"Floor");
	}


	//円柱
	FixedCylinder::FixedCylinder(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedCylinder::~FixedCylinder() {}

	//初期化
	void FixedCylinder::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);
		//OBB衝突判定を付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetAfterCollision(AfterCollision::Auto);
		ptrColl->SetFixed(true);
		AddTag(L"FixedCylinder");//タグをつける
		auto shadowPtr = AddComponent<Shadowmap>();//影をつける（シャドウマップを描画する）
		//影の形（メッシュ）を設定
		shadowPtr->SetMeshResource(L"DEFAULT_CYLINDER");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CYLINDER");
		ptrDraw->SetTextureResource(L"PILLAR");//テクスチャを反映する
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetOwnShadowActive(true);
	}

	void FixedCylinder::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"FallingRocks")) {
			GetStage()->RemoveGameObject<FallingRocks>(GetThis<FallingRocks>());
			GetStage()->AddGameObject<FallingRocks>();
			return;
		}
	}

}
