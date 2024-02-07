/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	//�J�����̐���
	void GameStage::CreateViewLight() {
		auto ptrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto ptrMyCamera = ObjectFactory::Create<MyCamera>();
		ptrView->SetCamera(ptrMyCamera);
		ptrMyCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		ptrMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();

		AddGameObject<CameraObject>(ptrMyCamera);
	}

	// �ǂ̐���
	void GameStage::CreateWall() {
		vector<vector<Vec3>> vec = {
			{//�k��
				Vec3(50.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,10.0f,40.0f)//Position
			},
			{//���
				Vec3(50.0f, 20.0f, 5.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,10.0f,-40.0f)//Position
			},
			{//����
				Vec3(5.0f, 20.0f, 80.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(25.0f,10.0f,0.0f)//Position
			},
			{//����
				Vec3(5.0f, 20.0f, 80.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(-25.0f,10.0f,0.0f)//Position
			}
		};
		for (auto v : vec) {
			AddGameObject<Wall>(v[0], v[1], v[2]);
		}

		vector<vector<Vec3>> vec2 = {
			{//�V��
				Vec3(50.0f, 1.0f, 80.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,20.0f,0.0f)//Position
			}
		};
		for (auto v : vec2) {
			AddGameObject<Wall>(v[0], v[1], v[2]);
		}
	}

	// ���̐���
	void GameStage::CreateFloor() {
		// �n�ʂ̐���
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

	// ���̐���
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

	
	//�v���C���[�̐���
	void GameStage::CreatePlayer() {
		auto ptrPlayer = AddGameObject <Player>();
		SetSharedGameObject(L"Player", ptrPlayer);
		ptrPlayer->AddTag(L"Player");

		AddGameObject<PlayerHpBar>();
	}

	//�G�̍쐬
	void GameStage::CreateEnemy() {
		AddGameObject<EnemySpawn>();
	}

	void GameStage::CreateGolem() {
		auto ptrGolem = AddGameObject<Golem>(Vec3(0, 2.5f, 20.0f));
		SetSharedGameObject(L"Golem", ptrGolem);
		AddGameObject<GolemHpBar>();
	}

	//���΂̍쐬
	void GameStage::CreateFallingRocks() {
		AddGameObject<FallingRocks>();
	}

	void GameStage::PlayBGM() {
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		m_bgm = XAPtr->Start(L"GAMESTAGE_BGM", XAUDIO2_LOOP_INFINITE, 1.0f);
	}

	void GameStage::OnDestroy()
	{
		//BGM�̃X�g�b�v
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		XAPtr->Stop(m_bgm);
	}

	void GameStage::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̐���
			CreateViewLight();
			//�ǂ̐���
			CreateWall();
			//���̐���
			CreateFloor();
			//���̐���
			CreatePiller();
			//�v���C���[�̐���
			CreatePlayer();
			//�G�̐���
			CreateEnemy();
			// �S�[�����̐���
			CreateGolem();
			//���΂̍쐬
			CreateFallingRocks();
			//BGM�̐���
			PlayBGM();
		}
		catch (...) {
			throw;
		}
	}

	void GameStage::OnDraw()
	{
		// �f�o�b�O������������I�ɋ�ɂ���
		App::GetApp()->GetScene<Scene>()->SetDebugString(L"");

		// �p�����̕`�掞�̊֐������s����
		Stage::OnDraw();

		// �f�o�b�N�p������̕\����\���؂�ւ�
		const auto& debugStr = GetSharedObject(L"DebugString");
		debugStr->SetDrawActive(true);
	}
}
//end basecross
