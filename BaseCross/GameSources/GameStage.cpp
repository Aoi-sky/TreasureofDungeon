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

		AddGameObject<CameraObject>(ptrMyCamera);
	}

	
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

	// 壁の生成
	void GameStage::CreateWall() {
		vector<vector<Vec3>> vec = {
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
			}
		};
		for (auto v : vec) {
			AddGameObject<Wall>(v[0], v[1], v[2]);
		}

		vector<vector<Vec3>> vec2 = {
			{//天井
				Vec3(50.0f, 1.0f, 80.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,20.0f,0.0f)//Position
			}
		};
		for (auto v : vec2) {
			AddGameObject<Wall>(v[0], v[1], v[2]);
		}
	}

	// 床の生成
	void GameStage::CreateFloor() {
		// 地面の生成
		vector<vector<Vec3>> vec = {
			{
				Vec3(50.0f, 1.0f, 80.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,0.0f,0.0f)//Position
			},
		};
		for (auto v : vec) {
			AddGameObject<Floor>(v[0], v[1], v[2]);
		}
	}

	// 柱の生成
	void GameStage::CreatePiller() {
		vector<vector<Vec3>> vec = {
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
		for (auto v : vec) {
			AddGameObject<FixedCylinder>(v[0], v[1], v[2]);
		}
	}

	
	//プレイヤーの生成
	void GameStage::CreatePlayer() {
		auto ptrPlayer = AddGameObject <Player>();
		SetSharedGameObject(L"Player", ptrPlayer);
		ptrPlayer->AddTag(L"Player");

		AddGameObject<PlayerHpBar>();
	}

	//敵の作成
	void GameStage::CreateEnemy() {
		//auto Ptrcellmap = GetSharedGameObject<StageCellMap>(L"StageCellMap");
		//AddGameObject<Enemy>(Ptrcellmap, Vec3(0, 1.0f, 15.0f));
		AddGameObject<ShotEnemy>(Vec3(10.0f,2.0f,20.0f));
		AddGameObject<ShotEnemy>(Vec3(10.0f,2.0f,-20.0f));
		AddGameObject<ShotEnemy>(Vec3(-10.0f,2.0f,20.0f));
		AddGameObject<ShotEnemy>(Vec3(-10.0f,2.0f,-20.0f));
	}

	void GameStage::CreateGolem() {
		auto ptrGolem = AddGameObject<Golem>(Vec3(0, 2.5f, 20.0f));
		SetSharedGameObject(L"Golem", ptrGolem);
		AddGameObject<GolemHpBar>();
	}

	//落石の作成
	void GameStage::CreateFallingRocks() {
		AddGameObject<FallingRocks>();
	}
	

	void GameStage::OnCreate() {
		try {
			//ビューとライトの生成
			CreateViewLight();
			//壁の生成
			CreateWall();
			//床の生成
			CreateFloor();
			//柱の生成
			CreatePiller();
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

	void GameStage::OnDraw()
	{
		// デバッグ文字列を強制的に空にする
		App::GetApp()->GetScene<Scene>()->SetDebugString(L"");

		// 継承元の描画時の関数を実行する
		Stage::OnDraw();

		// デバック用文字列の表示非表示切り替え
		const auto& debugStr = GetSharedObject(L"DebugString");
		debugStr->SetDrawActive(true);
	}
}
//end basecross
