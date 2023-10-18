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
		const Vec3 eye(0.0f, 3.0f, -5.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
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
