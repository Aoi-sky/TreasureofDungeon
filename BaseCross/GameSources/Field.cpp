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
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");//影の形（メッシュ）を設定
		
		m_mesh = MeshResource::CreateCube(1.0f, true); // ボックスを生成する
		const Vec3 scale(m_Scale / 10);

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(m_mesh);
		ptrDraw->SetTextureResource(L"FLOOR");//テクスチャを反映する
		ptrDraw->SetSamplerState(SamplerState::AnisotropicWrap); 

		auto vertices = m_mesh->GetBackupVerteces<VertexPositionNormalTexture>();
		for (auto& vertex : vertices) { 
			if (vertex.normal.z < 0.0f || vertex.normal.z > 0.0f){ // 法線がZ軸方向の向いていたらXY平面のポリゴン（前後）
				vertex.textureCoordinate.x *= scale.x;
				vertex.textureCoordinate.y *= scale.y;
			}
			if (vertex.normal.x < 0.0f || vertex.normal.x > 0.0f){ // 法線がX方向の向いていたらZY平面のポリゴン（左右）
				vertex.textureCoordinate.x *= scale.z;
				vertex.textureCoordinate.y *= scale.y;
			}
			if (vertex.normal.y < 0.0f || vertex.normal.y > 0.0f){ // 法線がY方向を向いていたらXZ平面のポリゴン（上下）
				vertex.textureCoordinate.x *= scale.x;
				vertex.textureCoordinate.y *= scale.z;
			}
		}
		m_mesh->UpdateVirtexBuffer<VertexPositionNormalTexture>(vertices);

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
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetTextureResource(L"WALL");//テクスチャを反映する

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
	}

	void FixedCylinder::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"FallingRocks")) {
			GetStage()->RemoveGameObject<FallingRocks>(GetThis<FallingRocks>());
			GetStage()->AddGameObject<FallingRocks>();
			return;
		}
	}

}
