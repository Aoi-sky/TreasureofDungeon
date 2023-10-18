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
	void GameStage::CreateViewLight() {
		//const Vec3 eye(0.0f, 3.0f, -5.0f);
		//const Vec3 at(0.0f);
		//auto PtrView = CreateView<SingleView>();
		////ビューのカメラの設定
		//auto PtrCamera = ObjectFactory::Create<Camera>();
		//PtrView->SetCamera(PtrCamera);
		//PtrCamera->SetEye(eye);
		//PtrCamera->SetAt(at);

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

	void GameStage::CreateField() {
		vector<vector<Vec3>> vec = {
			{
				Vec3(10.0f, 1.0f, 10.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,0.0f,0.0f)//Position
			},
			{
				Vec3(10.0f, 10.0f, 10.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(10.0f,0.0f,0.0f)//Position
			},
			{
				Vec3(10.0f, 10.0f, 10.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(-10.0f,0.0f,0.0f)//Position
			},
			{
				Vec3(10.0f, 10.0f, 10.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,0.0f,10.0f)//Position
			},
			{
				Vec3(10.0f, 10.0f, 10.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,0.0f,-10.0f)//Position
			}
		};
		for (auto v : vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}

	}
	
	void GameStage::CreatePlayer() {
		auto ptrPlayer = AddGameObject <Player>();
		SetSharedGameObject(L"Player", ptrPlayer);
		ptrPlayer->AddTag(L"Player");

	}
	//敵の作成
	void GameStage::CreateEnemy() {
		auto Ptrcellmap = GetSharedGameObject<StageCellMap>;
		AddGameObject<Enemy>(Ptrcellmap, Vec3(0, 0.25f, 15.0f));

	}


	void GameStage::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();
			//フィールドの作成
			CreateField();
			//プレイヤーの作成
			CreatePlayer();
			//敵の作成
			CreateEnemy();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
