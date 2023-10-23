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
	void GameStage::CreateViewLight() {
		//const Vec3 eye(0.0f, 3.0f, -5.0f);
		//const Vec3 at(0.0f);
		//auto PtrView = CreateView<SingleView>();
		////�r���[�̃J�����̐ݒ�
		//auto PtrCamera = ObjectFactory::Create<Camera>();
		//PtrView->SetCamera(PtrCamera);
		//PtrCamera->SetEye(eye);
		//PtrCamera->SetAt(at);

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
	}
	//�Z���}�b�v�̍쐬
	void GameStage::CreateStageCellMap(){
		float PieceSize = 1.0f;
		auto Ptr = AddGameObject<StageCellMap>(Vec3(-12.0f, 0, 5.0f), PieceSize, 26, 16);
		//�Z���}�b�v�̋���\������ꍇ�̐ݒ�
		Ptr->SetDrawActive(true);
		SetSharedGameObject(L"StageCellMap", Ptr);
	}
	

	void GameStage::CreateField() {
		vector<vector<Vec3>> vec = {
			{
				Vec3(30.0f, 1.0f, 30.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,-0.5f,0.0f)//Position
			},
			{
				Vec3(4.0f, 1.0f, 1.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(10.0f,0.5f,0.0f)//Position
			},
			{
				Vec3(4.0f, 1.0f, 1.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(-10.0f,0.5f,0.0f)//Position
			},
			{
				Vec3(4.0f, 1.0f, 1.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,0.5f,10.0f)//Position
			},
			{
				Vec3(4.0f, 1.0f, 1.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,0.5f,-10.0f)//Position
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
	//�G�̍쐬
	void GameStage::CreateEnemy() {
		auto Ptrcellmap = GetSharedGameObject<StageCellMap>(L"StageCellMap");
		AddGameObject<Enemy>(Ptrcellmap, Vec3(0, 0.25f, 15.0f));

	}


	void GameStage::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�t�B�[���h�̍쐬
			CreateField();
			//�Z���}�b�v�̍쐬
			CreateStageCellMap();
			//�v���C���[�̍쐬
			CreatePlayer();
			//�G�̍쐬
			CreateEnemy();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
