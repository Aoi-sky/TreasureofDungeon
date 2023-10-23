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

	void GameStage::CreateField() {
		vector<vector<Vec3>> vec = {
			{//�n��
				Vec3(50.0f, 1.0f, 80.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,0.0f,0.0f)//Position
			},
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
				Vec3(0.0f, 20.0f, 80.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(25.0f,10.0f,0.0f)//Position
			},
			{//����
				Vec3(0.0f, 20.0f, 80.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(-25.0f,10.0f,0.0f)//Position
			},
			{//�V��
				Vec3(50.0f, 1.0f, 80.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,20.0f,0.0f)//Position
			}
		};
		for (auto v : vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}

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
	
	void GameStage::CreatePlayer() {
		auto ptrPlayer = AddGameObject <Player>();
		SetSharedGameObject(L"Player", ptrPlayer);
		ptrPlayer->AddTag(L"Player");

	}


	void GameStage::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�t�B�[���h�̍쐬
			CreateField();
			//�v���C���[�̍쐬
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
