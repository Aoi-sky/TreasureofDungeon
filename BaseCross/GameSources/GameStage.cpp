/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	//カメラの生成
	void GameStage::CreateViewLight() {
		auto ptrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto ptrMyCamera = ObjectFactory::Create<MyCamera>();
		ptrView->SetCamera(ptrMyCamera);
		ptrMyCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		ptrMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	
	//セルマップの作成
	// セルマップの生成
	void GameStage::CreateStageCellMap()
	{
		//セルマップを生成
		auto Ptr = AddGameObject<StageCellMap>(Vec3(0.0f, 0.0f, 0.0f), 1.0f, 50, 80);

		//セルマップの区画を表示
		Ptr->SetDrawActive(false);

		//シェアオブジェクトに登録
		SetSharedGameObject(L"StageCellMap", Ptr);
	}

	// セルマップ内のセルにコストを設定
	void GameStage::SetCellMapCost() {
		////セルマップ内にFixedBoxの情報をセット
		//auto PtrCellmap = GetSharedGameObject<StageCellMap>(L"StageCellMap");
		//auto BoxGroup = GetSharedObjectGroup(L"CellMap");
		////セルマップからセルの配列を取得
		//auto& CellVec = PtrCellmap->GetCellVec();
		////ボックスグループからボックスの配列を取得
		//auto& BoxVec = BoxGroup->GetGroupVector();
		//vector<AABB> ObjectsAABBVec;
		//for (auto& v : BoxVec) {
		//	auto FixedBoxPtr = dynamic_pointer_cast<FixedBox>(v.lock());
		//	if (FixedBoxPtr) {
		//		auto ColPtr = FixedBoxPtr->GetComponent<CollisionObb>();
		//		//ボックスの衝突判定かラッピングするAABBを取得して保存
		//		ObjectsAABBVec.push_back(ColPtr->GetObb().GetWrappedAABB());
		//	}
		//}
		////セル配列からセルをスキャン
		//for (auto& v : CellVec) {
		//	for (auto& v2 : v) {
		//		for (auto& vObj : ObjectsAABBVec) {
		//			if (HitTest::AABB_AABB_NOT_EQUAL(v2.m_PieceRange, vObj)) {
		//				//ボックスのABBとNOT_EQUALで衝突判定
		//				v2.m_Cost = -1;
		//				break;
		//			}
		//		}
		//	}
		//}
	}

//フィールド生成
	void GameStage::CreateField() {
		vector<vector<Vec3>> vec = {
			{//地面
				Vec3(50.0f, 1.0f, 80.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,0.0f,0.0f)//Position
			},
			{//北壁
				Vec3(50.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,10.0f,40.0f)//Position
			},
			{//南壁
				Vec3(50.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,10.0f,-40.0f)//Position
			},
			{//西壁
				Vec3(5.0f, 20.0f, 80.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(25.0f,10.0f,0.0f)//Position
			},
			{//東壁
				Vec3(5.0f, 20.0f, 80.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(-25.0f,10.0f,0.0f)//Position
			},
			{//天井
				Vec3(50.0f, 1.0f, 80.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,20.0f,0.0f)//Position
			}
		};
		for (auto v : vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
		
		//柱生成
		vector<vector<Vec3>> vec2 = {
			{
				Vec3(5.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(15.0f,10.0f,-25.0f)//Position
			},
			{
				Vec3(5.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(15.0f,10.0f,25.0f)//Position
			},
			{
				Vec3(5.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(-15.0f,10.0f,-25.0f)//Position
			},
			{
				Vec3(5.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(-15.0f,10.0f,25.0f)//Position
			},
			{
				Vec3(5.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(15.0f,10.0f,0.0f)//Position
			},
			{
				Vec3(5.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(-15.0f,10.0f,0.0f)//Position
			},
		};
		for (auto v : vec2) {
			AddGameObject<FixedCylinder>(v[0], v[1], v[2]);
		}
	}
	
	//プレイヤーの生成
	void GameStage::CreatePlayer() {
		auto ptrPlayer = AddGameObject <Player>();
		SetSharedGameObject(L"Player", ptrPlayer);
		ptrPlayer->AddTag(L"Player");
	}

	//敵の作成
	void GameStage::CreateEnemy() {
		//auto Ptrcellmap = GetSharedGameObject<StageCellMap>(L"StageCellMap");
		//AddGameObject<Enemy>(Ptrcellmap, Vec3(0, 1.0f, 15.0f));
	}

	void GameStage::CreateGolem() {
		auto Ptrcellmap = GetSharedGameObject<StageCellMap>(L"StageCellMap");
		AddGameObject<Golem>(Ptrcellmap, Vec3(0, 3.5f, 20.0f));
	}

	//落石の作成
	void GameStage::CreateFallingRocks() {
		AddGameObject<FallingRocks>();
	}
	

	void GameStage::OnCreate() {
		try {
			//ビューとライトの生成
			CreateViewLight();
			//フィールドの生成
			CreateField();
			//セルマップの生成
			CreateStageCellMap();
			//プレイヤーの生成
			CreatePlayer();
			//敵の生成
			CreateEnemy();
			// ゴーレムの生成
			CreateGolem();
			//落石の作成
			CreateFallingRocks();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
