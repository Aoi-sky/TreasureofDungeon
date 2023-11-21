
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
		RoadTexture(L"FIELD", L"field.png");
		RoadTexture(L"PILLAR", L"pillar2.png");
		RoadTexture(L"PLAYER", L"Player.png");
		RoadTexture(L"WALL", L"wall.jpg");

		RoadStaticModelMesh(L"Rock3", L"FALLINGROCKS");

		RoadBoneModel(L"Golem_Boss", L"GOLRM", L"GOLRM_TAN");
		RoadBoneModel(L"Player", L"M_PLAYER", L"PLAYER_TAN");
		

		const auto& app = App::GetApp();
		// ディレクトリパスの設定
		const wstring mediaPath = app->GetDataDirWString();

		// ゴーレムのテクスチャの読込
		//RoadTexture(L"Golem", L"Golem.png");
		//RoadTexture(L"Golem", L"Golem_Core.png");

		// モデルディレクトリパスの取得
		const wstring modelPath = mediaPath + L"Models/";
	
		// ゴーレムのモデルの読み込み
		RegisterMultiMesh(L"GOLEM", modelPath, L"Golem", true);
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
