
/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	
	// テクスチャの読み込み
	void Scene::RoadTexture(const wstring& UseTextureName, const wstring& TextureDataName)
	{
		wstring dataDir;
		App::GetApp()->GetDataDirectory(dataDir);
		wstring strTexture = dataDir + L"Textures\\" + TextureDataName;
		App::GetApp()->RegisterTexture(UseTextureName, strTexture);
	}

	// サウンドの読み込み
	void Scene::RoadSound(const wstring& soundName, const wstring& soundDataName)
	{
		wstring dataDir;
		App::GetApp()->GetDataDirectory(dataDir);
		wstring strMusic = dataDir + L"Sounds\\" + soundDataName;
		App::GetApp()->RegisterWav(soundName, strMusic);
	}

	// 静的なモデルの読み込み
	void Scene::RoadStaticModelMesh(const wstring& staticModelbmfName, const wstring& staticModelMeshName)
	{
		wstring dataDir;
		App::GetApp()->GetDataDirectory(dataDir);

		App::GetApp()->RegisterResource(
			staticModelMeshName,
			MeshResource::CreateStaticModelMesh(dataDir, L"Models\\" + staticModelbmfName + L".bmf")
		);
	}

	// ボーンモデルの読み込み
	void Scene::RoadBoneModel(const wstring& boneModelbmfName, const wstring& boneModelMeshName, const wstring& boneModelMeshTangentName)
	{
		wstring dataDir;
		App::GetApp()->GetDataDirectory(dataDir);

		App::GetApp()->RegisterResource(boneModelMeshName,
			MeshResource::CreateBoneModelMesh(dataDir, L"Models\\" + boneModelbmfName + L".bmf"));

		App::GetApp()->RegisterResource(boneModelMeshTangentName,
			MeshResource::CreateBoneModelMeshWithTangent(dataDir, L"Models\\" + boneModelbmfName + L".bmf")
		);

	}

	void Scene::CreateResourses()
	{
		//テクスチャ
		RoadTexture(L"FIELD", L"field.png");
		RoadTexture(L"FLOOR", L"StoneFloor.png");
		RoadTexture(L"PILLAR", L"pillar2.png");
		RoadTexture(L"PLAYER", L"Player.png");
		RoadTexture(L"WALL", L"wall.jpg");


		RoadTexture(L"TITLE", L"title.png");
		RoadTexture(L"CLEAR", L"clear.png");
		RoadTexture(L"GAMEOVER", L"gameover.png");

		RoadTexture(L"GREENHPBAR", L"GreenHpBar.png");
		RoadTexture(L"REDHPBAR", L"RedHpBar.png");

		RoadTexture(L"ORANGESTAMINABAR", L"OrangeStaminaBar.png");
		RoadTexture(L"OCHERSTAMINABAR", L"OcherStaminaBar.png");

		RoadTexture(L"BPUSH", L"BPush.png");

		//スタティックモデル
		RoadStaticModelMesh(L"Rock/Rock3", L"FALLINGROCKS");

		const auto& app = App::GetApp();
		const wstring mediaPath = app->GetDataDirWString();// ディレクトリパスの設定
		const wstring modelPath = mediaPath + L"Models/";// モデルディレクトリパスの取得
	
		// ゴーレムのモデルの読込
		RegisterMultiMesh(L"GOLEM", modelPath + L"Golem/", L"Golem", true);

		// 水晶のモデルの読込
		RegisterMultiMesh(L"CRYSTAL", modelPath + L"Crystal/", L"Crystal", false);
		RegisterSingleMesh(L"CRYSTAL_R", modelPath + L"Crystal/", L"Crystal_R", false);
		RegisterSingleMesh(L"CRYSTAL_G", modelPath + L"Crystal/", L"Crystal_G", false);
		RegisterSingleMesh(L"CRYSTAL_B", modelPath + L"Crystal/", L"Crystal_B", false);

		//ShotEnemyの読込
		App::GetApp()->RegisterResource(L"ShootEnemy",
			MeshResource::CreateBoneModelMesh(modelPath, L"ShootEnemy/ShootEnemy.bmf"));

		// プレイヤーのモデルの読込
		RegisterMultiMesh(L"M_PLAYER", modelPath + L"Player/", L"Player", true);
		
		//BGM
		RoadSound(L"TITLESTAGE_BGM", L"TitleBGM.wav");
		RoadSound(L"GAMESTAGE_BGM", L"GameStageBGM4.wav");
		RoadSound(L"CLEARSTAGE_BGM", L"ClearStageBGM.wav");
		RoadSound(L"GAMEOVERSTAGE_BGM", L"GameOverStage.wav");
		//SE
		RoadSound(L"SELECT_SE", L"SelectSE.wav");
		RoadSound(L"STONE_SE", L"StoneSE.wav");
		RoadSound(L"GOLEM_SE", L"GolemSE.wav");
		RoadSound(L"HIT_SE", L"attack4.wav");
		RoadSound(L"ATTACK_SE", L"AttackSE.wav");
		RoadSound(L"Walk_SE", L"boss.wav");


	}

	void Scene::OnCreate(){
		try {
			//エフェクト作成
			m_EfkInterface = ObjectFactory::Create<EfkInterface>();

			// リソースの取得
			CreateResourses();

			//クリアする色を設定
			Col4 Col;
			Col.set(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
			SetClearColor(Col);
			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			//PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStage");
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToTitleStage");
		}
		catch (...) {
			throw;
		}
	}

	void Scene::OnUpdate()
	{
		SceneBase::OnUpdate();
		m_EfkInterface->OnUpdate();
	}

	void Scene::OnDraw()
	{
		SceneBase::OnDraw();
		auto& camera = GetActiveStage()->GetView()->GetTargetCamera();
		m_EfkInterface->SetViewProj(camera->GetViewMatrix(), camera->GetProjMatrix());
		m_EfkInterface->OnDraw();
	}

	Scene::~Scene() {
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//最初のアクティブステージの設定
			ResetActiveStage<GameStage>();
		}
		else if (event->m_MsgStr == L"ToTitleStage") {
			ResetActiveStage<TitleStage>();
		}
		else if (event->m_MsgStr == L"ToClearStage") {
			ResetActiveStage<ClearStage>();
		}
		else if (event->m_MsgStr == L"ToGameOverStage") {
			ResetActiveStage<GameOverStage>();
		}

	}

	void Scene::RegisterSingleMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse)
	{
		shared_ptr<MeshResource> modelMesh;
		if (boneUse)
		{
			modelMesh = MeshResource::CreateBoneModelMesh(path, fileName + L".bmf");
		}
		else
		{
			modelMesh = MeshResource::CreateStaticModelMesh(path, fileName + L".bmf");
		}
		const auto& app = App::GetApp();
		app->RegisterResource(registerKey, modelMesh);
	}

	void Scene::RegisterMultiMesh(const wstring& registerKey, const wstring& path, const wstring& fileName, bool boneUse)
	{
		shared_ptr<MultiMeshResource> modelMesh;
		if (boneUse)
		{
			modelMesh = MultiMeshResource::CreateBoneModelMultiMesh(path, fileName + L".bmf");
		}
		else
		{
			modelMesh = MultiMeshResource::CreateStaticModelMultiMesh(path, fileName + L".bmf");
		}
		const auto& app = App::GetApp();
		app->RegisterResource(registerKey, modelMesh);
	}
}
//end basecross
