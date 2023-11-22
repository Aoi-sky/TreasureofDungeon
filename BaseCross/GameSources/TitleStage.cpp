/*!
@file title.cpp
@brief �^�C�g�����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void TitleStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(Vec3(0.0f, 0.0f, -5.0f));
		PtrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}

	void TitleStage::CreateTitleSprite() {
		vector<vector<Vec3>> vec = {
			{
				Vec3(10.0f, 10.0f, 1.0f),//Scale
				Vec3(0.0f,0.0f,0.0f),//Rotation
				Vec3(0.0f,0.0f,5.0f)//Position
			},
		};
		for (auto v : vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}

		//AddGameObject<TitleSprite>();
	} // end CreateTitleSprite



	void TitleStage::OnCreate() {
		CreateViewLight();
		CreateTitleSprite();
	}

	void TitleStage::OnUpdate() {
		auto& app = App::GetApp();
		auto device = app->GetInputDevice(); 
		auto& pad = device.GetControlerVec()[0];

		if (pad.wPressedButtons & XINPUT_GAMEPAD_B) {

			PostEvent(1.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");

		}

	}

}
//end basecross
