
/*!
@file Scene.cpp
@brief �V�[������
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	
	// �e�N�X�`���̓ǂݍ���
	void Scene::RoadTexture(const wstring& UseTextureName, const wstring& TextureDataName)
	{
		wstring dataDir;
		App::GetApp()->GetDataDirectory(dataDir);
		wstring strTexture = dataDir + L"Textures\\" + TextureDataName;
		App::GetApp()->RegisterTexture(UseTextureName, strTexture);
	}

	// �T�E���h�̓ǂݍ���
	void Scene::RoadSound(const wstring& soundName, const wstring& soundDataName)
	{
		wstring dataDir;
		App::GetApp()->GetDataDirectory(dataDir);
		wstring strMusic = dataDir + L"Sounds\\" + soundDataName;
		App::GetApp()->RegisterWav(soundName, strMusic);
	}

	// �ÓI�ȃ��f���̓ǂݍ���
	void Scene::RoadStaticModelMesh(const wstring& staticModelbmfName, const wstring& staticModelMeshName)
	{
		wstring dataDir;
		App::GetApp()->GetDataDirectory(dataDir);

		App::GetApp()->RegisterResource(
			staticModelMeshName,
			MeshResource::CreateStaticModelMesh(dataDir, L"Models\\" + staticModelbmfName + L".bmf")
		);
	}

	// �{�[�����f���̓ǂݍ���
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
		//�e�N�X�`��
		RoadTexture(L"FIELD", L"field.png");
		RoadTexture(L"PILLAR", L"pillar2.png");
		RoadTexture(L"PLAYER", L"Player.png");
		RoadTexture(L"WALL", L"wall.jpg");


		RoadTexture(L"TITLE", L"title.png");
		RoadTexture(L"CLEAR", L"clear.png");
		RoadTexture(L"GAMEOVER", L"gameover.png");

		RoadTexture(L"GREENHPBAR", L"GreenHpBar.png");
		RoadTexture(L"REDHPBAR", L"RedHpBar.png");

		RoadTexture(L"BPUSH", L"BPush.png");

		//�X�^�e�B�b�N���f��
		RoadStaticModelMesh(L"Rock3", L"FALLINGROCKS");
		//RoadStaticModelMesh(L"ShootEnemy_Test", L"ShootEnemy");


		const auto& app = App::GetApp();
		const wstring mediaPath = app->GetDataDirWString();// �f�B���N�g���p�X�̐ݒ�
		const wstring modelPath = mediaPath + L"Models/";// ���f���f�B���N�g���p�X�̎擾
	
		// �S�[�����̃��f���̓ǂݍ���
		RegisterMultiMesh(L"GOLEM", modelPath + L"Golem/", L"Golem", true);
		//ShotEnemy�̓ǂݍ���
		RoadBoneModel(L"ShootEnemy", modelPath , L"ShootEnemy");

		// �v���C���[�̃��f���̓ǂݍ���
		RegisterMultiMesh(L"M_PLAYER", modelPath + L"Player/", L"Player01", true);

		
		//���y
		RoadSound(L"TITLESTAGE_BGM", L"TitleBGM.wav");
		RoadSound(L"GAMESTAGE_BGM", L"TitleBGM.wav");
		RoadSound(L"CLEARSTAGE_BGM", L"TitleBGM.wav");
		RoadSound(L"GAMEOVERSTAGE_BGM", L"TitleBGM.wav");
	}


	void Scene::OnCreate(){
		try {
			//�G�t�F�N�g�쐬
			m_EfkInterface = ObjectFactory::Create<EfkInterface>();

			// ���\�[�X�̎擾
			CreateResourses();

			//�N���A����F��ݒ�
			Col4 Col;
			Col.set(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
			SetClearColor(Col);
			//�������g�ɃC�x���g�𑗂�
			//����ɂ��e�X�e�[�W��I�u�W�F�N�g��Create���ɃV�[���ɃA�N�Z�X�ł���
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
			//�ŏ��̃A�N�e�B�u�X�e�[�W�̐ݒ�
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
